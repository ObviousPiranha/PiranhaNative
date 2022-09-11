#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "piranha-net.h"

static WSADATA theWsaData;

// https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsagetlasterror

int jawboneStartNetworking()
{
    // https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
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
    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int socketError = s == INVALID_SOCKET ? WSAGetLastError() : 0;

    *outSocketError = socketError;

    if (socketError)
    {
        *outBindError = 0;
        memcpy(outSocket, &s, sizeof s);
        return;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = port;
    sa.sin_addr.s_addr = address;

    int bindError = bind(s, (struct sockaddr *)&sa, sizeof sa);
    *outBindError = bindError == -1 ? WSAGetLastError() : 0;

    if (bindError)
    {
        closesocket(s);
        s = INVALID_SOCKET;
    }

    memcpy(outSocket, &s, sizeof s);
}

void jawboneCreateAndBindUdpV6Socket(
    const void *inAddress,
    unsigned short port,
    void *outSocket,
    int *outSocketError,
    int *outBindError)
{
    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
    SOCKET s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    int socketError = s == INVALID_SOCKET ? WSAGetLastError() : 0;

    *outSocketError = socketError;

    if (socketError)
    {
        *outBindError = 0;
        memcpy(outSocket, &s, sizeof s);
        return;
    }

    struct sockaddr_in6 sa;
    memset(&sa, 0, sizeof sa);
    memcpy(&sa.sin6_addr, inAddress, 16);
    sa.sin6_family = AF_INET6;
    sa.sin6_port = port;

    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind
    int bindError = bind(s, (struct sockaddr *)&sa, sizeof sa);
    *outBindError = bindError == -1 ? WSAGetLastError() : 0;

    if (bindError)
    {
        // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-closesocket
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

    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-getsockname
    int result = getsockname(s, (struct sockaddr *)&sa, &nameLen);

    if (result == SOCKET_ERROR)
    {
        *outAddress = 0;
        *outPort = 0;
        return WSAGetLastError();
    }
    else
    {
        *outAddress = sa.sin_addr.s_addr;
        *outPort = sa.sin_port;
        return 0;
    }
}

int jawboneGetV6SocketName(
    const void *inSocket,
    void *outAddress,
    unsigned short* outPort)
{
    struct sockaddr_in6 sa;
    SOCKET s;
    int nameLen = sizeof sa;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);

    int result = getsockname(s, (struct sockaddr *)&sa, &nameLen);

    if (result == SOCKET_ERROR)
    {
        memset(outAddress, 0, 16);
        *outPort = 0;
        return WSAGetLastError();
    }
    else
    {
        memcpy(outAddress, &sa.sin6_addr, 16);
        *outPort = sa.sin6_port;
        return 0;
    }
}

int jawboneSendToV4(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    unsigned int address,
    unsigned short port,
    int *outErrorCode)
{
    struct sockaddr_in sa;
    SOCKET s;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = port;
    sa.sin_addr.s_addr = address;

    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-sendto
    int sendResult = sendto(
        s,
        (const char *)inBuffer,
        bufferLength,
        0,
        (const struct sockaddr *)&sa,
        sizeof sa);
    
    *outErrorCode = sendResult == -1 ? WSAGetLastError() : 0;
    return sendResult;
}

int jawboneSendToV6(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    const void *inAddress,
    unsigned short port,
    int *outErrorCode)
{
    struct sockaddr_in6 sa;
    SOCKET s;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);
    sa.sin6_family = AF_INET6;
    sa.sin6_port = port;
    memcpy(&sa.sin6_addr, inAddress, 16);

    // // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-sendto
    int sendResult = sendto(
        s,
        (const char *)inBuffer,
        bufferLength,
        0,
        (const struct sockaddr *)&sa,
        sizeof sa);
    
    *outErrorCode = sendResult == -1 ? WSAGetLastError() : 0;
    return sendResult;
}

int jawboneReceiveFromV4(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    unsigned int *outAddress,
    unsigned short *outPort,
    int *outErrorCode,
    int milliseconds)
{
    struct sockaddr_in sa;
    socklen_t addressLength = sizeof sa;
    SOCKET s;
    memcpy(&s, inSocket, sizeof s);

    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/ns-winsock2-wsapollfd
    WSAPOLLFD pfd;
    memset(&pfd, 0, sizeof pfd);
    pfd.fd = s;
    pfd.events = POLLRDNORM;
    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll
    int pollResult = WSAPoll(&pfd, 1, milliseconds);

    if (0 < pollResult)
    {
        if (pfd.revents & POLLRDNORM)
        {
            int result = recvfrom(
                s,
                (char *)outBuffer,
                bufferLength,
                0,
                (struct sockaddr *)&sa,
                &addressLength);
            
            *outAddress = sa.sin_addr.s_addr;
            *outPort = sa.sin_port;
            *outErrorCode = result == -1 ? WSAGetLastError() : 0;
            
            return result;
        }
        else
        {
            *outAddress = 0;
            *outPort = 0;
            *outErrorCode = 0;
            return 0;
        }
    }
    else
    {
        *outAddress = 0;
        *outPort = 0;

        if (pollResult == -1)
        {
            *outErrorCode = WSAGetLastError();
            return -1;
        }
        else
        {
            *outErrorCode = 0;
            return 0;
        }
    }
}

int jawboneReceiveFromV6(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    void *outAddress,
    unsigned short *outPort,
    int *outErrorCode,
    int milliseconds)
{
    struct sockaddr_in6 sa;
    socklen_t addressLength = sizeof sa;
    SOCKET s;
    memcpy(&s, inSocket, sizeof s);

    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/ns-winsock2-wsapollfd
    WSAPOLLFD pfd;
    memset(&pfd, 0, sizeof pfd);
    pfd.fd = s;
    pfd.events = POLLRDNORM;
    // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsapoll
    int pollResult = WSAPoll(&pfd, 1, milliseconds);

    if (0 < pollResult)
    {
        if (pfd.revents & POLLIN)
        {
            int result = recvfrom(
                s,
                (char *)outBuffer,
                bufferLength,
                0,
                (struct sockaddr *)&sa,
                &addressLength);
            
            memcpy(outAddress, &sa.sin6_addr, 16);
            *outPort = sa.sin6_port;
            *outErrorCode = result == -1 ? WSAGetLastError() : 0;
            
            return result;
        }
        else
        {
            memset(outAddress, 0, 16);
            *outPort = 0;
            *outErrorCode = 0;
            return 0;
        }
    }
    else
    {
        memset(outAddress, 0, 16);
        *outPort = 0;

        if (pollResult == -1)
        {
            *outErrorCode = WSAGetLastError();
            return -1;
        }
        else
        {
            *outErrorCode = 0;
            return 0;
        }
    }
}

int jawboneShutdownSocket(const void *targetSocket)
{
    SOCKET s;
    memcpy(&s, targetSocket, sizeof s);
    return shutdown(s, SD_BOTH) == -1 ? WSAGetLastError() : 0;
}

int jawboneCloseSocket(const void *closingSocket)
{
    SOCKET s;
    memcpy(&s, closingSocket, sizeof s);
    return closesocket(s) == -1 ? WSAGetLastError() : 0;
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
    // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
    int getResult = getaddrinfo(node, service, &hints, &result);
    if (getResult != 0) return getResult; WSATRY_AGAIN;
    if (!result) return 0;

    int countV4 = 0;
    int countV6 = 0;
    char *currentV4 = (char *)outV4Results;
    char *currentV6 = (char *)outV6Results;
    
    // https://docs.microsoft.com/en-us/windows/win32/api/ws2def/ns-ws2def-addrinfoa
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
