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
int break_while= 0 ;
pcap_t *handle;
void signal_handler(int sig) {
    if (sig == SIGINT) {
        // Přeruší smyčku pcap
        pcap_breakloop(handle); 
        break_while = 1;
    }
}

int main(int argc, char* argv[])
{
    
    signal(SIGINT, signal_handler);
    packet_info_t packet[10];
/**
 * @brief arguments program
 */
    int opt;
    arg_t args;
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
   // pcap_loop(handle, -1, packet_handler, (unsigned char *)&packet);
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    
    while (1) {
        pcap_dispatch(handle, 0, packet_handler, (unsigned char *)&packet);

        // Clear the screen and print the packet info
        clear();
        mvprintw(0, 0, "Src IP\t\tDst IP\t\tProto\tSrc Port\tDst Port");
        mvprintw(1, 0, "---------------------------------------------------------");

        for (int i = 0; i < MAX_PACKETS; i++) {
            mvprintw(i + 2, 0, "%s\t%s\t%s\t%d\t%d\t%lu\t%lu",
                     packet[i].src_ip, packet[i].dst_ip,
                     packet[i].protocol, packet[i].src_port, packet[i].dst_port, packet[i].rx_bytes, packet[i].tx_bytes);
        }

        // Refresh the screen
       refresh();

        // Wait 1 second before updating
        sleep(1);
        if (break_while== 1)
        {
           break;
        }
        
    }

    // Cleanup
    endwin();
    /**
     * @brief Used to free the memory allocated for the compiled filter stored in the bpf_program structure
     */
    pcap_freecode(&filter);

    pcap_close(handle);

    return 0;
}