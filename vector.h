/**
 * @file vector.g
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */
 /**
  * TODO ceske komenty pryc
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <pthread.h>
#include <stdbool.h>

#ifndef VECTOR_H
#define VECTOR_H

/**
 * @brief Struct of arguments program
 */
typedef struct {
    int time;
    char interface[255];
    char sorting_criteria[2];
    char pcap_file_path[255];
    int interface_provided;
    int pcap_file_provided;
} arg_t;

/**
 * @brief Information about packet
 */
typedef struct packet_info_t {
    char src_ip[INET6_ADDRSTRLEN];   // Zdrojová IP adresa
    char dst_ip[INET6_ADDRSTRLEN];   // Cílová IP adresa
    unsigned int src_port;  // Zdrojový port
    unsigned int dst_port;  // Cílový port
    char protocol[8];   // Protokol (TCP, UDP atd.)
    unsigned int byte;  //velikost paketu 
    unsigned int rx_byte;
    unsigned int tx_byte;
    float rx_bites;  
    float tx_bites;
    char rx_units[2];
    char tx_units[2];
    float rx_packets;
    float tx_packets;
    int tx_packet_count;
    int rx_packet_count;
    char rx_packets_units[2];
    char tx_packets_units[2];
} packet_info_t;

/**
 * @brief Structure for storing packets
 */
typedef struct packet_vector_t {
    packet_info_t *data;  // Dynamické pole struktur packet_info_t
    int size;          // Current number of elements
    int capacity;      // Maximum capacity
} packet_vector_t;

/**
 * @brief Global struct
 */
typedef struct {
    pcap_t *handle;
    packet_vector_t *packets;
    pthread_mutex_t vector_mutex; 
    arg_t args;
    bool break_while;
} thread_args_t;

/**
 * @brief Initialization vector
 */
void init_packet_vector(packet_vector_t *vector);

/**
 * @brief Adding packets to the vector
 */
void add_packet_to_vector(void* arg, packet_info_t packet);

/**
 * @brief Release vector
 */
void free_packet_vector(packet_vector_t *vector);
#endif // VECTOR_H