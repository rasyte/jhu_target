#pragma once

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDialog>

class QSettings;

class settingDlg:public QDialog
{
public:
    settingDlg(QWidget*, QSettings*);

private slots:
    void accept();
    void reject();

private:
    void setupUi();
 
    QLineEdit *m_edtServer;
    QLineEdit *m_edtPort;

    QSettings* m_pSettings;
    QString    m_qstrServer;
    QString    m_qstrPort;


};
