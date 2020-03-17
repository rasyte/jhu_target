#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class QSettings;

class loginDlg : public QDialog
{
    Q_OBJECT

public:
    loginDlg(QWidget*, QSettings*);

    QString getUid() { return m_qstrUid; }
    QString getPwd() { return m_qstrPwd; }

private slots:
    void accept();
    void reject();
    void doSettings();

private:
    QString    m_qstrUid;           // user name
    QString    m_qstrPwd;           // password

    QPushButton *m_btnSettings;
    QLineEdit   *m_edtUID;
    QLineEdit   *m_edtPwd;
    QSettings*   m_pSettings;

    void setupUi();
};

