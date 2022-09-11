#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include "piranha-net.h"

#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close

int jawboneStartNetworking()
{
    return 0;
}

void jawboneStopNetworking()
{
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
    int socketError = s == INVALID_SOCKET ? errno : 0;

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

    // // https://man7.org/linux/man-pages/man2/bind.2.html
    int bindError = bind(s, (struct sockaddr *)&sa, sizeof sa);
    *outBindError = bindError == -1 ? errno : 0;

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
    SOCKET s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    int socketError = s == INVALID_SOCKET ? errno : 0;

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

    // https://man7.org/linux/man-pages/man2/bind.2.html
    int bindError = bind(s, (struct sockaddr *)&sa, sizeof sa);
    *outBindError = bindError == -1 ? errno : 0;

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
    socklen_t nameLen = sizeof sa;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);

    // https://man7.org/linux/man-pages/man2/getsockname.2.html
    int result = getsockname(s, (struct sockaddr *)&sa, &nameLen);

    if (result)
    {
        *outAddress = 0;
        *outPort = 0;
        return errno;
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
    socklen_t nameLen = sizeof sa;
    memcpy(&s, inSocket, sizeof s);
    memset(&sa, 0, sizeof sa);

    // https://man7.org/linux/man-pages/man2/getsockname.2.html
    int result = getsockname(s, (struct sockaddr *)&sa, &nameLen);

    if (result)
    {
        memset(outAddress, 0, 16);
        *outPort = 0;
        return errno;
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

    // https://man7.org/linux/man-pages/man2/sendto.2.html
    int sendResult = sendto(
        s,
        (const char *)inBuffer,
        bufferLength,
        0,
        (const struct sockaddr *)&sa,
        sizeof sa);
    
    *outErrorCode = sendResult == -1 ? errno : 0;
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

    // https://man7.org/linux/man-pages/man2/sendto.2.html
    int sendResult = sendto(
        s,
        (const char *)inBuffer,
        bufferLength,
        0,
        (const struct sockaddr *)&sa,
        sizeof sa);
    
    *outErrorCode = sendResult == -1 ? errno : 0;
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

    struct pollfd pfd;
    memset(&pfd, 0, sizeof pfd);
    pfd.fd = s;
    pfd.events = POLLIN;
    // https://man7.org/linux/man-pages/man2/poll.2.html
    int pollResult = poll(&pfd, 1, milliseconds);

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
            
            *outAddress = sa.sin_addr.s_addr;
            *outPort = sa.sin_port;
            *outErrorCode = result == -1 ? errno : 0;
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
            *outErrorCode = errno;
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

    struct pollfd pfd;
    memset(&pfd, 0, sizeof pfd);
    pfd.fd = s;
    pfd.events = POLLIN;
    // https://man7.org/linux/man-pages/man2/poll.2.html
    int pollResult = poll(&pfd, 1, milliseconds);

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
            *outErrorCode = result == -1 ? errno : 0;
            
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
            *outErrorCode = errno;
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
    return shutdown(s, SHUT_RDWR) == -1 ? errno : 0;
}

int jawboneCloseSocket(const void *closingSocket)
{
    SOCKET s;
    memcpy(&s, closingSocket, sizeof s);
    return closesocket(s) == -1 ? errno : 0;
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
    // https://man7.org/linux/man-pages/man3/gai_strerror.3.html
    int getResult = getaddrinfo(node, service, &hints, &result);
    if (getResult != 0) return getResult;
    if (!result) return 0;

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
