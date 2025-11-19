/* Flash multiple partitions example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <termios.h>
#include <fcntl.h>
#include "generic_linux_port.h"
#include "esp_loader.h"
#include "example_common.h"

#define TARGET_RST_Pin 2
#define TARGET_IO0_Pin 3

#define DEFAULT_BAUD_RATE 115200
#define HIGHER_BAUD_RATE  460800
#define SERIAL_DEVICE     "/dev/ttyUSB2"



int main(void)
{
    example_binaries_t bin;

    const loader_linux_config_t config = {
        .device = SERIAL_DEVICE,
        .baudrate = DEFAULT_BAUD_RATE
    };

    loader_port_linux_init(&config);

    if (connect_to_target(DEFAULT_BAUD_RATE) == ESP_LOADER_SUCCESS) {

        get_example_binaries(esp_loader_get_target(), &bin);

        printf("Loading bootloader...\n");
        flash_binary(bin.boot.data, bin.boot.size, bin.boot.addr);
        printf("Loading partition table...\n");
        flash_binary(bin.part.data, bin.part.size, bin.part.addr);
        printf("Loading app...\n");
        flash_binary(bin.app.data,  bin.app.size,  bin.app.addr);
        printf("Done!\n");
        esp_loader_reset_target();
        loader_port_deinit();

        struct termios options;
        int status, fd;

        if ((fd = open (config.device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1) {
            printf("Error occurred while opening serial port !\n");
            return -1 ;
        }

        fcntl (fd, F_SETFL, O_RDWR) ;

        // Get and modify current options:

        tcgetattr (fd, &options);
        speed_t baud = B1152000;

        cfmakeraw   (&options);
        cfsetispeed (&options, baud);

        printf("********************************************\n");
        printf("*** Logs below are print from slave .... ***\n");
        printf("********************************************\n");

        // Delay for skipping the boot message of the targets
        sleep(5);
        while (1) {
            char read_buf [256];
	        int n = read(fd, &read_buf, sizeof(read_buf));
            printf("%s", read_buf);
        }
    }

}