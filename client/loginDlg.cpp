
#include <QMessageBox>
#include <QSettings>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include "loginDlg.h"
#include "settingDlg.h"

loginDlg::loginDlg(QWidget* parent, QSettings* pSettings) : QDialog(parent)
{
    setupUi();
    m_pSettings = pSettings;

    // if we have no value for server or port, so settings dialog...
    if ((pSettings->value("server", "") == "") || (pSettings->value("port", "") == ""))
        doSettings();
}



void loginDlg::setupUi()
{
    QLabel      *label1;
    QLabel      *label2;
    QWidget     *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    if (objectName().isEmpty()) setObjectName(QString::fromUtf8("Dialog"));
    setWindowTitle("Login to game server");
    resize(283, 183);

    // set the font for the dialog...
    QFont font;
    font.setFamily(QString::fromUtf8("Courier New"));
    font.setPointSize(12);    
    
    layoutWidget = new QWidget(this);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 140, 251, 33));
    layoutWidget->setFont(font);
    
    // set up the button box...
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // build OK button....
    okButton = new QPushButton(layoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));
    okButton->setText(QApplication::translate("Dialog", "Log In", nullptr));
    okButton->setFont(font);

    cancelButton = new QPushButton(layoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
    cancelButton->setText(QApplication::translate("Dialog", "Cancel", nullptr));
    cancelButton->setFont(font);

    // add items to the button box...
    hboxLayout->addItem(spacerItem);
    hboxLayout->addWidget(okButton);
    hboxLayout->addWidget(cancelButton);

    // build settings button
    m_btnSettings = new QPushButton(this);
    m_btnSettings->setObjectName(QString::fromUtf8("m_btnSettings"));
    m_btnSettings->setText(QApplication::translate("Dialog", "PushButton", nullptr));
    m_btnSettings->setGeometry(QRect(10, 10, 41, 41));
    m_btnSettings->setIcon(QIcon(":/mainWnd/Resources/gear.png"));
    
    // build username GUI items....
    label1 = new QLabel(this);
    label1->setObjectName(QString::fromUtf8("label1"));
    label1->setText(QApplication::translate("Dialog", "User Name", nullptr));
    label1->setGeometry(QRect(20, 70, 100, 16));
    label1->setFont(font);

    m_edtUID = new QLineEdit(this);
    m_edtUID->setObjectName(QString::fromUtf8("m_edtUID"));
    m_edtUID->setGeometry(QRect(140, 70, 113, 20));
    m_edtUID->setFont(font);

    // build password GUI items....
    label2 = new QLabel(this);
    label2->setObjectName(QString::fromUtf8("label_2"));
    label2->setText(QApplication::translate("Dialog", "Password", nullptr));
    label2->setGeometry(QRect(20, 110, 100, 16));
    label2->setFont(font);

    m_edtPwd = new QLineEdit(this);
    m_edtPwd->setObjectName(QString::fromUtf8("m_edtPwd"));
    m_edtPwd->setGeometry(QRect(140, 110, 113, 20));
    m_edtPwd->setEchoMode(QLineEdit::Password);
    m_edtPwd->setFont(font);


    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(m_btnSettings, SIGNAL(clicked()), this, SLOT(doSettings()));
} 

void loginDlg::accept()
{
    if (m_edtUID->text().isEmpty())
    {
        QMessageBox::critical(this, "error - missing user name", "please enter a user name");
    }
    else if (m_edtPwd->text().isEmpty())
    {
        QMessageBox::critical(this, "error - missing password", "please enter a password");
    }
    else
    {
        m_qstrUid = m_edtUID->text();             
        m_qstrPwd = m_edtPwd->text();            // TODO: probably should encrypt strings prior to returning...
        QDialog::accept();
    }
}



void loginDlg::reject()
{
    QDialog::reject();
}



void loginDlg::doSettings()
{
    qDebug("in doSettings");

    settingDlg  dlg(nullptr, m_pSettings);
    if (QDialog::Accepted == dlg.exec())
    {
        // todo : save the current values for server & port
    }
    else               // use hit cancel, we have no value we need so bail...
    {

    }
}




