#pragma once

#include <QMainWindow>

class QTextEdit;
class QLabel;

class mainWnd : public QMainWindow
{
    Q_OBJECT

public:
    mainWnd(QString, QString, char* serverPort = nullptr, short sPort = -1, QWidget *parent = Q_NULLPTR);


private slots:
    void errorString(QString);

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createWorker(char*, short);

    QTextEdit* m_txtState;   
    QLabel*    m_map;

};
