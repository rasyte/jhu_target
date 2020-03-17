#pragma once

#include <QMainWindow>

class QTextEdit;
class QLabel;

class mainWnd : public QMainWindow
{
    Q_OBJECT

public:
    mainWnd(QString, QString, QWidget *parent = Q_NULLPTR);


private:
    void setupUI();
    void createActions();
    void createMenus();
    void createWorker();

    QTextEdit* m_txtState;   
    QLabel*    m_map;

};
