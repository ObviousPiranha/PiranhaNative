#ifndef PIRANHA_NET_H
#define PIRANHA_NET_H

int jawboneStartNetworking();
void jawboneStopNetworking();
void jawboneCreateAndBindUdpV4Socket(
    unsigned int address,
    unsigned short port,
    void *outSocket,
    int *outSocketError,
    int *outBindError);
void jawboneCreateAndBindUdpV6Socket(
    const void *inAddress,
    unsigned short port,
    void *outSocket,
    int *outSocketError,
    int *outBindError);
int jawboneGetV4SocketName(
    const void *inSocket,
    unsigned int *outAddress,
    unsigned short *outPort);
int jawboneGetV6SocketName(
    const void *inSocket,
    void *outAddress,
    unsigned short* outPort);
int jawboneSendToV4(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    unsigned int address,
    unsigned short port,
    int *outErrorCode);
int jawboneSendToV6(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    const void *inAddress,
    unsigned short port,
    int *outErrorCode);
int jawboneReceiveFromV4(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    unsigned int *outAddress,
    unsigned short *outPort,
    int *outErrorCode,
    int milliseconds);
int jawboneReceiveFromV6(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    void *outAddress,
    unsigned short *outPort,
    int *outErrorCode,
    int milliseconds);
int jawboneShutdownSocket(const void *targetSocket);
int jawboneCloseSocket(const void *closingSocket);
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
    int* outCountV6);

#endif
