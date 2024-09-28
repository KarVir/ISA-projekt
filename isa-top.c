/**
 * @file main.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @language english
 * 
 * @date 2024-10-21 
 */

#include "paket_handler.h"


int main(int argc, char* argv[])
{

/**
 * @brief arguments program
 */
    int opt;
    arg_t args;
    pcap_t *handle;
    char err_buf[PCAP_ERRBUF_SIZE];
    char filter_exp[3] = "ip";
    struct bpf_program filter;
    /**
     * Parsing input arguments
    */
    while ((opt = getopt(argc, argv, "i:s:t:")) != -1) {
        switch (opt) {
            case 'i':
                if (optarg == NULL) {
                     
                    break;
                }
                strncpy(args.interface, optarg, 254);
                break;
            case 's':
                if (optarg == NULL) {
                     
                    break;
                }
                strncpy(args.sorting_criteria, optarg, 1);
                break;
            case 't':
                if (optarg == NULL) {
                     
                    break;
                }
                args.time = atoi(optarg);
                break;

            default:
                return EXIT_FAILURE;
        }
    }

    //printf("Zacatek i %s, s %s, t %d\n", args.interface, args.sorting_criteria, args.time);

    handle = pcap_open_live(args.interface, BUFSIZ, 1, 1000, err_buf);
    if (handle == NULL) {
        exit(2);
    }

    /**
    * @brief Used to translate a text string containing a BPF (Berkeley Packet Filter)
    */
    if (pcap_compile(handle, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        exit(2);
    }

    if (pcap_setfilter(handle, &filter) == -1) {
        exit(2);
    }

    /**
     * @brief Used to capture packets on the specified capture interface within a loop.
     */
    pcap_loop(handle, -1, packet_handler, (unsigned char *)&args);

    /**
     * @brief Used to free the memory allocated for the compiled filter stored in the bpf_program structure
     */
    pcap_freecode(&filter);

    pcap_close(handle);

    return 0;
}