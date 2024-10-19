/**
 * @file packet_handler,h
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include <pcap/pcap.h>
#include <pthread.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/ip.h>   
#include <netinet/tcp.h>   
#include <netinet/udp.h>
#include <unistd.h>
#include <netinet/ip6.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "vector.h"

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

/**
 * @brief A packet capture thread
 */
void* packet_capture_thread(void* arg);

/**
 * @brief Features for handling IPv4 packets
 */
void process_ipv4_packet(const unsigned char *packet, packet_info_t *new_packet);

/**
 * @brief Features for handling IPv6 packets
 */
void process_ipv6_packet(const unsigned char *packet, packet_info_t *new_packet);


/**
 * @brief Callback function for captured packets
 */
void packet_handler(unsigned char *user, const struct pcap_pkthdr *header __attribute__((unused)), const unsigned char *packet);
#endif // PACKET_HANDLER_H