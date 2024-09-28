#include "paket_handler.h"
connection_stat_t connection[MAX_connection];
int connection_count = 0;
int zadano = 0;


void packet_handler(unsigned char *user __attribute__((unused)) , const struct pcap_pkthdr *header __attribute__((unused)) , const unsigned char *packet) {
    const struct ether_header *ethernet = (struct ether_header *)packet;
    switch (ntohs(ethernet->ether_type)) {
        case ETHERTYPE_IP: {
            // IPv4
            uint16_t src_port;
            uint16_t dst_port; 
            char protokol[5];
            struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));
            char src_ip[INET_ADDRSTRLEN];
            char dst_ip[INET_ADDRSTRLEN];
            
            // Convert source and destination IP addresses to string format
            inet_ntop(AF_INET, &ip_header->saddr, src_ip, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &ip_header->daddr, dst_ip, INET_ADDRSTRLEN);
            
            // Print source and destination IP addresses
            
            // Print source and destination port if the protocol is TCP or UDP
            if (ip_header->protocol == IPPROTO_TCP) {
                src_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4));
                dst_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2));
                strncpy(protokol, "TCP", sizeof(protokol));;

            }else if (ip_header->protocol == IPPROTO_UDP) {
                src_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4));
                dst_port = *((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2));
                strncpy(protokol, "UDP", sizeof(protokol));
            }else if (ip_header->protocol == IPPROTO_ICMP) {
                /* TODO nic nevypisovat*/
                src_port= 0;
                dst_port= 0;
                strncpy(protokol, "ICMP", sizeof(protokol));
            }else if (ip_header->protocol == IPPROTO_IGMP)
            {
            /* TODO nic nevypisovat*/
                src_port= 0;
                dst_port= 0;
                strncpy(protokol, "IGMP", sizeof(protokol));
            }
            
            uint16_t rx = header->len;
            uint16_t tx = header->len;
            printf("%s: %d\t\t%s: %d\t%s\t%u\t%u\n", src_ip, ntohs(src_port), dst_ip, ntohs(dst_port), protokol, rx, tx);
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
}