#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "piranha-net.h"

unsigned int MakeAddress32(
    unsigned char a,
    unsigned char b,
    unsigned char c,
    unsigned char d)
{
    unsigned char bytes[sizeof(unsigned int)];
    bytes[0] = a;
    bytes[1] = b;
    bytes[2] = c;
    bytes[3] = d;

    unsigned int result;
    memcpy(&result, bytes, sizeof(unsigned int));
}

int main(int argc, char** argv)
{
    int startupResult = jawboneStartNetworking();
    if (startupResult)
    {
        printf("Failed to start networking: %d\n", startupResult);
        return 1;
    }

    unsigned int address = 0;
    unsigned short port = 0;
    int64_t socket;
    int socketError;
    int bindError;
    jawboneCreateAndBindUdpV4Socket(
        address,
        port,
        &socket,
        &socketError,
        &bindError);
    
    if (socketError)
    {
        printf("Socket error: %d\n", socketError);
    }
    else if (bindError)
    {
        printf("Bind error: %d\n", bindError);
    }
    else
    {
        printf("Bound to socket %lld\n", socket);

        if (jawboneGetV4SocketName(&socket, &address, &port))
        {
            printf("Failed to obtain address and port.\n");
        }
        else
        {
            printf("Address %u port %u\n", ntohl(address), (unsigned int)ntohs(port));
        }
        jawboneCloseSocket(&socket);
    }

    jawboneStopNetworking();

    printf("Tests complete.\n");
    return 0;
}
