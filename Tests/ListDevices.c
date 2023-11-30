#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <signal.h>
#include <libhackrf/hackrf.h>

int main(int argc, char** argv)
{
    int i;
    hackrf_device_list_t* HackRFDevices = NULL;

    printf("Init hackrf\n");
    hackrf_init();

    HackRFDevices = hackrf_device_list();
    printf("Detected devices usb_devicecount: %i\n", HackRFDevices->usb_devicecount);
    printf("Detected HackRF devices devicecount: %i\n", HackRFDevices->devicecount);
    for (i = 0; i < HackRFDevices->devicecount; i++)
    {
        printf("--------- HackRF Device %i --------\n", i);
        printf("Serial: %s\n", HackRFDevices->serial_numbers[i]);
        printf("Type: 0x%X\n", HackRFDevices->usb_board_ids[i]);
        printf("USB Device Index: %i\n", HackRFDevices->usb_device_index[i]);

    }
    hackrf_device_list_free(HackRFDevices);
    hackrf_exit();

    return 0;
}
