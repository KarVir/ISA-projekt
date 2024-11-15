/**
 * @file vector.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "vector.h"

void init_packet_vector(packet_vector_t *vector) {
    vector->capacity = 10;
    vector->size = 0;
    vector->data = (packet_info_t*)malloc(vector->capacity * sizeof(packet_info_t));
    
    if (vector->data == NULL) {
        // Error allocating memory
        exit(1);
    }
   
    for (int i = 0; i < vector->capacity; i++) {
        vector->data[i].dst_port = 0;
        vector->data[i].src_port = 0;
        vector->data[i].byte = 0;
        vector->data[i].rx_byte = 0;
        vector->data[i].tx_byte = 0;
        vector->data[i].tx_bites = 0;
        vector->data[i].rx_bites = 0;
        vector->data[i].tx_packets = 0;
        vector->data[i].rx_packets = 0;
        vector->data[i].tx_packet_count = 0;
        vector->data[i].rx_packet_count = 0;

        strcpy(vector->data[i].dst_ip, "");
        strcpy(vector->data[i].src_ip, "");
        strcpy(vector->data[i].protocol, "");
        strcpy(vector->data[i].tx_units, "");
        strcpy(vector->data[i].rx_units, "");
        strcpy(vector->data[i].tx_packets_units, "");
        strcpy(vector->data[i].rx_packets_units, "");
    }
}

void add_packet_to_vector(void* thread_args, packet_info_t packet) {
    thread_args_t* args =(thread_args_t*) thread_args;
    int found = 0;

    //Looking for the same packet

    for (int i = 0; i < args->packets->size; i++) {
        // compare IP addresses and ports
        // same src and dst address and same ports
        if ((strcmp(args->packets->data[i].src_ip, packet.src_ip) == 0 &&
             strcmp(args->packets->data[i].dst_ip, packet.dst_ip) == 0 &&
             args->packets->data[i].src_port == packet.src_port &&
             args->packets->data[i].dst_port == packet.dst_port)) {
            // paket byl nalezen, aktualizace hodnoty tx_bytes
            args->packets->data[i].tx_byte += packet.byte;
            args->packets->data[i].tx_packet_count += 1;
            found = 1;
            break;
        // src and dst address with ports are reversed
        } else if (strcmp(args->packets->data[i].src_ip, packet.dst_ip) == 0 &&
           strcmp(args->packets->data[i].dst_ip, packet.src_ip) == 0 &&
           args->packets->data[i].src_port == packet.dst_port &&
           args->packets->data[i].dst_port == packet.src_port) {
        args->packets->data[i].rx_byte += packet.byte;
        args->packets->data[i].rx_packet_count++;
    found = 1;
    break;
}   
    }

    // if the capacity is full, it increases
    if (!found) {
    
     if (args->packets->size == args->packets->capacity) {
        args->packets->capacity *= 2; // Increase capacity
        packet_info_t* temp = (packet_info_t*)realloc(args->packets->data, args->packets->capacity * sizeof(packet_info_t));

       // Checj realloc
     if (temp == NULL) {    
       fprintf(stderr, "Chyba při realokaci paměti pro pakety.\n");
            exit(1);
        }
            
         args->packets->data = temp; 
     }

    // Sdd a new packet to the end of the vector
    packet.tx_packet_count += 1;
    packet.tx_byte = packet.byte;
    args->packets->data[args->packets->size] = packet;
    args->packets->size++;
    }
}

void free_packet_vector(packet_vector_t *vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}