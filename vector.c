/**
 * @file vector.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "vector.h"

void init_packet_vector(packet_vector_t *vector) {
    vector->capacity = 10; // nebo jiná počáteční kapacita
    vector->size = 0;
    vector->data = (packet_info_t*)malloc(vector->capacity * sizeof(packet_info_t));
    
    if (vector->data == NULL) {
        // Chyba při alokaci paměti
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

        strcpy(vector->data[i].dst_ip, "");
        strcpy(vector->data[i].src_ip, "");
        strcpy(vector->data[i].protocol, "");
        strcpy(vector->data[i].tx_units, "");
        strcpy(vector->data[i].rx_units, "");
    }
}

void add_packet_to_vector(void* thread_args, packet_info_t packet) {
    // Získání ukazatele na thread_args
    thread_args_t* args =(thread_args_t*) thread_args;
    int found = 0;

    // Procházejte všechny pakety v poli
    for (int i = 0; i < args->packets->size; i++) {
        packet_info_t *current_packet = &args->packets->data[i];

        // Porovnání IP adres a portů (src a dst musí být stejné)
        if ((strcmp(current_packet->src_ip, packet.src_ip) == 0 &&
             strcmp(current_packet->dst_ip, packet.dst_ip) == 0 &&
             current_packet->src_port == packet.src_port &&
             current_packet->dst_port == packet.dst_port)) {

            // Paket byl nalezen, aktualizace hodnoty tx_bytes
            current_packet->tx_byte += packet.byte;
            current_packet->tx_packets += 1;
            found = 1;
            break;
        }else if (strcmp(current_packet->src_ip, packet.dst_ip) == 0 &&
             strcmp(current_packet->dst_ip, packet.src_ip) == 0 &&
             current_packet->src_port == packet.dst_port &&
             current_packet->dst_port == packet.src_port)
        {
           current_packet->rx_byte += packet.byte;
            current_packet->rx_packets++;
           found = 1;
        }    
    }

    // Pokud je kapacita plná, zvětšuje se
    if (!found) {
    if (args->packets->size == args->packets->capacity) {
        args->packets->capacity *= 2; // Zvětšení kapacity
        packet_info_t* temp = (packet_info_t*)realloc(args->packets->data, args->packets->capacity * sizeof(packet_info_t));

        // Kontrola, zda realloc uspěl
        if (temp == NULL) {
            fprintf(stderr, "Chyba při realokaci paměti pro pakety.\n");
            exit(1); // Nebo jiné zpracování chyby
        }

        args->packets->data = temp; // Aktualizace ukazatele na novou paměť
    }

    // Přidání nového paketu na konec pole
    packet.tx_packets += 1;
    packet.tx_byte = packet.byte;
    args->packets->data[args->packets->size] = packet;
    args->packets->size++; // Zvýšení velikosti po přidání nového paketu
    }
}


// Uvolnění paměti vektoru
void free_packet_vector(packet_vector_t *vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}