/**
 * @file isa-top.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */
#include <stdio.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <pcap/pcap.h>
#include <pthread.h>
#include <net/ethernet.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <ifaddrs.h>
#include "vector.h"

/**
 * @brief Vlákno pro zachytávání paketů
 * 
 * @param arg 
 * @return void* 
 */
void* packet_capture_thread(void* arg);

/**
 * @brief 
 * 
 * @param user 
 * @param __attribute__ 
 * @param packet 
 */
void packet_handler(unsigned char *user, const struct pcap_pkthdr *header __attribute__((unused)), const unsigned char *packet);

/**
 * @brief 
 * 
 * @param packet 
 * @param new_packet 
 */
void process_ipv4_packet(const unsigned char *packet, packet_info_t *new_packet);

/**
 * @brief 
 * 
 * @param packet 
 * @param new_packet 
 */
void process_ipv6_packet(const unsigned char *packet, packet_info_t *new_packet);