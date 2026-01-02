#pragma once

// ===== PLATFORM IDENTIFIERS =====
#define PLATFORM_UNKNOWN 0
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 2

// ===== PLATFORM DETECTION =====
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(__linux__) || defined(LINUX)
    #define PLATFORM PLATFORM_LINUX
#else
    #define PLATFORM PLATFORM_UNKNOWN
    #error "Unsupported platform !!!"
#endif



// ===== HEADERS =====
#if PLATFORM == PLATFORM_WINDOWS
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #include <windows.h>
#endif

#if PLATFORM == PLATFORM_LINUX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <poll.h>
    #include <pthread.h>
#endif

// ===== SOCKETS =====
#if PLATFORM == PLATFORM_WINDOWS
    #define MSG_NOSIGNAL_PLATFORM 0
    #define SOCKET_ERROR_PLATFORM SOCKET_ERROR
    #define INVALID_SOCKET_PLATFORM INVALID_SOCKET
    using SocketResult = int;
    using SocketLen = int;
    // SOCKET is already defined by <WinSock2.h>
#endif

#if PLATFORM == PLATFORM_LINUX
    #define MSG_NOSIGNAL_PLATFORM MSG_NOSIGNAL
    #define SOCKET_ERROR_PLATFORM -1
    #define INVALID_SOCKET_PLATFORM -1
    using SocketResult = ssize_t;
    using SocketLen = socklen_t;
    using SOCKET = int;
#endif

// ===== POLL =====
#if PLATFORM == PLATFORM_WINDOWS
    using PollFd = WSAPOLLFD;
    #define poll WSAPoll
    typedef int nfds_t;
#endif

#if PLATFORM == PLATFORM_LINUX
    using PollFd = struct pollfd;
#endif
