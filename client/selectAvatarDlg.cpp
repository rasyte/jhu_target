#include "../common/common.h"
#include "selectAvatarDlg.h"

#include <QCheckBox>
#include <QPushButton>
#include <QByteArray>
#include <QMessageBox>

#include <iostream>
#include <iterator>
#include <algorithm>

selectAvatarDlg::selectAvatarDlg(QWidget* parent, QString qba) : QDialog(parent), m_avatar(-1), m_qstrAvatars(qba)
{
    std::cout << std::endl;
    setupUI();
}

void selectAvatarDlg::setupUI()
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("selectAvatarDlg"));
    resize(175, 182);
    setContextMenuPolicy(Qt::NoContextMenu);
    setWindowTitle("Select Avatar");

    QFont font;
    font.setFamily(QString::fromUtf8("AR BERKLEY"));
    font.setPointSize(16);

    for (int ndx = 0; ndx < NBR_SUSPECTS; ndx++)
    {
        QCheckBox* pTemp = new QCheckBox(this);
        pTemp->setObjectName(QString("btnAvatar%1").arg(ndx));
        pTemp->setGeometry(QRect(10, 10 + 20*ndx, 130, 17));
        pTemp->setText(lpszSuspects[ndx]);
        pTemp->setFont(font);
        if ('u' == m_qstrAvatars.at(ndx))
        {
            pTemp->setEnabled(false);           // avatar has already been selected
            pTemp->setChecked(true);            // show it as selected
        }
        connect(pTemp, SIGNAL(stateChanged(int)), this, SLOT(avatarSelected(int)));
        m_vecPushButton.push_back(pTemp);
    }

    m_btnSelect = new QPushButton(this);
    m_btnSelect->setObjectName(QString::fromUtf8("m_btnSelect"));
    m_btnSelect->setGeometry(QRect(10, 130, 75, 23));
    m_btnSelect->setText("Select");
    m_btnSelect->setFont(font);

    connect(m_btnSelect, SIGNAL(clicked()), this, SLOT(onDone()));
}


void selectAvatarDlg::onDone()
{
    QString  trial;

    // get current state of the Dialog...
    for (int ndx = 0; ndx < NBR_SUSPECTS; ndx++)
    {
        if (m_vecPushButton.at(ndx)->isChecked())
        {
            trial.append('u');
        }
        else
        {
            trial.append('a');
        }
            
    }

    // compare the trial to the original, if the same user had not selected..
    if (trial == m_qstrAvatars)   
    {
        QMessageBox::critical(this, "error", "you must select an avatar");
    }
    else                                        // accept the current selection
    {
        m_qstrAvatars = trial;
        QDialog::accept();
    }
}

void selectAvatarDlg::avatarSelected(int)
{
    QCheckBox* pBox = (QCheckBox*)sender();

    if (Qt::Unchecked == pBox->checkState())
        m_avatar = -1;
    else if (Qt::Checked == pBox->checkState())
    {
        for (int ndx = 0; ndx < NBR_SUSPECTS; ndx++)
        {
            if (m_vecPushButton[ndx] == pBox)
            {
                m_avatar = ndx;
                break;
            }
        }
    }
        

}
