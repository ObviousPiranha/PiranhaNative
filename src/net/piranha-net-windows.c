#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// #include <iphlpapi.h>
#include "piranha-net.h"

static WSADATA theWsaData;

int jawboneStartNetworking()
{
    int result = WSAStartup(MAKEWORD(2, 2), &theWsaData);
    return result;
}

void jawboneStopNetworking()
{
    WSACleanup();
}

void jawboneCreateAndBindUdpV4Socket(
    unsigned int address,
    unsigned short port,
    void *outSocket,
    int *outSocketError,
    int *outBindError)
{
    struct sockaddr_in sa;
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int socketError = s == INVALID_SOCKET ? WSAGetLastError() : 0;

    *outSocketError = socketError;

    if (socketError)
    {
        *outBindError = 0;
        memcpy(outSocket, &s, sizeof s);
        return;
    }

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = port;
    sa.sin_addr.s_addr = address;

    int bindError = bind(s, (struct sockaddr *)&sa, sizeof sa);
    *outBindError = bindError;

    if (bindError)
    {
        closesocket(s);
        s = INVALID_SOCKET;
    }

    memcpy(outSocket, &s, sizeof s);
}

int jawboneGetV4SocketName(
    const void *inSocket,
    unsigned int* outAddress,
    unsigned short* outPort)
{
    struct sockaddr_in sa;
    SOCKET s;
    int nameLen = sizeof sa;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);

    int result = getsockname(s, (struct sockaddr *)&sa, &nameLen);

    if (result)
    {
        *outAddress = 0;
        *outPort = 0;
    }
    else
    {
        *outAddress = sa.sin_addr.s_addr;
        *outPort = sa.sin_port;
    }

    return result;
}

int jawboneSendToV4(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    unsigned int address,
    unsigned short port)
{
    struct sockaddr_in sa;
    SOCKET s;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = port;
    sa.sin_addr.s_addr = address;

    return sendto(
        s,
        (const char *)inBuffer,
        bufferLength,
        0,
        (const struct sockaddr *)&sa,
        sizeof sa);
}

int jawboneReceiveFromV4(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    unsigned int *outAddress,
    unsigned short *outPort)
{
    struct sockaddr_in sa;
    int addressLength = sizeof sa;
    SOCKET s;
    memcpy(&s, inSocket, sizeof s);

    int result = recvfrom(
        s,
        (char *)outBuffer,
        bufferLength,
        0,
        (struct sockaddr *)&sa,
        &addressLength);
    
    *outAddress = sa.sin_addr.s_addr;
    *outPort = sa.sin_port;
    
    return result;
}

void jawboneCloseSocket(void *closingSocket)
{
    SOCKET s;
    memcpy(&s, closingSocket, sizeof s);
    closesocket(s);
}

int jawboneGetAddressInfo(
    const char *node,
    const char *service,
    void* outV4Results,
    int sizeV4,
    int capacityV4,
    int* outCountV4,
    void* outV6Results,
    int sizeV6,
    int capacityV6,
    int* outCountV6)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    if (0 < capacityV4 && 0 < capacityV6)
    {
        hints.ai_family = AF_UNSPEC;
    }
    else if (0 < capacityV4)
    {
        hints.ai_family = AF_INET;
    }
    else if (0 < capacityV6)
    {
        hints.ai_family = AF_INET6;
    }
    
    struct addrinfo *result;
    int getResult = getaddrinfo(node, service, &hints, &result);
    if (getResult != 0) return getResult;
    if (!result) return -1;

    int countV4 = 0;
    int countV6 = 0;
    char *currentV4 = (char *)outV4Results;
    char *currentV6 = (char *)outV6Results;
    
    for (struct addrinfo *ai = result; ai; ai = ai->ai_next)
    {
        if (ai->ai_family == AF_INET)
        {
            if (countV4 < capacityV4)
            {
                struct sockaddr_in *addr = (struct sockaddr_in *)ai->ai_addr;
                memcpy(currentV4, &addr->sin_addr, 4);
                memcpy(currentV4 + 4, &addr->sin_port, 2);
                currentV4 += sizeV4;
                ++countV4;
            }
        }
        else if (ai->ai_family == AF_INET6)
        {
            if (countV6 < capacityV6)
            {
                struct sockaddr_in6 *addr = (struct sockaddr_in6 *)ai->ai_addr;
                memcpy(currentV6, &addr->sin6_addr, 16);
                memcpy(currentV6 + 16, &addr->sin6_port, 2);
                currentV6 += sizeV6;
                ++countV6;
            }
        }
    }

    *outCountV4 = countV4;
    *outCountV6 = countV6;

    freeaddrinfo(result);

    return 0;
}
