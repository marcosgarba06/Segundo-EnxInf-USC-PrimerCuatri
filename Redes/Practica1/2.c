#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    u_int16_t porto = 3979;
    printf("Porto usado: 0x%x\nPorto convertido: 0x%x\nPorto reconvertido: 0x%x\n", porto, htons(porto), ntohs(htons(porto)));

    char ip4Txt[] = "142.250.184.174";
    char ip4TxtConv[16], ip4TxtReconv[16];
	in_addr_t ip4Bin, ip4BinConv, ip4BinReconv;
    if (inet_pton(AF_INET, ip4Txt, &ip4Bin) == 1)
    {
        ip4BinConv = htonl(ip4Bin);
        ip4BinReconv = ntohl(ip4BinConv);

        inet_ntop(AF_INET, &ip4Bin, ip4Txt, sizeof(ip4Txt));
        inet_ntop(AF_INET, &ip4BinConv, ip4TxtConv, sizeof(ip4Txt));
        inet_ntop(AF_INET, &ip4BinReconv, ip4TxtReconv, sizeof(ip4Txt));

        printf("\nIPv4 usada: 0x%x (%s)\nIPv4 convertida: 0x%x (%s)\nIPv4 reconvertida: 0x%x (%s)\n", ip4Bin, ip4Txt, ip4BinConv, ip4TxtConv, ip4BinReconv, ip4TxtReconv);
    }
    else
        return 1;

    return 0;
}