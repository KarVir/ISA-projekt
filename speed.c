/**
 * @file speed.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "speed.h"

void unit_conversion(packet_info_t* user, int number){
     packet_info_t *packets = user;
    
    if (packets[number].tx_bites >= 1000000000) {
        packets[number].tx_bites /= 1000000000.0; // Převod na Gb
        strcpy(packets[number].tx_units, "G");
     } else if (packets[number].tx_bites >= 1000000) {
        packets[number].tx_bites /= 1000000.0; // Převod na Mb
        strcpy(packets[number].tx_units, "M");
    } else if (packets[number].tx_bites >= 1000) {
        packets[number].tx_bites /= 1000.0; // Převod na kb
        strcpy(packets[number].tx_units, "k");
    }

    if (packets[number].rx_bites >= 1000000000) {
        packets[number].rx_bites /= 1000000000.0; // Převod na Gb
        strcpy(packets[number].rx_units, "G");
     } else if (packets[number].rx_bites >= 1000000) {
        packets[number].rx_bites /= 1000000.0; // Převod na Mb
        strcpy(packets[number].rx_units, "M");
    } else if (packets[number].rx_bites >= 1000) {
        packets[number].rx_bites /= 1000.0; // Převod na kb
        strcpy(packets[number].rx_units, "k");
    }
}

void speed(packet_info_t* user, int number, int time){
    packet_info_t *packets= user;

    packets[number].tx_bites = (packets[number].tx_byte * 8)/ time;

    packets[number].rx_bites = (packets[number].rx_byte * 8)/ time;

   // packets[number].tx_packets = packets[number].count/ time;

   unit_conversion(packets, number);
} 