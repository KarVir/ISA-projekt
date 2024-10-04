/**
 * @file main.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "paket_handler.h"

int compare_bytes(const void *a, const void *b) {
    const packet_info_t *conn_a = (const packet_info_t *)a;
    const packet_info_t *conn_b = (const packet_info_t *)b;
    
    uint16_t res_a = conn_a->rx_bytes + conn_a->tx_bytes;
    uint16_t res_b = conn_b->rx_bytes + conn_b->tx_bytes;


   // seřazení v sestupném pořadí podle rx_bytes
    if (res_a < res_b) {
        return 1;
    } else if (res_a > res_b) {
        return -1;
    } else {
        return 0; // Pokud jsou hodnoty stejné
    }
}

int compare_pakets(const void *a, const void *b) {
    const packet_info_t *conn_a = (const packet_info_t *)a;
    const packet_info_t *conn_b = (const packet_info_t *)b;
    
    uint16_t res_a = conn_a->rx_packets + conn_a->tx_packets;
    uint16_t res_b = conn_b->rx_packets + conn_b->tx_packets;
   // Seřazení v sestupném pořadí podle rx_bytes
    if (res_a < res_b) {
        return 1;
    } else if (res_a > res_b) {
        return -1;
    } else {
        return 0; // Pokud jsou hodnoty stejné
    }
}
