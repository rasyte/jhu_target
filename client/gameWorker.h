#pragma once

#include <QObject>
#include <QString>


#include "../common/common.h"
#include "logger.h"

class gameWorker : public QObject
{
    Q_OBJECT

public:
    gameWorker(char* serverIP, short sPort);
    ~gameWorker();

public slots:
    void process();
    void sendMsg(QByteArray);
    void onTurnOver();

signals:
    void finished();
    void error(QString err);
    void hrtBeat(QString time);
    void serverShutdown(QString msg);
    void gameBegin(QString msg);
    void selectAvatar(QString);
    void onInit();
    void onTurn();

private:
    bool     m_bRun;
    char*    m_sIP;
    short    m_sPort;
    int /*SOCKET*/   m_soc;               // socket we talk to the server on.


    bool   connectServer();
    void   disconnectServer();



};

// TODO : need to set m_bRun to false in finished handler