#ifndef _manager_h_
#define _manager_h_
#include <vector>

#ifdef __WIN
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#else           

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#endif

typedef struct _connInfo
{
    int                   connfd;
    int                   player;
    int                   avatar;
    std::vector<char>     cards;
    struct   sockaddr_in  cliAddr;
} connInfoT, *pconnInfoT;

void manage();


#endif
