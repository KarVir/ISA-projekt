/**
 * @file main.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @language english
 * 
 * @date 2024-10-21 
 */

#include <stdio.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <pcap/pcap.h>

int main(int argc, char* argv[])
{

/**
 * @brief arguments program
 */
    int opt;
    int option_index = 0;
    char interface[255] = "";
    int interface_flag = 0;
    char err_buf[PCAP_ERRBUF_SIZE];
    //pcap_t *handle;

    static struct option long_options[] = {
        {"interface", optional_argument, NULL, 'i'},
        {"sorting", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };


    while ((opt = getopt_long(argc, argv, "i:s:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                if (argv[optind] == NULL) {
                    interface_flag = 1;
                    break;
                }
                strncpy(interface, argv[optind], 254);

                interface_flag = 1;
                break;
            case 's':
                printf("s zadáno");
                break;
            default:
                printf("Usage: ./isa-top -i int [-s b|p]");
                exit(EXIT_FAILURE);
        }
    }

    printf("ok");
    return 0;
}