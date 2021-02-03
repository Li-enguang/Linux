#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>

#define PACK_LEN 72

void make_icmp_packet(struct icmp* picmp, int len, int n);
u_short checksum(u_short *data, int len);
void tvsub(struct timeval *out, struct timeval *in);

int main( void ) {
	char buf[32] = "192.168.204";
	printf("请输入想要扫描的网段(X.X.X):");
	scanf("%s", buf);
	int i, j;
	char ip_addr[32];

	int sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); // 创建原始套接字, 专门收发icmp报文
	if ( sfd == -1 ) perror("socket"),exit(1);

	for (i=1; i<255; i++) {
		memset(ip_addr, 0x00, 32);
		sprintf(ip_addr, "%s.%d", buf, i);
		printf("正在扫描:%s\n", ip_addr);
		char send_buf[PACK_LEN];
		struct sockaddr_in addr;
		memset(&addr, 0x00, sizeof(addr));
		addr.sin_family = AF_INET;	
		inet_aton(ip_addr, &addr.sin_addr);
		for (j=0; j<3; j++) {
			make_icmp_packet((struct icmp*)send_buf, PACK_LEN, j); // 组icmp报文
			if ( sendto(sfd, send_buf, PACK_LEN, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1 ) // 发送
				perror("sendto"),exit(1);
			fd_set rset;
			FD_ZERO(&rset);
			FD_SET(sfd, &rset);
			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 200*1000;
			while ( 1 ) {
				int ready;
				if ( (ready=select(sfd+1, &rset, NULL, NULL, &tv)) <= 0 ) break; // 用select的主要目的是为了超时
				char recv_buf[2048];
				if ( recvfrom(sfd, recv_buf, 2048, 0, NULL, NULL) == -1 ) exit(0);
				struct ip *pip = (struct ip*)recv_buf;
				int len = pip->ip_hl << 2; // 获取首部长度
				if ( pip->ip_src.s_addr == addr.sin_addr.s_addr ) { //  来的ip包和我们发送的目标机器一致，才处理
					struct icmp *picmp = (struct icmp*)(recv_buf+len); // 获取到收到的icmp报文
					if ( picmp->icmp_type == ICMP_ECHOREPLY ) { // 收到的是应答包
						printf("\t%s   ", inet_ntoa(pip->ip_src));
						struct timeval curt;
						gettimeofday(&curt, NULL);
						tvsub(&curt, (struct timeval*)(picmp->icmp_data));
						printf("\tttl:%hhu rtt:%d\n", pip->ip_ttl, curt.tv_sec + curt.tv_usec/1000);
					}
					goto lab;
				}
			}
		}
	lab: ; // 分号是一个空语句，目的不让lab报错
	}
}

void make_icmp_packet(struct icmp* picmp, int len, int n) {
	memset(picmp, 0x00, len);
	gettimeofday((struct timeval*)(picmp->icmp_data), NULL);
	picmp->icmp_type = ICMP_ECHO;
	picmp->icmp_code = 0;
	picmp->icmp_cksum     = 0;
	picmp->icmp_id   = getpid();
	picmp->icmp_seq  = n;
	picmp->icmp_cksum = checksum((u_short*)picmp, len);
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

void tvsub(struct timeval *out, struct timeval *in) {
	out->tv_sec -= in->tv_sec;
}

