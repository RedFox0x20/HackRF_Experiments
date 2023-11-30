# HackRF Experiments
Test signal generation for the HackRF

# Tests
The HackRF is set to a Tx frequency of 144MHz in these example programmes. This
is within the 2 meter amateur band. A dummy load should be used when testing.
- FM\_SingleTone.c: Transmit a constant tone under FM, Inspired by [Rufty/hackrf_beep](https://github.com/rufty/hackrf_beep)
- FM\_FSK.c: Transmit a 10Bd FM modulated FSK switching between 1200 and 2400Hz on an 800KHz carrier. Uses similar implementation to above
- FSK.c: Transmit a 10Bd FSK switching between carrier 800KHz and 801.2KHz.
- MFSK.c: Transmit a 10Bd MFSK using freqs carrier 800kHz + 1200, 2400, 3600Hz
- Sweep.c: Transmit a sweeping tone from 1 to 4000Hz at a rate of 4Hz
