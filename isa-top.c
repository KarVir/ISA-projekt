/**
 * @file main.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
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


//void prevod_jednotek(unsigned char *user, int number) {
    // packet_info_t *packets = (packet_info_t *)user;
    
    // if (packets[number].rx_bytes >= 1000000000) {
    //     packets[number].rx_bytes /= 1000000000.0; // Převod na GB
    //         //strcpy(packets[number].units, "G");
    // } else if (packets[number].rx_bytes >= 1000000) {
    //     packets[number].rx_bytes /= 1000000.0; // Převod na MB
    //     //strcpy(packets[number].units, "M");
    // } else if (packets[number].rx_bytes >= 1000) {
    //     packets[number].rx_bytes /= 1000.0; // Převod na kB
    //         //strcpy(packets[number].units, "K");
    // }
//}

void speed(unsigned char *user, int number, int time){
    packet_info_t *packets= (packet_info_t *)user;
   

    packets[number].tx_bytes = (packets[number].tx_bytes * 8)/ time;

    packets[number].tx_packets = packets[number].count/ time;

   // prevod_jednotek((unsigned char *)packets, number);
    
}


int main(int argc, char* argv[])
{
    
    signal(SIGINT, signal_handler);
    packet_info_t packet[10000] = {{"", "", 0, 0, "", 0, 0, 0, 0, 0}};
    
    int opt;
    int interface_flag = 0;
    arg_t args= {0,"", "b"};

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
                    interface_flag = 1;
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


    //If no interface is provided, print a list of available interfaces and exit
    if (strcmp(args.interface, "") == 0) {
        if ((interface_flag == 1 && argc == 2) || (interface_flag == 0 && argc == 1)) {
            pcap_if_t *interfaces;
                           // pcap_findalldevs returns a linked list of available interfaces
                if (pcap_findalldevs(&interfaces, err_buf) == 0) {
                    printf("\n");
                    // loop through all interfaces and print their names
                    for (pcap_if_t *interface = interfaces; interface != NULL; interface = interface->next) {
                        printf("%s\n", interface->name);
                    }
                    printf("\n");
                    // free the memory allocated by pcap_findalldevs
                    pcap_freealldevs(interfaces);
                } else {
                    // if pcap_findalldevs fails, print an error message
                }
            return 0;
        } else {
            exit(1);
        }
    }
        
    if (args.time == 0)
    {
        fprintf(stderr, "Error not zero time\n");
        exit(2);
    }
    
    if ((strcmp(args.sorting_criteria, "b") == 0) || (strcmp(args.sorting_criteria, "p") == 0))
    {
    }else{
    fprintf(stderr, "Error: Invalid sorting option\n");
    exit(1);
    }      
                
    //printf("Zacatek i %s, s %s, t %d\n", args.interface, args.sorting_criteria, args.time);

    handle = pcap_open_live(args.interface, BUFSIZ, 1, 1000, err_buf);
    if (handle == NULL) {
        fprintf(stderr, "Error: invalid interface\n");
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
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    while (1) {
        clear();
        pcap_dispatch(handle, 10, packet_handler, (unsigned char *)&packet);
        if (strcmp(args.sorting_criteria, "b") == 0)
        {
            qsort(packet, 10, sizeof(packet_info_t), compare_bytes);
        }else if(strcmp(args.sorting_criteria, "p") == 0)
        {
            qsort(packet, 10, sizeof(packet_info_t), compare_pakets);
        }
        mvprintw(0, 0, "Src IP: Src Port\t\tDst IP: Dst Port\t\tProto\t\tRx\t\t\tTx");
        mvprintw(1, 0, "---------------------------------------------------------");
        for (int i = 0; i < 10; i++)
        {
            speed((unsigned char *)&packet, i, args.time);
            mvprintw(i + 2, 0, "%s: %u\t%s: %u\t%s\t%u\t%lu\t%u\t%lu\n", packet[i].src_ip, packet[i].src_port, 
            packet[i].dst_ip, packet[i].dst_port, packet[i].protocol, packet[i].rx_bytes, packet[i].rx_packets,
            packet[i].tx_bytes, packet[i].tx_packets);
            packet[i].count=0;

        }
        refresh();
                // Wait 1 second before updating
        if (break_while == 1)
        {
           break;
        }
        sleep(args.time);

    }
    endwin();
    pcap_freecode(&filter);

    pcap_close(handle);

    return 0;
}