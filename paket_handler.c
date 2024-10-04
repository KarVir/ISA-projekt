#include "paket_handler.h"

bool is_packet_duplicate(packet_info_t *saved_packets, int packet_count, packet_info_t *new_packet) {
    for (int i = 0; i < packet_count; i++) {
        // Porovnej IP adresy, porty a protokol
        if (strcmp(saved_packets[i].src_ip, new_packet->src_ip) == 0 &&
            strcmp(saved_packets[i].dst_ip, new_packet->dst_ip) == 0 &&
            saved_packets[i].src_port == new_packet->src_port &&
            saved_packets[i].dst_port == new_packet->dst_port &&
            strcmp(saved_packets[i].protocol, new_packet->protocol) == 0) {
            // Pokud se shodují, aktualizuj přijaté a odeslané byty
            saved_packets[i].rx_bytes += new_packet->rx_bytes;
            saved_packets[i].tx_bytes += new_packet->tx_bytes;
            saved_packets[i].count++;
            return true;  // Paket je duplicitní
        }
    }
    return false;  // Paket je nový
}
/**
 * Zpracování IPv4 paketu
 */
int packet_count = 0;
void process_ipv4_packet(const unsigned char *packet, packet_info_t *new_packet) {
    struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));

    // Převod IP adres na čitelné řetězce
    inet_ntop(AF_INET, &ip_header->saddr, new_packet->src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip_header->daddr, new_packet->dst_ip, INET_ADDRSTRLEN);

    // Získání portů podle protokolu
    if (ip_header->protocol == IPPROTO_TCP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4)));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2)));
        strncpy(new_packet->protocol, "TCP", sizeof(new_packet->protocol));
    } else if (ip_header->protocol == IPPROTO_UDP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4)));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + ip_header->ihl * 4 + 2)));
        strncpy(new_packet->protocol, "UDP", sizeof(new_packet->protocol));
    } else if (ip_header->protocol == IPPROTO_ICMP) {
        strncpy(new_packet->protocol, "ICMP", sizeof(new_packet->protocol));
        new_packet->src_port = 0;
        new_packet->dst_port = 0;
    }

    new_packet->rx_bytes = ntohs(ip_header->tot_len);
    new_packet->tx_bytes = ntohs(ip_header->tot_len);
}

/**
 * Zpracování IPv6 paketu
 */
void process_ipv6_packet(const unsigned char *packet, packet_info_t *new_packet) {
    struct ip6_hdr *ip6_header = (struct ip6_hdr *)(packet + sizeof(struct ether_header));

    inet_ntop(AF_INET6, &ip6_header->ip6_src, new_packet->src_ip, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &ip6_header->ip6_dst, new_packet->dst_ip, INET6_ADDRSTRLEN);

    if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_TCP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr))));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + 2)));
        strncpy(new_packet->protocol, "TCP", sizeof(new_packet->protocol));
    } else if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_UDP) {
        new_packet->src_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr))));
        new_packet->dst_port = ntohs(*((uint16_t *)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + 2)));
        strncpy(new_packet->protocol, "UDP", sizeof(new_packet->protocol));
    } else if (ip6_header->ip6_ctlun.ip6_un1.ip6_un1_nxt == IPPROTO_ICMPV6) {
        strncpy(new_packet->protocol, "ICMPv6", sizeof(new_packet->protocol));
        new_packet->src_port = 0;
        new_packet->dst_port = 0;
    }

    new_packet->rx_bytes = ntohs(ip6_header->ip6_ctlun.ip6_un1.ip6_un1_plen);
}

/**
 * Funkce pro zpracování paketu
 */
void packet_handler(unsigned char *user, const struct pcap_pkthdr *header __attribute__((unused)), const unsigned char *packet) {
    packet_info_t new_packet;
    packet_info_t *packets = (packet_info_t *)user;

    const struct ether_header *ethernet = (struct ether_header *)packet;

    if (packet_count < 10000) {
        switch (ntohs(ethernet->ether_type)) {
            case ETHERTYPE_IP:
                process_ipv4_packet(packet, &new_packet);
                break;
            case ETHERTYPE_IPV6:
                process_ipv6_packet(packet, &new_packet);
                break;
            default:
                printf("Unknown Ethernet Type\n");
                return;
        }

        if (!is_packet_duplicate(packets, packet_count, &new_packet)) {
            packets[packet_count] = new_packet;
            packets[packet_count].count++;
        }
        
       packet_count++;
    }
}
