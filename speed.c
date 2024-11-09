/**
 * @file speed.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "speed.h"

float unit_conversion(float value, char *units){
  if (value >= 1000000000) {
        strcpy(units, "G");
        return value / 1000000000.0; // G
    } else if (value >= 1000000) {
        strcpy(units, "M");
        return value / 1000000.0; // M
    } else if (value >= 1000) {
        strcpy(units, "k");
        return value / 1000.0; // k
    } else {
        strcpy(units, "");
        return value; // no
    }
}

void speed(packet_info_t* user, int number, int time){
    packet_info_t *packets= user;

    // Calculation b/s
    packets[number].tx_bites = (packets[number].tx_byte * 8)/ time;
    packets[number].rx_bites = (packets[number].rx_byte * 8)/ time;

    // Calculation p/s
    packets[number].tx_packets = packets[number].tx_packet_count/ time;
    packets[number].rx_packets = packets[number].rx_packet_count/ time;

    // Conversion to units
    packets[number].tx_bites = unit_conversion(packets[number].tx_bites, packets[number].tx_units);
    packets[number].rx_bites = unit_conversion(packets[number].rx_bites, packets[number].rx_units);
    packets[number].rx_packets = unit_conversion(packets[number].rx_packets, packets[number].rx_packets_units);
    packets[number].tx_packets = unit_conversion(packets[number].tx_packets, packets[number].tx_packets_units);

    } 