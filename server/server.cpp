// server.cpp:
// to test without a client around: in a command prompt on another system type: nc <serverAddress> 1337

#ifdef __WIN
    #define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#pragma comment(lib, "ws2_32.lib")
//#pragma pack(show)
//typedef struct _tag_hdr
//{
//    short      msgLen;
//    char       chCode;
//    char       szMsg[128];              // more than we really need
//} msgT, *pMsgT;

#else           
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

//typedef struct _tag_hdr
//{
//    short      msgLen;
//    char       chCode;
//    char       szMsg[128];              // more than we really need
//} __attribute__((packed)) msgT, *pMsgT;

#endif


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include "../common/common.h"
#include "manager.h" 

const char defServer[] = {"192.168.1.19"};
const unsigned short defPort = 1337;
const char lpszVersion[] = {"0.0.5"};


bool initWinSock();  // for windows only...need to initialize the socket subsytems
int getLastError(); // windows -- wrapper around WSAGetLastError; linux - just return errno
void showUsage();
void showVersion();


std::vector<pconnInfoT>  g_conns;              // global queue of connections
bool                     g_bForce;             // condition variable to force a game to start
bool                     g_bMgrRun;            // condition variable to control manager thread
std::mutex               g_mutexque;           // mutex to guard access to player queue


int main()
{
  int                 sockfd, connfd;
  unsigned int        len;
  struct sockaddr_in  servAddr, cliAddr;
  int                 nRet;
  
  g_bForce = false;
  g_bMgrRun = true;
  // TODO: process command line arguments...only use defalut if necessary

  std::cout << "Welcome to the Clue-less server." << std::endl;
  showUsage();
  
  // launch our manager thread...
  std::thread manager(manage);                       // manage is the name of the tread function

  

  if (initWinSock())
  {
      if (-1 != (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
      {
          memset((void*)&servAddr, 0, sizeof(struct sockaddr_in));

          servAddr.sin_family = AF_INET;
          servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
          servAddr.sin_port = htons(defPort);



          // build our heatbeat message, will reuse this often...
          msgT   hbMsg;
          memset((void*)hbMsg.szMsg, '\0', 128);
          int cntCh = sprintf(hbMsg.szMsg, "connect to server %s", "192.168.1.19");
          hbMsg.msgLen = (short)(4 + cntCh);           // header length + null terminator...
          hbMsg.chCode = CMD_HRT_BEAT;


          if (0 == (nRet = bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr))))
          {
              if (0 == listen(sockfd, 5))
              {
                  bool bRun = true;
                  int  ret;                   // return value from select
                  len = sizeof(cliAddr);
                  fd_set   rdfs;              // input descriptors to listen on
                  struct timeval tv;
		  
                  while (bRun)
                  {
                      tv.tv_sec = 5;            // set timeout for 5 sec
                      tv.tv_usec = 0;

                      FD_ZERO(&rdfs);
                      FD_SET(0, &rdfs);
                      FD_SET(sockfd, &rdfs);

                      ret = select(sockfd + 1, &rdfs, nullptr, nullptr, &tv);
                      if (-1 == ret)
                      {
                          std::cerr << "select failed" << std::endl;
                          bRun = false;
                      }
                      else if (ret > 0)
                      {
                          if (FD_ISSET(0, &rdfs))             // data on stdin
                          {
                              char chIn = getchar();
                              getchar();                       // eat carriage return still lurking on stdin
                              switch (chIn)
                              {
                              case 'l':
                              {
                                  std::vector<pconnInfoT>::iterator   iter = g_conns.begin();
                                  if (g_conns.size() == 0)
                                  {
                                      std::cout << "connection queue is empty" << std::endl;
                                  }
                                  else
                                  {
                                      int cnt = 1;
                                      while (g_conns.end() != iter)
                                      {
                                          std::cout << "connection: (" << cnt++ << ") IP address " << defServer << std::endl;
                                          ++iter;
                                      }
                                  }
                              }
                              break;

                              case 'g':
                              {
                                  std::cout << "NYI" << std::endl;
                              }
                              break;

                              case 'f':
                              {
                                  g_bForce = true;
                              }
                              break;

                              case 'v':
                              {
                                  showVersion();
                              }
                              break;

                              case 'h':
                              {
                                  showUsage();
                              }
                              break;

                              case 'q':
                              {
                                  bRun = false;
                                  g_bMgrRun = false;
				  std::cout << "waiting on manager thread to terminate..."<< std::endl;
				  manager.join();
				  
                              }
                              break;

                              default:
                                  std::cout << "unreconized command: " << chIn << " press h for help" << std::endl;
                              } // end of switch...
                          } // end of stdin FD_ISSET
                          else if (FD_ISSET(sockfd, &rdfs))    // data on listening sockect
                          {
                              if (0 < (connfd = accept(sockfd, (struct sockaddr*)&cliAddr, reinterpret_cast<socklen_t*>(&len))))
                              {
                                  pconnInfoT connInfo = new connInfoT;
                                  connInfo->connfd = connfd;
                                  connInfo->cliAddr.sin_family = AF_INET;
                                  connInfo->cliAddr.sin_addr.s_addr = cliAddr.sin_addr.s_addr;
                                  connInfo->cliAddr.sin_port = cliAddr.sin_port;

				  g_mutexque.lock();
                                  g_conns.push_back(connInfo);
				  g_mutexque.unlock();

                                  // send a connected message here....
                                  send(connfd, (char*)&hbMsg, hbMsg.msgLen, 0);
                              }
                              else
                              {
                                  std::cerr << "failed to accept connect, error: " << getLastError() << std::endl;
                                  bRun = false;
                              }
                          } // end of sockfd FD_ISSET...
                          else
                          {
                              std::cerr << "unexpected file descriptor signaled?" << std::endl;
                          }
                      }
                      else                     // send a heartbeat on each timeout...
                      {
                          std::vector<pconnInfoT>::iterator iter = g_conns.begin();
                          if (g_conns.size() > 0)
                          {
                              while (g_conns.end() != iter)
                              {
                                  send((*iter)->connfd, (char*)&hbMsg, hbMsg.msgLen, 0);
                                  ++iter;
                              }
                          }
                      }
                  }   // end of while block

		  std::cout << "main thread exiting, joining manager thread..." << std::endl;

		  std::cout << "closing any still open sockets..." << std::endl;

                  // close all sockets still in the queue.....
                  std::vector<pconnInfoT>::iterator   iter = g_conns.begin();
                  if (g_conns.size() > 0)
                  {   
                      while (g_conns.end() != iter)
                      {
                          const char* str = "Server is shutting down";
                          msgT   msg;
                          msg.msgLen = (short)(4 + strlen(str));           // header length + null terminator...
                          msg.chCode = CMD_SHUTDOWN;
                          memset((void*)msg.szMsg, '\0', 50);            
                          strcpy(msg.szMsg, str);           

                          send((*iter)->connfd, (char*)&msg, msg.msgLen, 0);
#ifdef __WIN
                          closesocket((*iter)->connfd);
#else
			  close((*iter)->connfd);
#endif
                          (*iter)->connfd = -1;
                          ++iter;
                      }
                  }

                  // TODO : shutdown any active games...
              } // end of listen if-block  
              else
              {
                  std::cerr << "falied to listen on socket, error: " << getLastError() << std::endl;
              }
          }  // end of bind if-block
          else
          {
              std::cerr << "failed to bind socket, error: " << getLastError() << std::endl;
          }
      }
      else
      {
          std::cerr << "socket creation failed, error: " << getLastError() << std::endl;
      }
#ifdef __WIN
        WSACleanup();
#endif

  }  // end of init WinSock if block
  return 0;

}


void showUsage()
{
  std::cout << "Commands accepted:" << std::endl;
  std::cout << "l    list clients connected to the server " << std::endl;
  std::cout << "g    list games currently running on server (NYI)" << std::endl;
  std::cout << "f    force a game to start, even with less than 3 players " << std::endl;
  std::cout << "v    shows version information" << std::endl;
  std::cout << "h    shows this screen " << std::endl;
  std::cout << "q    close all sockets and exit" << std::endl;
}



void showVersion()
{
  std::cout << "version: " << lpszVersion << std::endl;
}



bool initWinSock() 
{
  bool bRet = false;
#ifdef __WIN
  
  WORD wVerReq;
  int  nRet = 0;

  wVerReq = MAKEWORD(2,2);
  WSADATA    wsaData;

  if(0 == (nRet = WSAStartup(wVerReq, &wsaData)))
  {
    if(( LOBYTE(wsaData.wVersion) == 2) && (HIBYTE(wsaData.wVersion) == 2))
    {
	bRet = true;
    }
    else
    {
	std::cout << "Unable to find acceptable winsockdll" << std::endl;
    }
  }
  else
  {
    std::cout << "WSAFailed, error is: " << nRet << std::endl;
  }
#else
  bRet = true;
#endif
  
  return bRet;
}



int getLastError()
{
  int nRet = 0;

#ifdef __WIN
  nRet = WSAGetLastError();
#else
  nRet = errno;
#endif

  return nRet;
}
