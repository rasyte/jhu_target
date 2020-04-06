
#include <QThread>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QApplication>
#include <QTextEdit>
#include <QMessageBox>
#include <QStatusBar>
#include <QKeyEvent>

#include <iostream>

#include "mainWnd.h"
#include "gameWorker.h"
#include "GameMenuDlg.h"
#include "GuessDlg.h"
#include "selectAvatarDlg.h"
#include "../common/common.h"


extern std::vector<int>  g_vecCards;


mainWnd::mainWnd(QString qstrUid, QString qstrPwd, char* serverIP, short sPort, QWidget *parent) : QMainWindow(parent)
{
    setupUI();
    createActions();
    createMenus();
    createWorker(serverIP,sPort);
}


void mainWnd::setupUI() 
{
    QWidget*    centralWidget;   
    QFrame*     line1, * line2, * line3;
    QMenuBar*   menuBar;
    QMenu*      menuGame;
    QToolBar*   mainToolBar;
    QStatusBar* statusBar;
    QGroupBox*  grpBoxRooms;
    QGroupBox*  grpBoxSuspects;
    QGroupBox*  grpBoxWeapons;


    int cntSuspects = sizeof(lpszSuspects) / sizeof(lpszSuspects[0]);
    int cntRooms = sizeof(lpszRooms) / sizeof(lpszRooms[0]);
    int cntWeapons = sizeof(lpszWeapons) / sizeof(lpszWeapons[0]);

    QVector<QLabel*> qvecSuspects;
    QVector<QLabel*> qvecRooms;
    QVector<QLabel*> qvecWeapons;
    QVector<QLabel*> m_qvecCards;

    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("mainWndClass"));
    resize(1200, 919);//resize(887, 919);
    setWindowTitle("clue-less");

    // set up fonts that we will use
    QFont font;
    font.setFamily(QString::fromUtf8("Courier New"));
    font.setPointSize(14);

    QFont font1;
    font1.setFamily(QString::fromUtf8("AR BERKLEY"));
    font1.setPointSize(11);

    // set up central widget
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    this->setCentralWidget(centralWidget);

    // build first note-book page.....
    // build suspects group box
    grpBoxSuspects = new QGroupBox(centralWidget);
    grpBoxSuspects->setObjectName(QString::fromUtf8("grpBoxSuspects"));
    grpBoxSuspects->setGeometry(QRect(10, 10, 191, 151));
    grpBoxSuspects->setTitle("Suspects");
    grpBoxSuspects->setFont(font);

    // build note-book entries...
    for (int ndx = 0; ndx < cntSuspects; ndx++)
    {
        QLabel* pTemp = new QLabel(grpBoxSuspects);
        pTemp->setObjectName(QString("suspect%1").arg(ndx));
        pTemp->setGeometry(QRect(10, 30 + 20 * ndx, 91, 16));
        pTemp->setText(lpszSuspects[ndx]);
        pTemp->setFont(font1);
        qvecSuspects.push_back(pTemp);

        // add in check boxes....
        for (int jdx = 0; jdx < 4; jdx++)
        {
            QCheckBox* pTemp1 = new QCheckBox(grpBoxSuspects);
            pTemp1->setObjectName(QString("chkBox%1%2").arg(ndx).arg(jdx));
            //setGeometry(QRect(x, y, width, heigth)
            pTemp1->setGeometry(QRect(110 + 20 * jdx, 30 + 20 * ndx, 16, 16));
        }
    }

    // add dividing line...
    line1 = new QFrame(grpBoxSuspects);
    line1->setObjectName(QString::fromUtf8("line1"));
    line1->setGeometry(QRect(90, 20, 20, 121));
    line1->setFrameShape(QFrame::VLine);
    line1->setFrameShadow(QFrame::Sunken);


    // build second note-book page...
    // build rooms group box
    grpBoxRooms = new QGroupBox(centralWidget);
    grpBoxRooms->setObjectName(QString::fromUtf8("grpBoxRooms"));
    grpBoxRooms->setGeometry(QRect(10, 160, 191, 221));
    grpBoxRooms->setTitle("Rooms");
    grpBoxRooms->setFont(font);

    // build note-book entries...
    for (int ndx = 0; ndx < cntRooms; ndx++)
    {
        QLabel* pTemp = new QLabel(grpBoxRooms);
        pTemp->setObjectName(QString("room%1").arg(ndx));
        pTemp->setGeometry(QRect(10, 30 + 20*ndx, 91, 16));
        pTemp->setText(lpszRooms[ndx]);
        pTemp->setFont(font1);
        qvecRooms.push_back(pTemp);

        // add in check boxes....
        for (int jdx = 0; jdx < 4; jdx++)
        {
            QCheckBox* pTemp1 = new QCheckBox(grpBoxRooms);
            pTemp1->setObjectName(QString("chkBox%1%2").arg(ndx).arg(jdx));
            //setGeometry(QRect(x, y, width, heigth)
            pTemp1->setGeometry(QRect(110 + 20 * jdx, 30 + 20 * ndx, 16, 16));
        }
    }

    // add dividing line ...
    line2 = new QFrame(grpBoxRooms);
    line2->setObjectName(QString::fromUtf8("line"));
    line2->setGeometry(QRect(90, 20, 20, 191));
    line2->setFrameShape(QFrame::VLine);
    line2->setFrameShadow(QFrame::Sunken);


    // build third note-book page...
    // build weapon group box
    grpBoxWeapons = new QGroupBox(centralWidget);
    grpBoxWeapons->setObjectName(QString::fromUtf8("grpBoxWeapon"));
    grpBoxWeapons->setGeometry(QRect(10, 380, 191, 161));
    grpBoxWeapons->setTitle("Weapons");
    grpBoxWeapons->setFont(font);

    // build note-book entries...
    for (int ndx = 0; ndx < cntWeapons; ndx++)
    {
        QLabel* pTemp = new QLabel(grpBoxWeapons);
        pTemp->setObjectName(QString("weapon%1").arg(ndx));
        pTemp->setGeometry(QRect(10, 30 + 20*ndx, 91, 16));
        pTemp->setText(lpszWeapons[ndx]);
        pTemp->setFont(font1);
        qvecWeapons.push_back(pTemp);

        // add in check boxes....
        for (int jdx = 0; jdx < 4; jdx++)
        {
            QCheckBox* pTemp1 = new QCheckBox(grpBoxWeapons);
            pTemp1->setObjectName(QString("chkBox%1%2").arg(ndx).arg(jdx));
            //setGeometry(QRect(x, y, width, heigth)
            pTemp1->setGeometry(QRect(110 + 20 * jdx, 30 + 20 * ndx, 16, 16));
        }
    }

    // add dividing line ...
    line2 = new QFrame(grpBoxWeapons);
    line2->setObjectName(QString::fromUtf8("line2"));
    line2->setGeometry(QRect(90, 20, 16, 131));
    line2->setFrameShape(QFrame::VLine);
    line2->setFrameShadow(QFrame::Sunken);

    // build state mangement window...
    m_txtState = new QTextEdit(centralWidget);
    m_txtState->setObjectName(QString::fromUtf8("m_txtState"));
    m_txtState->setGeometry(QRect(20, 620, 861, 241));

    // set up map...
    m_map = new QLabel(centralWidget);
    m_map->setObjectName(QString::fromUtf8("label"));
    m_map->setGeometry(QRect(210, 0, 681, 621));
    m_map->setPixmap(QPixmap(QString::fromUtf8("Resources/board.png")));
    m_map->setStyleSheet("border: 1px solid black");

    // setup card locations
    // TODO : this is ugly...however m_qvecCards changes size from here to onInit...FIX THIS
    m_card1 = new QLabel(centralWidget);
    m_card1->setObjectName("card1");
    m_card1->setGeometry(QRect(900, 30, 133, 205));
    m_card1->setStyleSheet("border: 1px solid black");

    m_card2 = new QLabel(centralWidget);
    m_card2->setObjectName("card2");
    m_card2->setGeometry(QRect(1063, 30, 133, 205));
    m_card2->setStyleSheet("border: 1px solid black");

    m_card3 = new QLabel(centralWidget);
    m_card3->setObjectName("card3");
    m_card3->setGeometry(QRect(900, 265, 133, 205));
    m_card3->setStyleSheet("border: 1px solid black");

    m_card4 = new QLabel(centralWidget);
    m_card4->setObjectName("card4");
    m_card4->setGeometry(QRect(1063, 265, 133, 205));
    m_card4->setStyleSheet("border: 1px solid black");

    m_card5 = new QLabel(centralWidget);
    m_card5->setObjectName("card5");
    m_card5->setGeometry(QRect(900, 500, 133, 205));   
    m_card5->setStyleSheet("border: 1px solid black");

    m_card6 = new QLabel(centralWidget);
    m_card6->setObjectName("card6");
    m_card6->setGeometry(QRect(1063, 500, 133, 205));
    m_card6->setStyleSheet("border: 1px solid black");

    //int cnt = 1;
    //for (int row = 30; row < 501; row += 235)
    //{
    //    for (int col = 900; col < 1064; col += 163)
    //    {
    //        QLabel* pTemp = new QLabel(nullptr/*centralWidget*/);
    //        pTemp->setObjectName(QString("card%1").arg(cnt));
    //        pTemp->setGeometry(QRect(col, row, 133, 205));
    //        pTemp->setStyleSheet("border: 1px solid black");
    //        //pTemp->setPixmap(QPixmap(QString("Resources/card%1").arg(3*cnt + 2)));

    //        m_qvecCards.push_back(pTemp);
    //        ++cnt;
    //    }
    //}
 
    // setup StatusBar
    QStatusBar* statusbar = new QStatusBar(this);
    statusbar->setObjectName(QStringLiteral("statusbar"));
    this->setStatusBar(statusbar);

    // setup menu
    //menubar = new QMenuBar(this);
    //menubar->setObjectName(QStringLiteral("menubar"));
    //menubar->setGeometry(QRect(0, 0, WIN_WIDTH, 21));
    //this->setMenuBar(menubar);

    QMetaObject::connectSlotsByName(this);
} 



void mainWnd::createActions() 
{
    //m_fileOpen = new QAction("&Open", this);
    //m_fileOpen->setShortcuts(QKeySequence::Open);
    //m_fileOpen->setStatusTip("opens existing configuration file");
    //connect(m_fileOpen, &QAction::triggered, this, &LanGen::onFileOpen);
}

void mainWnd::createMenus() 
{
    //m_fileMenu = menuBar()->addMenu("&File");
    //m_fileMenu->addAction(m_fileNew);
    //m_fileMenu->addAction(m_fileOpen)
}

void mainWnd::createWorker(char* sIP, short sPort) 
{
    QThread* pThread = new QThread;                 // build communication thread....
    gameWorker* pWorker = new gameWorker(sIP, sPort);
    pWorker->moveToThread(pThread);

    // set up signal/slot connections 
    connect(pWorker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(pWorker, SIGNAL(serverShutdown(QString)), this, SLOT(shutdown(QString)));
    connect(pWorker, SIGNAL(hrtBeat(QString)), this, SLOT(heartBeat(QString)));
    connect(pWorker, SIGNAL(gameBegin(QString)), this, SLOT(gameBegin(QString)));
    connect(pWorker, SIGNAL(onInit()), this, SLOT(onInit()));
    connect(pWorker, SIGNAL(onTurn()), this, SLOT(onTurn()));
    connect(pWorker, SIGNAL(selectAvatar(QString)), this, SLOT(selectAvatar(QString)));
    connect(pWorker, SIGNAL(finished()), pWorker, SLOT(deleteLater()));
    connect(pWorker, SIGNAL(finished()), pThread, SLOT(quit()));
    connect(pThread, SIGNAL(started()), pWorker, SLOT(process()));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));
    connect(this, SIGNAL(sendMsg(QByteArray)), pWorker, SLOT(sendMsg(QByteArray)));
    connect(this, SIGNAL(onTurnOver()), pWorker, SLOT(onTurnOver()));

    pThread->start();                               // start the thread running
}

void mainWnd::keyPressEvent(QKeyEvent* pevt)
{
    if (pevt->key() == Qt::Key_Escape)
    {   
        GameMenuDlg   dlg(this);
        dlg.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        dlg.setWindowTitle("Game Menu");
        dlg.exec();
    }
    else if (pevt->key() == Qt::Key_A)
    {
        guessDlg   dlg(this, false);
        if (QDialog::Accepted == dlg.exec() )
        {
            QString qstrGuess = dlg.getGuess(); 
            msgT  msg;
            msg.msgLen = HDR_LEN + qstrGuess.length();
            msg.chCode = CMD_ACCUSE;
            strcpy(msg.szMsg, qstrGuess.toStdString().c_str());
            // need to comvert msg to a qbytearray so we can use signals/slots.
            char* buf = new char[msg.msgLen];
            memcpy((void*)buf, (void*)&msg, msg.msgLen);
            QByteArray  qbaMsg = QByteArray::fromRawData(buf, msg.msgLen);

            emit sendMsg(qbaMsg);                                // TODO : send the message to server.....
        }

    }
    else if (pevt->key() == Qt::Key_S)
    {
        CLogger::getInstance()->LogMessage("sending a suggestion message....\n");
        guessDlg   dlg(this, true);
        if (QDialog::Accepted == dlg.exec())
        {
            QString qstrGuess = dlg.getGuess();
            msgT  msg;
            msg.msgLen = HDR_LEN + qstrGuess.length();
            msg.chCode = CMD_SUGGEST;
            strcpy(msg.szMsg, qstrGuess.toStdString().c_str());
            // need to comvert msg to a qbytearray so we can use signals/slots.
            char* buf = new char[msg.msgLen];
            memcpy((void*)buf, (void*)&msg, msg.msgLen);
            QByteArray  qbaMsg = QByteArray::fromRawData(buf, msg.msgLen);

            emit sendMsg(qbaMsg);                                // TODO : send the message to server.....
        }
    }
    else if (pevt->key() == Qt::Key_O)
    {
        CLogger::getInstance()->LogMessage("sending turn over message");
        int nRet = QMessageBox::question(this, "end turn", "Do you wish to end your turn?");
        if(QDialog::Accepted == nRet)
            emit onTurnOver();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// private slots:
void mainWnd::errorString(QString msg)
{
    QMessageBox::critical(this, "[mainWnd.cpp] Error", msg);
}

void mainWnd::shutdown(QString msg)
{
    QString strHtml= QString("<font color=\"red\">%1</font>").arg(msg);
    m_txtState->insertHtml(strHtml);

}

void mainWnd::heartBeat(QString msg)
{
    QString strHtml = QString(msg);
    statusBar()->showMessage(msg);
}

void mainWnd::gameBegin(QString msg)
{
    QString strHtml = QString("%1").arg(msg);
    m_txtState->insertHtml(strHtml);
}

void mainWnd::selectAvatar(QString  avatars)
{
    CLogger::getInstance()->LogMessage("[mainWnd]: received from comms thread");
    for (int ndx = 0; ndx < NBR_SUSPECTS; ndx++)
    {
        CLogger::getInstance()->LogMessage("%c", avatars.at(ndx));
    }


    selectAvatarDlg   dlg(this, avatars);

    if (QDialog::Accepted == dlg.exec())
    {
        QString   newList = dlg.getAvatars();
        CLogger::getInstance()->LogMessage("[mainWnd]: sending to server:");
        for (int ndx = 0; ndx < NBR_SUSPECTS; ndx++)
        {
            CLogger::getInstance()->LogMessage("%c", newList.at(ndx));
        }
        CLogger::getInstance()->LogMessage("sending message to comms thread");
        emit sendMsg(QByteArray(newList.toStdString().c_str(), newList.size()));

    }
}

/*
cards are created in order: 1   2
                            3   4
                            5   6
 cards [0-9] are suspect    resource wise they are card1 - card6
 cards [10-19] are weapon   resource wise they are card7 - card12    10 -10 = 0 + 7 = 7
 cards [20-21] are rooms    resource wise they are card13- card21    15 - 10 = 5 + 7 = 12
*/                                                                    
void mainWnd::onInit()
{       
    std::vector<int> cardID;
    // TODO : check the conversion here ....
    CLogger::getInstance()->LogMessage("[in onInit] in vector we have:");
    for (int ndx = 0; ndx < g_vecCards.size(); ndx++)
    {
        int cID;
        int card = g_vecCards.at(ndx);
        if (card < 9) cID = card+1;                // cards in resources are one based.
        else if (card < 20) cID = (card - 10) + 7; //[10-19]  -> [0-9] -> [6 - 15]
        else cID = (card - 20) + 12;               // [20-29] -> [0-9] -> [12 - 21]
        cardID.push_back(cID);

        CLogger::getInstance()->LogMessage("%d -> %d  \n", card, cID);
    }
    m_card1->setPixmap(QPixmap(QString("Resources/card%1").arg(cardID.at(0))));
    m_card3->setPixmap(QPixmap(QString("Resources/card%1").arg(cardID.at(1))));
    m_card5->setPixmap(QPixmap(QString("Resources/card%1").arg(cardID.at(2))));
}

void mainWnd::onTurn()
{
    QMessageBox::information(this, "Turn alert", "Your turn, press O to end turn");
}
