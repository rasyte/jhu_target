#pragma once

#include <qdialog.h>
#include <qpushbutton.h>

class GameMenuDlg : public QDialog
{
    Q_OBJECT

public:
    GameMenuDlg(QWidget* parent);

private slots:
private slots:
    // void accept();       -- if we need to simulate an OK button
    void reject();          // This is used to simply close the dialog

private:
    void setupUI();

    QPushButton* m_btnSettings;
    QPushButton* m_btnConnect;
    QPushButton* m_btnDisconnect;
    QPushButton* m_btnReturn;
    QPushButton* m_btnSpare2;
    QPushButton* m_btnExit;

};
