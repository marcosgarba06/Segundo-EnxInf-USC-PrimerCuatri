#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{

    char ip_txt[]= "193.110.128.200";
    struct in_addr binario4;
    uint8_t ip1[4];
    if(inet_pton(AF_INET, ip_txt, &binario4)!=1)
    {
        printf("Error\n");
        return -1;
    }

    printf("Dirección IP: %X\n", binario4.s_addr);

    printf("Formato binario de %s:\n", ip_txt);

    uint8_t *bytes = (uint8_t*)&binario4.s_addr;
    for (int i = 0; i < 4; i++) {
        printf("%u (0x%X)\n",bytes[i], bytes[i]);
    }

    char ip_back[INET_ADDRSTRLEN]; 
    if (inet_ntop(AF_INET, &binario4, ip_back, INET_ADDRSTRLEN) == NULL) {
        printf("Error\n");
        return -1;
    }
    return 0;

}