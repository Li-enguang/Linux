#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>

void print_mac(struct ethhdr *p);
void parse_ip(char *buf);
void parse_arp(char *buf);

int main( void ) {
	int sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	char buf[2000];
	while ( 1 ) {
		memset(buf, 0x00, sizeof(buf));
		int r = read(sfd, buf, 2000);
		if ( r <= 0 ) break;
		struct ethhdr *peth = (struct ethhdr*)buf;
		print_mac(peth);
		printf("Type:%#x\n", ntohs(peth->h_proto));

		if ( ntohs(peth->h_proto) == ETH_P_IP ) {
			parse_ip(buf+sizeof(struct ethhdr));
		} else if ( ntohs(peth->h_proto) == ETH_P_ARP ) {
			parse_arp(buf+sizeof(struct ethhdr));
		}
	}
}

void print_mac(struct ethhdr *p) {
	printf("%02x:%02x:%02x:%02x:%02x:%02x", p->h_source[0], p->h_source[1], p->h_source[2],
							p->h_source[3], p->h_source[4], p->h_source[5]);
	printf(" <==> : %02x:%02x:%02x:%02x:%02x:%02x ", p->h_dest[0], p->h_dest[1], p->h_dest[2],
							p->h_dest[3], p->h_dest[4], p->h_dest[5]);
}

void print_udp(char *buf) {
	struct udphdr *pt = (struct udphdr*)buf;
	printf("%hu : %hu", ntohs(pt->source), ntohs(pt->dest));
	printf("\n\n");
}

void print_icmp(char *buf) {
	printf("icmp\n");
}

void print_tcp(char *buf) {
	struct tcphdr *pt = (struct tcphdr*)buf;
	printf("%hu : %hu  seq : %u", ntohs(pt->source), ntohs(pt->dest), ntohl(pt->seq));
	
	if ( pt->ack )
		printf(" ack_seq: %u", ntohl(pt->ack_seq));

	if ( pt->fin ) printf(" fin");
	if ( pt->syn ) printf(" syn");
	if ( pt->ack ) printf(" ack");
	
	printf("\n\n");
}

void parse_ip(char *buf) {
	struct iphdr *phdr = (struct iphdr*)buf;
	struct in_addr ad;
	ad.s_addr = phdr->saddr;
	printf("\t%s <==> ", inet_ntoa(ad));
	ad.s_addr = phdr->daddr;
	printf("%s, procotol: %hhd, ttl:%hhu, tot_len:%hu", inet_ntoa(ad), phdr->protocol, phdr->ttl, phdr->tot_len);
	if ( phdr->protocol == IPPROTO_TCP ) {
		printf("\n\t\t");
		print_tcp(buf+sizeof(struct iphdr));
	} else if ( phdr->protocol == IPPROTO_UDP) {
		printf("\n\t\t");
		print_udp(buf+sizeof(struct iphdr));
	} else if ( phdr->protocol == IPPROTO_ICMP ) {
		printf("\n\t\t");
		print_icmp(buf+sizeof(struct iphdr));
	}
}

void parse_arp(char *buf) {
	printf("arp packet\n");
}

