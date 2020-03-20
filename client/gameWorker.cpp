#include "gameWorker.h"
#include "logger.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>

#include <QSettings>

const int HDR_LEN = 3;                         // out message header length

extern int networkError();

gameWorker::gameWorker(char* serverIP, short sPort): m_sIP(nullptr), m_sPort(-1)
{
    CLogger::getInstance()->LogMessage("gameWorker constructor");
    
    if (serverIP != nullptr)
    {
        m_sIP = new char[strlen(serverIP) + 1];
        memset((void*)m_sIP, '/0', strlen(serverIP) + 1);
        strcpy(m_sIP, serverIP);
    }
    if(sPort > 0) m_sPort = sPort;
}

gameWorker::~gameWorker()
{
    CLogger::getInstance()-> LogMessage("gameWorker destructor");
    if (m_sIP != nullptr) delete[] m_sIP;
}

void gameWorker::process()
{
    CLogger::getInstance()->LogMessage("worker alive and running");

    
    if (connectServer())
    {
        m_bRun = true;
        fd_set                  rdfs;               // input descriptor to listen on...
        struct timeval          tv;

        while (m_bRun)
        {
            FD_ZERO(&rdfs);
            FD_SET(m_soc, &rdfs);               // monitor our listening socket for input

            tv.tv_sec = 5;                      // set timeout for 5 sec
            tv.tv_usec = 0;

            int ret = select(m_soc + 1, &rdfs, nullptr, nullptr, &tv);
            if (-1 == ret)
            {
                CLogger::getInstance()->LogMessage("select failed");
                // TODO : we need to show this in an error dialog...
                m_bRun = false;                 // QUESTION : is this the correct behavior, close connection?
            }
            else if (ret > 0)
            {
                if (FD_ISSET(m_soc, &rdfs))             // server sent us data
                {
                    //  +--------------+-------------------+---------------------------+
                    //  | len (2 bytes)| msg code (1 byte) | msg content (variable len |
                    //  +--------------+-------------------+---------------------------+
                    char  hdr[HDR_LEN];

                    recv(m_soc, &hdr[0], HDR_LEN, 0);                // read in header...
                    short msgLen = ntohs((hdr[0] << 8) | hdr[1]);    // get message length from header
                    unsigned char cmd = hdr[2];                      // get command from header
                    try
                    {
                        if (msgLen > 3)                                  // remember message length includes header
                        {
                            char* buf = new char[msgLen - 3];
                            recv(m_soc, buf, msgLen - 3, 0);
                        

                            switch (cmd)
                            {
                                case  CMD_HRT_BEAT:                             // got a heart-beat from server
                                {
                                    emit hrtBeat(QString(buf));
                                    break;
                                }
                                case CMD_UPDATE_STATE:                          // got a state update from server
                                {
                                    break;
                                }
                                case CMD_SUGGEST:                               // got a suggestion from server
                                {
                                    break;
                                }
                                case CMD_ACCUSE:                                // got an accquision from server
                                {
                                    break;
                                }
                                case CMD_INIT:                                  // got a board initialization from server
                                {
                                    break;
                                }
                                case CMD_PLAYER_JOIN:
                                {
                                    break;
                                }
                                default:
                                    CLogger::getInstance()->LogMessage("Unknown message, command %d\n", cmd);
                            }
                            delete[] buf;
                        }
                    }
                    catch (std::bad_alloc)
                    {
                        CLogger::getInstance()->LogMessage("failed to allocate buffer of message contents\n");
                        m_bRun = false;
                    }
                }
                else                                   // this should not happen
                {
                    CLogger::getInstance()->LogMessage("unexpected file descriptor signaled?");
                }
            }
            else
            {
                CLogger::getInstance()->LogMessage("timeout has occured");
                // we should allow QT to process messages here....
            }

        }

        disconnectServer();


    }
    else
    {
        CLogger::getInstance()->LogMessage("server connect failed, error is %d\n", networkError());
    }


    emit finished();
}


bool gameWorker::connectServer()
{
    bool    bRet = false;
    QString qstrServer;
    short   sPort;
    CLogger::getInstance()->LogMessage("attempting to connect to server");

    if (-1 != (m_soc = socket(AF_INET, SOCK_STREAM, 0)))
    {
        // get server address/port from settings .... see note in main about dynamically updateing settings.
        QSettings  setting("JHUProj", "clue-less");
        if (m_sIP == nullptr)
        {
            qstrServer = setting.value("server", "").toString();
        }
        else
        {
            qstrServer = QString(m_sIP);
        }

        if (m_sPort = -1)
        {
            sPort = setting.value("port", "").toString().toShort();
        }
        else
        {
            sPort = m_sPort;
        }

        struct sockaddr_in  serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(qstrServer.toStdString().c_str());
        serverAddr.sin_port = htons(sPort);

        if (-1 != ::connect(m_soc, (const sockaddr*)&serverAddr, sizeof(serverAddr)))
        {
            bRet = true;
        }
        else
        {
            CLogger::getInstance()->LogMessage("Failed to connect to server, error: %d\n", networkError());
            emit error("failed to connect to server, check network connections");
        }

    }
    else
    {
        CLogger::getInstance()->LogMessage("failed to open socket, error %d\n", networkError());
    }

    return bRet;
}



void gameWorker::disconnectServer()
{
#ifdef __WIN
    if (0 != closesocket(m_soc))
    {
        CLogger::getInstance()->LogMessage("failed to close socket, error %d\n", networkError());
    }
#else
    if (0 != close(m_soc))
    {
        CLogger::::getInstance()->LogMessage("failed to close socket, error %d\n", networkError());
    }
#endif
}
