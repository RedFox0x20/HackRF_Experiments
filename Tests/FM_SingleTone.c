#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <signal.h>
#include <libhackrf/hackrf.h>

#define F_1200_LEN 3636
#define F_CARRIER_LEN 10 

const uint64_t TxFreq = 144000000L;// 7078000;
const uint32_t SampleRate = 8000000;
const uint32_t Gain = 47;
static hackrf_device* HackRF = NULL;
static int8_t BuffReal[F_1200_LEN][F_CARRIER_LEN];
static int8_t BuffImag[F_1200_LEN][F_CARRIER_LEN];
static int should_run = 1;


int tx_callback(hackrf_transfer* Transfer)
{
    size_t i, j;
    static uint32_t n = 0;
    for (i = 0, j = 0; i < Transfer->valid_length/2; i++, j++)
    {
        Transfer->buffer[2*i] = BuffReal[i % F_1200_LEN][j % F_CARRIER_LEN];
        Transfer->buffer[2*i+1] = BuffImag[i % F_1200_LEN][j % F_CARRIER_LEN];
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
    size_t i, j;
    double SignalAngle;
    double CarrierAngle;
    double ModulationDepth = 4.0 / 3.0; 
    for (i = 0; i < F_1200_LEN; i++)
    {
        SignalAngle = i * 2 * M_PI / F_1200_LEN; 
        for (j = 0; j < F_CARRIER_LEN; j++)
        {
            CarrierAngle = j * 2 * M_PI / F_CARRIER_LEN;
            BuffReal[i][j] = (int8_t)(127.0 * cos(CarrierAngle - ModulationDepth * cos(SignalAngle)));
            BuffImag[i][j] = (int8_t)(127.0 * sin(CarrierAngle - ModulationDepth * cos(SignalAngle)));
        }
    }

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
