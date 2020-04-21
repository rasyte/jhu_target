#pragma once

#include <QMainWindow>
#include "../common/common.h"

class QTextEdit;
class QLabel;
class QThread;
class gameWorker;
class gameBoard;

class mainWnd : public QMainWindow
{
    Q_OBJECT

public:
    mainWnd(QString, QString, char* serverPort = nullptr, short sPort = -1, QWidget *parent = Q_NULLPTR);
    void keyPressEvent(QKeyEvent* pevt);
    
    int  getCurrentLoc() { return m_nCurrentLoc; }

signals:
    void sendMsg(int, QByteArray);
    void onTurnOver();

private slots:
    void errorString(QString);
    void shutdown(QString);
    void heartBeat(QString);
    void gameBegin(QString);
    void selectAvatar(QString);
    void onInit();
    void onTurn();
    // slots for the main game menu...
    void onSettings();
    void onConnect();
    void onDisconnect();
    void onThreadOver();
    void doSuggestion(int);
    void onExit();
    void currentLoc(int);

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createConnections();
    void createWorker(char*, short);

    QTextEdit*       m_txtState;   
    QLabel*          m_map;
    gameBoard*       m_pBoard;
    QThread*         m_pThread;
    gameWorker*      m_pWorker;
    int              m_nCurrentLoc;
    //QVector<QLabel*> m_qvecCards;
    QLabel*          m_card1;
    QLabel*          m_card2;
    QLabel*          m_card3;
    QLabel*          m_card4;
    QLabel*          m_card5;
    QLabel*          m_card6;

};
