/**
 * @file display.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "display.h"
#include "speed.h"
#include <ncurses.h>

void* statistics_thread(void* arg) {
    thread_args_t* thread_args = (thread_args_t*)arg;
    packet_vector_t* packets = thread_args->packets;
    int count = 0;

    initscr();
    cbreak();
    noecho();

    while (!thread_args->break_while) {
        pthread_mutex_lock(&thread_args->vector_mutex);
                clear();

        if (strcmp(thread_args->args.sorting_criteria, "b") == 0)
        {
            qsort(packets->data, packets->size, sizeof(packet_info_t), compare_bytes);
        }else if(strcmp(thread_args->args.sorting_criteria, "p") == 0)
        {
            qsort(packets->data, packets->size, sizeof(packet_info_t), compare_pakets);
        }
        // Výpis statistik
        mvprintw(0, 0, "Src IP: Src Port\t\tDst IP: Dst Port\t\tProto\t\tRx\t\t\tTx");
        mvprintw(1, 0, "---------------------------------------------------------");
        for (int i = 0; i < 10; i++)
        {
            speed(packets->data, i, thread_args->args.time);
            mvprintw(i + 2, 0, "%s: %u\t%s: %u\t%s\t%.1f %s\t%.1f %s\t%.1f %s\t%.1f %s\n", packets->data[i].src_ip, packets->data[i].src_port, 
            packets->data[i].dst_ip, packets->data[i].dst_port, packets->data[i].protocol,
            packets->data[i].rx_bites, packets->data[i].rx_units, packets->data[i].rx_packets, packets->data[i].rx_packets_units, 
            packets->data[i].tx_bites, packets->data->tx_units, packets->data[i].tx_packets,  packets->data[i].rx_packets_units);
            packets->data[i].tx_bites = 0;
            packets->data[i].rx_bites = 0;
            packets->data[i].tx_packet = 0;
            packets->data[i].rx_packet = 0;
            packets->data[i].rx_byte = 0;
            packets->data[i].tx_byte = 0;
            strcpy(packets->data[i].tx_units, "");
            strcpy(packets->data[i].rx_units, "");
        }

        pthread_mutex_unlock(&thread_args->vector_mutex); // Odemknutí mutexu po přístupu k vektoru
                refresh();
        count++;
        sleep(thread_args->args.time);  // Přidání krátké pauzy mezi iteracemi
    }
    
    endwin();
    return NULL;
}