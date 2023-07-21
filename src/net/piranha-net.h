#ifndef PIRANHA_NET_H
#define PIRANHA_NET_H

#ifdef JawboneWindows
#define JawboneExport __declspec(dllexport)
#else
#define JawboneExport
#endif

#define JawboneFlagReuse 1
#define JawboneFlagBind 2
#define JawboneFlagIpV6Only 4

JawboneExport int jawboneGetVersion();
JawboneExport int jawboneStartNetworking();
JawboneExport void jawboneStopNetworking();
JawboneExport void jawboneCreateAndBindUdpV4Socket(
    unsigned int address,
    unsigned short port,
    int flags,
    void *outSocket,
    int *outSocketError,
    int *outBindError);
JawboneExport void jawboneCreateAndBindUdpV6Socket(
    const void *inAddress,
    unsigned short port,
    int flags,
    void *outSocket,
    int *outSocketError,
    int *outSetSocketOptionError,
    int *outBindError);
JawboneExport int jawboneGetV4SocketName(
    const void *inSocket,
    unsigned int *outAddress,
    unsigned short *outPort);
JawboneExport int jawboneGetV6SocketName(
    const void *inSocket,
    void *outAddress,
    unsigned short* outPort);
JawboneExport int jawboneSendToV4(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    unsigned int address,
    unsigned short port,
    int *outErrorCode);
JawboneExport int jawboneSendToV6(
    const void *inSocket,
    const void *inBuffer,
    int bufferLength,
    const void *inAddress,
    unsigned short port,
    int *outErrorCode);
JawboneExport int jawboneReceiveFromV4(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    unsigned int *outAddress,
    unsigned short *outPort,
    int *outErrorCode,
    int milliseconds);
JawboneExport int jawboneReceiveFromV6(
    const void *inSocket,
    void *outBuffer,
    int bufferLength,
    void *outAddress,
    unsigned short *outPort,
    int *outErrorCode,
    int milliseconds);
JawboneExport int jawboneShutdownSocket(const void *targetSocket);
JawboneExport int jawboneCloseSocket(const void *closingSocket);
JawboneExport int jawboneGetAddressInfo(
    const char *node,
    const char *service,
    void *outV4Results,
    int sizeV4,
    int capacityV4,
    int *outCountV4,
    void *outV6Results,
    int sizeV6,
    int capacityV6,
    int *outCountV6);

#endif
