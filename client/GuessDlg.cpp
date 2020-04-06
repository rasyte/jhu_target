#include "GuessDlg.h"
#include "../common/common.h"

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QWidget>
#include <QMessageBox>



guessDlg::guessDlg(QWidget* parent, bool bMode) : QDialog(parent), m_bMode(bMode)
{ 
    setupUI();

}

void guessDlg::setupUI()
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("GuessDlg"));
    resize(376, 187);
    if (m_bMode)
        setWindowTitle("Make suggestion...");
    else
        setWindowTitle("Make accusation...");

    QWidget* playoutWidget = new QWidget(this);
    playoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    playoutWidget->setGeometry(QRect(10, 140, 351, 37));

    QFont font;
    font.setFamily(QString::fromUtf8("AR BERKLEY"));
    font.setPointSize(16);

    QHBoxLayout* pHBoxLayout = new QHBoxLayout(playoutWidget);
    pHBoxLayout->setSpacing(6);
    pHBoxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    pHBoxLayout->setContentsMargins(0, 0, 0, 0);
    
    QSpacerItem* spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QPushButton* okButton = new QPushButton(playoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));
    okButton->setFont(font);
    okButton->setText("OK");

    QPushButton* cancelButton = new QPushButton(playoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
    cancelButton->setFont(font);
    cancelButton->setText("Cancel");

    pHBoxLayout->addItem(spacerItem);
    pHBoxLayout->addWidget(okButton);
    pHBoxLayout->addWidget(cancelButton);

    QLabel* lblSuspect = new QLabel(this);
    lblSuspect->setObjectName(QString::fromUtf8("lblSuspect"));
    lblSuspect->setGeometry(QRect(10, 20, 91, 16));
    lblSuspect->setText("Suspect :");
    lblSuspect->setFont(font);

    m_cboSuspect = new QComboBox(this);
    m_cboSuspect->setObjectName(QString::fromUtf8("comboBox"));
    m_cboSuspect->setGeometry(QRect(100, 20, 221, 21));
    unsigned int cntSuspects = sizeof(lpszSuspects) / sizeof(lpszSuspects[0]);
    m_cboSuspect->insertItem(0, "  ");
    for(unsigned int ndx=0; ndx < cntSuspects; ndx++)
    {
        m_cboSuspect->insertItem(ndx+1, lpszSuspects[ndx]);
    }
    m_cboSuspect->setCurrentIndex(0);
    m_cboSuspect->setFont(font);

    QLabel* lblWeapons = new QLabel(this);
    lblWeapons->setObjectName(QString::fromUtf8("lblWeapons"));
    lblWeapons->setGeometry(QRect(10, 60, 91, 16));
    lblWeapons->setText("Weapon :");
    lblWeapons->setFont(font);

    m_cboWeapon = new QComboBox(this);
    m_cboWeapon->setObjectName(QString::fromUtf8("comboBox_2"));
    m_cboWeapon->setGeometry(QRect(100, 60, 221, 21));
    unsigned int cntWeapons = sizeof(lpszWeapons) / sizeof(lpszWeapons[0]);
    m_cboWeapon->insertItem(0, " ");
    for (unsigned int ndx = 0; ndx < cntWeapons; ndx++)
    {
        m_cboWeapon->insertItem(ndx + 1, lpszWeapons[ndx]);
    }
    m_cboWeapon->setCurrentIndex(0);
    m_cboWeapon->setFont(font);

    QLabel* lblRooms = new QLabel(this);
    lblRooms->setObjectName(QString::fromUtf8("lblRooms"));
    lblRooms->setGeometry(QRect(10, 100, 91, 16));
    lblRooms->setText("Room    :");
    lblRooms->setFont(font);
    lblRooms->setEnabled(!m_bMode);

    m_cboRoom = new QComboBox(this);
    m_cboRoom->setObjectName(QString::fromUtf8("comboBox_3"));
    m_cboRoom->setEnabled(!m_bMode);
    m_cboRoom->setGeometry(QRect(100, 100, 221, 21));
    unsigned int cntRooms = sizeof(lpszRooms) / sizeof(lpszRooms[0]);
    m_cboRoom->insertItem(0, " ");
    for (unsigned int ndx = 0; ndx < cntRooms; ndx++)
    {
        m_cboRoom->insertItem(ndx + 1, lpszRooms[ndx]);
    }
    m_cboRoom->setCurrentIndex(0);
    m_cboRoom->setFont(font);

    //retranslateUi(GuessDlg);
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

}
    
void guessDlg::accept()
{
    int suspect;
    int weapon;
    int room;    // only care about room in accusation mode

    if (0 != (suspect = m_cboSuspect->currentIndex()))
    {
        if (0 != (weapon = m_cboWeapon->currentIndex()))
        {
            room = m_cboRoom->currentIndex();
            if ((0 != room) || m_bMode)
            {
                // build the guess string
                if (m_bMode)
                {
                    // TODO: need to query state for correct room location
                }

                
                m_qstrGuess = QString("%1%2%3").arg(suspect).arg(weapon).arg(room);
                QDialog::accept();              
            }
            else
            {
                QMessageBox::warning(this, "Error", "[GuessDlg.cpp] You must select a room");
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "[GuessDlg.cpp] You must select a weapon");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "[GuessDlg.cpp] You must select a suspect");
        
    }
    






}
    

void guessDlg::reject()
{
    QDialog::reject();
}



