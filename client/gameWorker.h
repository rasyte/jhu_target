#pragma once

#include <QObject>
#include <QString>

#include "logger.h"

class gameWorker : public QObject
{
    Q_OBJECT

public:
    gameWorker();
    ~gameWorker();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

private:


};