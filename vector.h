/**
 * @file vector.g
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <pthread.h>

#ifndef VECTOR_H
#define VECTOR_H

/**
 * @brief struktura zahrnující argumenty programu
 */
typedef struct {
    int time;
    char interface[255];
    char sorting_criteria[2];
} arg_t;

/**
 * @brief 
 */
typedef struct packet_info_t {
    char src_ip[16];   // Zdrojová IP adresa
    char dst_ip[16];   // Cílová IP adresa
    unsigned int src_port;  // Zdrojový port
    unsigned int dst_port;  // Cílový port
    char protocol[8];  
    unsigned int byte; // Protokol (TCP, UDP atd.)
    unsigned int rx_byte;  // Přijaté bajty
    unsigned int tx_byte;
    float rx_bites;  // Odeslané bajty
    float tx_bites;
    char rx_units[2];
    char tx_units[2];
     uint16_t rx_packets;
     uint16_t tx_packets;
} packet_info_t;

/**
 * @brief 
 */
typedef struct packet_vector_t {
    packet_info_t *data;  // Dynamické pole struktur packet_info_t
    int size;          // Aktuální počet prvků
    int capacity;      // Maximální kapacita
} packet_vector_t;

/**
 * @brief globalní struktura
 */
typedef struct {
    pcap_t *handle;
    packet_vector_t *packets;
    pthread_mutex_t vector_mutex; 
    arg_t args;
    bool break_while;
} thread_args_t;

/**
 * @brief Funkce pro inicializaci vektoru
 * 
 * @param vector 
 */
void init_packet_vector(packet_vector_t *vector);

/**
 * @brief přidání paketů do vektru
 * 
 * @param arg 
 * @param packet 
 */
void add_packet_to_vector(void* arg, packet_info_t packet);

/**
 * @brief uvolnění vektru
 * 
 * @param vector 
 */
void free_packet_vector(packet_vector_t *vector);
#endif // VECTOR_H