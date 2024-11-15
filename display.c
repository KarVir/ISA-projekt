#include "display.h"
#include "speed.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>

void* statistics_thread(void* arg) {
    thread_args_t* thread_args = (thread_args_t*)arg;
    packet_vector_t* packets = thread_args->packets;

    initscr();
    cbreak();
    noecho();

    // Maximální šířky pro kombinace IP adres a portů
    
    // Hlavní smyčka pro zobrazení statistik
    while (!thread_args->break_while) {
        int max_src_ip_len = 0;
        int max_dst_ip_len = 0;
        int max_src_port_len = 0;
        int max_dst_port_len = 0;
        int src_ip_port_width =0;
        int dst_ip_port_width = 0;
        int proto_width = 6;

        pthread_mutex_lock(&thread_args->vector_mutex);
        clear();
        if (strcmp(thread_args->args.sorting_criteria, "b") == 0)
        {
            qsort(packets->data, packets->size, sizeof(packet_info_t), compare_bytes);
        }else if(strcmp(thread_args->args.sorting_criteria, "p") == 0)
        {
            qsort(packets->data, packets->size, sizeof(packet_info_t), compare_pakets);
        }

        
               // Určení maximálních délek pro IP adresy a porty
    for (int i = 0; i < packets->size; i++) {
        int src_ip_len = strlen(packets->data[i].src_ip);
        int src_port_len = snprintf(NULL, 0, "%u", packets->data[i].src_port);
        int dst_ip_len = strlen(packets->data[i].dst_ip);
        int dst_port_len = snprintf(NULL, 0, "%u", packets->data[i].dst_port);
    

        if (src_ip_len > max_src_ip_len) max_src_ip_len = src_ip_len;
        if (src_port_len > max_src_port_len) max_src_port_len = src_port_len;
        if (dst_ip_len > max_dst_ip_len) max_dst_ip_len = dst_ip_len;
        if (dst_port_len > max_dst_port_len) max_dst_port_len = dst_port_len;


    // Nastavení šířek pro výstupní formát
     src_ip_port_width = max_src_ip_len + max_src_port_len + 1; // Src IP a Src Port
     dst_ip_port_width = max_dst_ip_len + max_dst_port_len + 1; // Dst IP a Dst Port
    }

            for (int i = 0; i < 10; i++) {
                
              // Zobrazení hlavičky
        mvprintw(0, 0, "%-*s | %-*s | %-*s | Rx b/s p/s  |    Tx b/s p/s  ",
                 src_ip_port_width, "Src IP:Src Port",
                 dst_ip_port_width, "Dst IP:Dst Port",
                 proto_width, "Proto");
        mvprintw(1, 0, "%s", "----------------------------------------------------------------------------------------------------------------------------");

            speed(packets->data, i, thread_args->args.time);
           
            // Kombinace zdrojové a cílové IP adresy s porty pro zobrazení
            char src_ip_port[50];
            char dst_ip_port[50];
           
            if (packets->data[i].rx_byte == 0 && packets->data[i].tx_byte == 0) {
                continue;  // Přejít na další paket, pokud není aktivní
            }
            
            if (strcmp(packets->data[i].protocol, "icmp") != 0)
            {
                snprintf(src_ip_port, sizeof(src_ip_port), "%s:%u", packets->data[i].src_ip, packets->data[i].src_port);
                snprintf(dst_ip_port, sizeof(dst_ip_port), "%s:%u", packets->data[i].dst_ip, packets->data[i].dst_port);
                mvprintw(i + 2, 0, "%-*s | %-*s | %-*s | %.1f %s\t%.1f %s | %.1f %s\t%.1f %s",
                     src_ip_port_width, src_ip_port,
                     dst_ip_port_width, dst_ip_port,
                     proto_width, packets->data[i].protocol,
                     packets->data[i].rx_bites, packets->data[i].rx_units, packets->data[i].rx_packets, packets->data[i].rx_packets_units, 
                    packets->data[i].tx_bites, packets->data->tx_units, packets->data[i].tx_packets,  packets->data[i].rx_packets_units);
            }else
            {
                snprintf(src_ip_port, sizeof(src_ip_port), "%s", packets->data[i].src_ip );
            snprintf(dst_ip_port, sizeof(dst_ip_port), "%s", packets->data[i].dst_ip);
                mvprintw(i + 2, 0, "%-*s | %-*s | %-*s  | %.1f %s\t%.1f %s | %.1f %s\t%.1f %s",
                     src_ip_port_width, src_ip_port,
                     dst_ip_port_width, dst_ip_port,
                     proto_width, packets->data[i].protocol,
                     packets->data[i].rx_bites, packets->data[i].rx_units, packets->data[i].rx_packets, packets->data[i].rx_packets_units, 
                    packets->data[i].tx_bites, packets->data->tx_units, packets->data[i].tx_packets,  packets->data[i].rx_packets_units);
            }
            
            
            
            // Resetování hodnot
            packets->data[i].tx_bites = 0;
            packets->data[i].rx_bites = 0;
            packets->data[i].tx_packet_count = 0;
            packets->data[i].rx_packet_count = 0;
            packets->data[i].rx_byte = 0;
            packets->data[i].tx_byte = 0;
            strcpy(packets->data[i].tx_units, "");
            strcpy(packets->data[i].rx_units, "");
        }

        pthread_mutex_unlock(&thread_args->vector_mutex); // Odemknutí mutexu po přístupu k vektoru
        refresh();
        // Pauza mezi příkazy
            sleep(thread_args->args.time);

    }
        endwin();
            return NULL;

}  
 