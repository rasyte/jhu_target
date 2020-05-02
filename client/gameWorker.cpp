#include "../common/common.h"
#include "gameWorker.h"
#include "util.h"
#include "logger.h"

#ifdef __WIN
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>

  #include <unistd.h>
#endif

#include <time.h>
#include <QCoreApplication>
#include <QSettings>
#include <QVector>

#include <iostream>
#include <iterator>
#include <algorithm>

//const int HDR_LEN = 3;                         // out message header length

extern int networkError();
std::vector<int>  g_vecCards;                    // TODO : figure out how to make this work with signals/slots

gameWorker::gameWorker(char* serverIP, short sPort): m_sIP(nullptr), m_sPort(-1)
{
    CLogger::getInstance()->LogMessage("gameWorker] -- gameWorker constructor");

    m_bConnected = false;
    
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
    CLogger::getInstance()-> LogMessage("[gameWorker] -- gameWorker destructor");
    if (m_bConnected)
    {
        disconnectServer();
    }
    if (m_sIP != nullptr) delete[] m_sIP;
}

void gameWorker::quit()
{
    CLogger::getInstance()->LogMessage("[gameWorker] -- resetting m_bRun to false");
    m_bRun = false;
}

void gameWorker::process()
{
    CLogger::getInstance()->LogMessage("[gameWorker] -- worker alive and running");

    if ((m_bConnected = connectServer()))
    {
        m_bRun = true;
        fd_set                  rdfs;               // input descriptor to listen on...
        struct timeval          tv;

        while (m_bRun)
        {
            CLogger::getInstance()->LogMessage("[gameWorker] -- top of while loop");
            FD_ZERO(&rdfs);
            FD_SET(m_soc, &rdfs);               // monitor our listening socket for input

            tv.tv_sec = 5;                      // set timeout for 5 sec
            tv.tv_usec = 0;

            int ret = select(m_soc + 1, &rdfs, nullptr, nullptr, &tv);
            if (-1 == ret)
            {
                CLogger::getInstance()->LogMessage("[gameWorker] -- select failed error is %d\n",networkError());
                // TODO : we need to show this in an error dialog...
                //m_bRun = false;                 // QUESTION : is this the correct behavior, close connection?
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
                    short msgLen = hdr[0];                           // get message length
                    unsigned char cmd = hdr[2];                      // get command from header
                    try
                    {
                        if (msgLen > 3)                                  // remember message length includes header
                        {
                            char* buf = new char[msgLen - 3 + 1];        // add space for null terminator if needed
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
                                case CMD_SUGGEST_RSP:                               // got a suggestion from server
                                {
                                    std::cout << "[gameWorker] in cmd_suggest_rsp, buffer is:" << std::endl;
                                    printBuffer(buf, 4);

                                    QByteArray qba(buf, 4);
                                    std::cout << "[gameWorker] in cmd_suggest_rsp, qba is:" << std::endl;
                                    printQBA(&qba);
                                    emit onSuggestRsp(qba);
                                    break;
                                }
                                case CMD_ACCUSE_RSP:                                // got an accusation from server
                                {
                                    QByteArray qba(buf, 4);
                                    emit onAccuseRsp(qba);
                                    break;
                                }
                                case CMD_GAME_OVER:                                //got game over from server
                                {
                                    CLogger::getInstance()->LogMessage("[gameWorker] In CMD_GAME_OVER handler");
                                    QByteArray qba(buf, 4);
                                    emit onGameOver(qba);
                                    m_bRun = false;
                                    break;
                                }
                                case CMD_INIT:                                  // got a board initialization from server
                                {
                                    char* cards = new char[msgLen];
                                    memcpy(cards, buf, msgLen);
                                    for (int ndx = 0; ndx < msgLen; ndx++)
                                    {
                                        g_vecCards.push_back((char)cards[ndx]); 
                                    }

                                    emit onInit();
                                    break;
                                }
                                case CMD_TURN:
                                {
                                    CLogger::getInstance()->LogMessage("[gameWorker] In CMD_TURN handler");
                                    emit onTurn();
                                    break;
                                }
                                case CMD_PLAYER_JOIN:                           // player joined the game
                                {
                                    break;
                                }
                                case CMD_GAME_BEGIN:                            // game is starting soon....
                                {
                                    buf[msgLen - 3] = '\0';                     // force a null terminator
                                    emit gameBegin(QString(buf));
                                    break;
                                }
                                case CMD_GAME_SELECT:                           // character to select avatar...
                                {
                                    emit selectAvatar(QString(buf));
                                    break;
                                }
                                case CMD_SHUTDOWN:
                                {
                                    emit serverShutdown(QString(buf));
                                    m_bRun = false;
                                    break;
                                }
                                default:
                                    CLogger::getInstance()->LogMessage("[gameWorker] -- Unknown message, command %d\n", cmd);
                            }
                            delete[] buf;
                        }
                    }
                    catch (std::bad_alloc)
                    {
                        CLogger::getInstance()->LogMessage("[gameWorker] -- failed to allocate buffer of message contents\n");
                        m_bRun = false;
                    }
                }
                else                                   // this should not happen
                {
                    CLogger::getInstance()->LogMessage("[gameWorker] -- unexpected file descriptor signaled?");
                }
            }
            else
            {
                //CLogger::getInstance()->LogMessage(" Worker: timeout has occured");
                QCoreApplication::processEvents();  // force QT to process messages here....
                
            }
        }

        // TODO : we are bailing out of the while loop...probably need to hang-up on server
        //      : dtor calls disconnect server....should we send a disconnect mesage to server so out
        //      : connection is cleared from all queues?
    }
    else
    {
        CLogger::getInstance()->LogMessage("[gameWorker] -- server connect failed, error is %d\n", networkError());
    }

    emit finished();
}


bool gameWorker::connectServer()
{
    bool    bRet = false;
    QString qstrServer;
    short   sPort;
    CLogger::getInstance()->LogMessage("[gameWorker] -- attempting to connect to server");

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

        if (m_sPort == -1)
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
            CLogger::getInstance()->LogMessage("[gameWorker] -- Failed to connect to server, error: %d\n", networkError());
            emit error("failed to connect to server, check network connections");
        }

    }
    else
    {
        CLogger::getInstance()->LogMessage("[gameWorker] -- failed to open socket, error %d\n", networkError());
    }

    return bRet;
}


void gameWorker::sendMsg(int cmd, QByteArray qbaMsg)
{
    CLogger::getInstance()->LogMessage("got a request to send a message\n");

    QString    qstrMsg(qbaMsg.constData());
    msgT   msg;
    msg.msgLen = 3 + qstrMsg.length();
    msg.chCode = cmd;                
    strcpy(msg.szMsg, qstrMsg.toStdString().c_str());
    int nRet = send(m_soc, (const char*)&msg, msg.msgLen, 0);


    CLogger::getInstance()->LogMessage("done sending message");
}


void gameWorker::onTurnOver()
{
    const char* str = "turn over";
    CLogger::getInstance()->LogMessage("got a turn over message\n");

    msgT   msg;
    msg.msgLen = HDR_LEN + strlen(str);                    
    msg.chCode = CMD_TURN_OVER;
    strcpy(msg.szMsg, str);

    int nRet = send(m_soc, (const char*)&msg, msg.msgLen, 0);
}



void gameWorker::disconnectServer()
{
#ifdef __WIN
    if (0 != closesocket(m_soc))
    {
        CLogger::getInstance()->LogMessage("[gameWorker] -- failed to close socket, error %d\n", networkError());
    }
#else
    if (0 != close(m_soc))
    {
        CLogger::getInstance()->LogMessage("[gameWorker] -- failed to close socket, error %d\n", networkError());
    }
#endif
}



