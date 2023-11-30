#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <signal.h>
#include <libhackrf/hackrf.h>

const uint64_t TxFreq = 144000000L;
const uint32_t SampleRate = 8000000;
const uint32_t Gain = 47;
static int should_run = 1;

/*
 * The following values determine the number of samples for a full wavelength
 * for a frequency at the given sample rate.
 * ---- SampleRate / Frequency = N  ----
 * 800kHz: 8000000 / 800000 = 10
 */
#define F_CARRIER_LEN 10

int tx_callback(hackrf_transfer* Transfer)
{
    size_t i;
    static double CarrierAngle     = 0;
    static double CarrierAngleDiff = 2 * M_PI / F_CARRIER_LEN;

    const double SweepLowFreq = 1;
    const double SweepHighFreq = 4000;
    const double SweepRate = 4;
    const double SweepFreqDelta = 
        ((SweepHighFreq - SweepLowFreq) / SampleRate) * SweepRate;
    static double SweepFreq = SweepLowFreq;

    for (i = 0; i < Transfer->valid_length/2; i++, SweepFreq += SweepFreqDelta) 
    {
        if (SweepFreq > SweepHighFreq) { SweepFreq = SweepLowFreq; }
        const double SweepAngle = 2 * M_PI / (SampleRate / SweepFreq);

        CarrierAngle = fmodf(
                CarrierAngle + CarrierAngleDiff + SweepAngle, 2*M_PI);

        Transfer->buffer[2*i] = (int8_t)(127.0 * cos(CarrierAngle));

        Transfer->buffer[2*i+1] = (int8_t)(127.0 * sin(CarrierAngle));

    }
    return 0;
}

void sigint_callback_handler(int signum)
{
    fprintf(stderr, "Caught signal %i\n", signum);
    should_run = 0;
}

int main(int argc, char** argv)
{
    hackrf_device* HackRF = NULL;

    printf("Init hackrf\n");
    hackrf_init();
    hackrf_open(&HackRF);
    hackrf_set_sample_rate_manual(HackRF, SampleRate, 1);
    hackrf_set_baseband_filter_bandwidth(
            HackRF,
            hackrf_compute_baseband_filter_bw_round_down_lt(SampleRate));
    hackrf_set_txvga_gain(HackRF, Gain);
    hackrf_start_tx(HackRF, tx_callback, NULL);
    hackrf_set_freq(HackRF, TxFreq);
    hackrf_set_amp_enable(HackRF, 1);

    while (hackrf_is_streaming(HackRF) && should_run) ;

    signal ( SIGINT, &sigint_callback_handler ) ;
    signal ( SIGILL, &sigint_callback_handler ) ;
    signal ( SIGFPE, &sigint_callback_handler ) ;
    signal ( SIGSEGV, &sigint_callback_handler ) ;
    signal ( SIGTERM, &sigint_callback_handler ) ;
    signal ( SIGABRT, &sigint_callback_handler ) ;


    hackrf_stop_tx(HackRF);
    hackrf_close(HackRF);
    hackrf_exit();

    return 0;
}
