#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main( void ) {
	struct ifreq  ir[5];
	struct ifconf conf; // 装所有设备的清单
	conf.ifc_len = sizeof(ir); // 缓存的大小
	conf.ifc_req = ir;

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	ioctl(sfd, SIOCGIFCONF, &conf);
	int cnt = conf.ifc_len / sizeof(struct ifreq);
	printf("cnt = %d\n", cnt);
	int i;
	for ( i=0; i<cnt; i++) {
		printf("%s\n", ir[i].ifr_name);
		struct ifreq iq;
		memcpy(iq.ifr_name, ir[i].ifr_name, sizeof(ir[i].ifr_name));
		ioctl(sfd, SIOCGIFDSTADDR, &iq);
		struct sockaddr_in *addr = (struct sockaddr_in*)&iq.ifr_dstaddr;
		printf("\tip : %s\t", inet_ntoa(addr->sin_addr));
		
		ioctl(sfd, SIOCGIFNETMASK, &iq); // 获取子网掩码
		addr = (struct sockaddr_in*)&iq.ifr_netmask;
		printf("\tnetmask : %s\t", inet_ntoa(addr->sin_addr));

		int ret = ioctl(sfd, SIOCGIFBRDADDR, &iq); // 获取广播地址
		addr = (struct sockaddr_in*)&iq.ifr_netmask;
		printf("\tbrdcast: %s\n", inet_ntoa(addr->sin_addr));

		ioctl(sfd, 0x8921, &iq); //  获取MTU
		printf("\tmtu:%d\n", iq.ifr_mtu);
	
		ioctl(sfd, SIOCGIFHWADDR, &iq); // 获取MAC地址
		char buf[6];
		memcpy(buf, iq.ifr_hwaddr.sa_data, 6);
		printf("\tHDaddr : %02X:%02X:%02X:%02X:%02X:%02X\n\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
	}
	printf("按任意键按继续...\n");
	getchar();
}

