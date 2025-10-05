#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	char ip4Txt[] = "142.250.184.174";
	uint8_t ip4Bin[4];
	if (inet_pton(AF_INET, ip4Txt, &ip4Bin) == 1)
	{
		printf("IPv4 en decimal: ");
		for (int i=0; i<3; i++)
			printf("%u - ", ip4Bin[i], ip4Bin[i]);
		printf("%u\n", ip4Bin[3], ip4Bin[3]);
		printf("IPv4 en hexadecimal: ");
		for (int i=0; i<3; i++)
			printf("%x - ", ip4Bin[i], ip4Bin[i]);
		printf("%x\n", ip4Bin[3], ip4Bin[3]);

		inet_ntop(AF_INET, &ip4Bin, ip4Txt, sizeof(ip4Txt));
		printf("Reconversión a texto dende binario: %s\n", ip4Txt);
	}
	else
		return 1;

	char ip6Txt[] = "1080:0:0:0:8:800:200C:417A";
	struct in6_addr ip6Bin;
	
	if (inet_pton(AF_INET6, ip6Txt, &ip6Bin) == 1)
	{
		printf("IPv6 en decimal: ");
		for (int i=0; i<14; i++)
			printf("%u - ", ip6Bin.s6_addr[i], ip6Bin.s6_addr[i]);
		printf("%u\n", ip6Bin.s6_addr[15], ip6Bin.s6_addr[15]);
		printf("IPv6 en hexadecimal: ");
		for (int i=0; i<14; i++)
			printf("%x - ", ip6Bin.s6_addr[i], ip6Bin.s6_addr[i]);
		printf("%x\n", ip6Bin.s6_addr[15], ip6Bin.s6_addr[15]);

		inet_ntop(AF_INET6, &ip6Bin, ip6Txt, sizeof(ip6Txt));
		printf("Reconversión a texto dende binario: %s\n", ip6Txt);
	}
	else
		return 1;


	return 0;	
}