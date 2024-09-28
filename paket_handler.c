#include "paket_handler.h"
int connection_count = 0;
int zadano = 0;
int packet_count = 0;

void packet_handler(unsigned char *user, const struct pcap_pkthdr *header __attribute__((unused)) , const unsigned char *packet) {
    const struct ether_header *ethernet = (struct ether_header *)packet;
    packet_info_t *packets= (packet_info_t *)user;
    if (packet_count < MAX_PACKETS) {
    switch (ntohs(ethernet->ether_type)) {
        case ETHERTYPE_IP: {
            // IPv4
           // uint16_t src_port;
//uint16_t dst_port; 
            //char protokol[5];
            struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));
            //char src_ip[INET_ADDRSTRLEN];
            //char dst_ip[INET_ADDRSTRLEN];
            
            // Convert source and destination IP addresses to string format
            inet_ntop(AF_INET, &ip_header->saddr, packets[packet_count].src_ip, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &ip_header->daddr, packets[packet_count].dst_ip, INET_ADDRSTRLEN);
            
            // Print source and destination IP addresses
            
            // Print source and destination port if the protocol is TCP or UDP
            if (ip_header->protocol == IPPROTO_TCP) {
                packets[packet_count].src_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4));
                packets[packet_count].dst_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2));
                strncpy(packets[packet_count].protocol, "TCP", sizeof(packets[packet_count].protocol));;

            }else if (ip_header->protocol == IPPROTO_UDP) {
                packets[packet_count].src_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4));
                packets[packet_count].dst_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2));
                strncpy(packets[packet_count].protocol, "UDP", sizeof(packets[packet_count].protocol));
            }else if (ip_header->protocol == IPPROTO_ICMP) {
                /* TODO nic nevypisovat*/
                packets[packet_count].src_port= 0;
                packets[packet_count].dst_port= 0;
                strncpy(packets[packet_count].protocol, "ICMP", sizeof(packets[packet_count].protocol));
            }else if (ip_header->protocol == IPPROTO_IGMP)
            {
            /* TODO nic nevypisovat*/
                packets[packet_count].src_port= 0;
                packets[packet_count].dst_port= 0;
                strncpy(packets[packet_count].protocol, "IGMP", sizeof(packets[packet_count].protocol));
            }
            
            packets[packet_count].rx_bytes = header->len;
            packets[packet_count].tx_bytes = header->len;
         //   printf("%s: %d\t\t%s: %d\t%s\t%u\t%u\n",  packets[0].src_ip, ntohs(packets[0].src_port), packets[0].dst_ip, ntohs(packets[0].dst_port), packets[0].protocol, rx, tx);
            break;
        }
        case ETHERTYPE_IPV6: {
            uint16_t src_port;
            uint16_t dst_port; 
            char protokol[5];
            struct ip6_hdr *ip6_header = (struct ip6_hdr *)(packet + sizeof(struct ethhdr));
            char src_ip[INET6_ADDRSTRLEN];
            char dst_ip[INET6_ADDRSTRLEN];
        
        // Convert source and destination IP addresses to string format
        inet_ntop(AF_INET6, &(ip6_header->ip6_src), src_ip, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &(ip6_header->ip6_dst), dst_ip, INET6_ADDRSTRLEN);
        
        // Print source and destination port if the next header is TCP or UDP
        if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_TCP || ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_UDP) {
             src_port = *((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr)));
             dst_port = *((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + 2));
         
        }
            printf("IPv6 %s: %d\t\t%s: %d\t%s\n", src_ip, ntohs(src_port), dst_ip, ntohs(dst_port), protokol);
            break;
        }default:
            printf("Unknown Ethernet Type\n");
            break;
    }
            packet_count++;
    }
}