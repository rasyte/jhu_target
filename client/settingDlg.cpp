#include "settingDlg.h"


#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <QSettings>
#include <QMessageBox>

settingDlg::settingDlg(QWidget* parent, QSettings* psettings) : QDialog(parent)
{
    m_pSettings = psettings;
    // get our initial values....
    m_qstrServer = m_pSettings->value("server", "").toString();
    m_qstrPort = m_pSettings->value("port", "").toString();

    setupUi();

}



void settingDlg::setupUi()
{    
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *label1;
    QLabel *label2;

    // build our font for the dialog
    QFont font;
    font.setFamily(QString::fromUtf8("Courier New"));
    font.setPointSize(12);

    // build dialog object....
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("Dialog"));
    resize(300, 161);
    setWindowTitle(QApplication::translate("Dialog", "Network Settings", nullptr));
    setFont(font);

    
    layoutWidget = new QWidget(this);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(20, 110, 221, 33));
    
    // set up the button box.....
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);
    okButton = new QPushButton(layoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));
    okButton->setText("OK");

    cancelButton = new QPushButton(layoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
    cancelButton->setText("Cancel");

    hboxLayout->addItem(spacerItem);
    hboxLayout->addWidget(okButton);
    hboxLayout->addWidget(cancelButton);

    // build GUI elements for server...
    label1 = new QLabel(this);
    label1->setObjectName(QString::fromUtf8("label1"));
    label1->setGeometry(QRect(10, 30, 110, 13));
    label1->setText("Server  IP:"); 

    m_edtServer = new QLineEdit(this);
    m_edtServer->setObjectName(QString::fromUtf8("m_edtServer"));
    m_edtServer->setGeometry(QRect(120, 30, 160, 20)); 
    m_edtServer->setText(m_qstrServer);
    
    // build GUI element for port...
    label2 = new QLabel(this);
    label2->setObjectName(QString::fromUtf8("label2"));
    label2->setGeometry(QRect(10, 60, 110, 13));
    label2->setText( "Port     :  ");

    m_edtPort = new QLineEdit(this);
    m_edtPort->setObjectName(QString::fromUtf8("m_edtPort"));
    m_edtPort->setGeometry(QRect(120, 60, 160, 20));
    m_edtPort->setText(m_qstrPort);

    // setup connections
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

} 


void settingDlg::accept()
{
    if (m_edtServer->text() == "")
    {
        QMessageBox::critical(nullptr, "[settingsDlg] : Error - missing value", "you must provide a server IP");
    }
    else if (m_edtPort->text() == "")
    {
        QMessageBox::critical(nullptr, "[settingsDlg] : Error - missing value", "you must provide a port number");
    }
    else
    {
        if (m_edtServer->text() != m_qstrServer)        // value has changed, update settings...
        {
            m_pSettings->setValue("server", m_qstrServer);
        }

        if (m_edtPort->text() != m_qstrPort)
        {
            m_pSettings->setValue("port", m_qstrPort);
        }
        m_pSettings->sync();
        QDialog::accept();
    }
            

}



void settingDlg::reject()
{
    QDialog::reject();
}

