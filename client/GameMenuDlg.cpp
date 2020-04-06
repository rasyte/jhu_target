#include "GameMenuDlg.h"

#include <QVariant>
#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

GameMenuDlg::GameMenuDlg(QWidget* parent) : QDialog(parent)
{ 
    setupUI();
}



void GameMenuDlg::setupUI()
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("Dialog"));
    resize(165, 320);
    setWindowTitle(QString("Dialog"));

    QWidget* pCentralWidget = new QWidget(this);
    pCentralWidget->setObjectName(QString::fromUtf8("widget"));
    pCentralWidget->setGeometry(QRect(10, 10, 144, 311));

    QVBoxLayout* pVerticalLayout = new QVBoxLayout(pCentralWidget);
    pVerticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    pVerticalLayout->setContentsMargins(0, 0, 0, 0);

    QFont font;
    font.setFamily(QString::fromUtf8("AR BERKLEY"));
    font.setPointSize(16);
    
    m_btnSettings = new QPushButton(pCentralWidget);
    m_btnSettings->setObjectName("m_btnSettings");
    m_btnSettings->setFont(font);
    m_btnSettings->setText("Settings");

    m_btnConnect = new QPushButton(pCentralWidget);
    m_btnConnect->setObjectName("m_btnConnect");
    m_btnConnect->setFont(font);
    m_btnConnect->setText("Connect");

    m_btnDisconnect = new QPushButton(pCentralWidget);
    m_btnDisconnect->setObjectName(QString::fromUtf8("m_btnDisconnect"));
    m_btnDisconnect->setFont(font);
    m_btnDisconnect->setText("Disconnect");

    m_btnReturn = new QPushButton(pCentralWidget);
    m_btnReturn->setObjectName("m_btnReturn");
    m_btnReturn->setFont(font);
    m_btnReturn->setText("Return to Game");

    m_btnSpare2 = new QPushButton(pCentralWidget);
    m_btnSpare2->setObjectName("m_btnSpare2");
    m_btnSpare2->setEnabled(false);
    m_btnSpare2->setFont(font);
    m_btnSpare2->setText("spare");

    QSpacerItem* pverticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_btnExit = new QPushButton(pCentralWidget);
    m_btnExit->setObjectName("m_btnExit");
    m_btnExit->setFont(font);
    m_btnExit->setText("Exit Game");

    pVerticalLayout->addWidget(m_btnSettings);
    pVerticalLayout->addWidget(m_btnConnect);
    pVerticalLayout->addWidget(m_btnDisconnect);
    pVerticalLayout->addWidget(m_btnReturn);
    pVerticalLayout->addWidget(m_btnSpare2);
    pVerticalLayout->addItem(pverticalSpacer);
    pVerticalLayout->addWidget(m_btnExit);

    connect(m_btnReturn, SIGNAL(clicked()), this, SLOT(reject()));

}

void GameMenuDlg::reject()
{
    QDialog::reject();
}
