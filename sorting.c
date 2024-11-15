/**
 * @file sorting.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "packet_handler.h"
#include "sorting.h"

int compare_bytes(const void* a, const void* b) {
    const packet_info_t* packetA = (const packet_info_t*)a;
    const packet_info_t* packetB = (const packet_info_t*)b;

    if ((packetA->rx_byte + packetA->tx_byte) > (packetB->rx_byte + packetB->tx_byte)) return -1;
    if ((packetA->rx_byte + packetA->tx_byte) < (packetB->rx_byte + packetB->tx_byte)) return 1;
    return 0;
}

int compare_pakets(const void* a, const void* b) {
    const packet_info_t* packetA = (const packet_info_t*)a;
    const packet_info_t* packetB = (const packet_info_t*)b;

    if ((packetA->rx_packets + packetA->tx_packet_count) > (packetB->rx_packets + packetB->tx_packet_count)) return -1;
    if ((packetA->rx_packets + packetA->tx_packet_count) < (packetB->rx_packets + packetB->tx_packet_count)) return 1;
    return 0;
}