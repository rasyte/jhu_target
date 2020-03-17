/********************************************************************************
** Form generated from reading UI file 'loginDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TEMP_H
#define TEMP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *m_btnSettings;
    QLabel *label1;
    QLabel *label_2;
    QLineEdit *m_edtUID;
    QLineEdit *m_edtPwd;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(283, 183);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 140, 251, 33));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(12);
        layoutWidget->setFont(font);
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setFont(font);

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setFont(font);

        hboxLayout->addWidget(cancelButton);

        m_btnSettings = new QPushButton(Dialog);
        m_btnSettings->setObjectName(QString::fromUtf8("m_btnSettings"));
        m_btnSettings->setGeometry(QRect(10, 10, 41, 41));
        QIcon icon;
        icon.addFile(QString::fromUtf8("Resources/gear.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8("Resources/gear.png"), QSize(), QIcon::Normal, QIcon::On);
        m_btnSettings->setIcon(icon);
        label1 = new QLabel(Dialog);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setGeometry(QRect(20, 70, 100, 16));
        label1->setFont(font);
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 110, 100, 16));
        label_2->setFont(font);
        m_edtUID = new QLineEdit(Dialog);
        m_edtUID->setObjectName(QString::fromUtf8("m_edtUID"));
        m_edtUID->setGeometry(QRect(140, 70, 113, 20));
        m_edtUID->setFont(font);
        m_edtPwd = new QLineEdit(Dialog);
        m_edtPwd->setObjectName(QString::fromUtf8("m_edtPwd"));
        m_edtPwd->setGeometry(QRect(140, 110, 113, 20));
        m_edtPwd->setFont(font);

        retranslateUi(Dialog);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        okButton->setText(QApplication::translate("Dialog", "Log In", nullptr));
        cancelButton->setText(QApplication::translate("Dialog", "Cancel", nullptr));
        m_btnSettings->setText(QApplication::translate("Dialog", "PushButton", nullptr));
        label1->setText(QApplication::translate("Dialog", "User Name", nullptr));
        label_2->setText(QApplication::translate("Dialog", "Password", nullptr));
        m_edtUID->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TEMP_H
