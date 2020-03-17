#include "gameWorker.h"
#include "logger.h"

gameWorker::gameWorker()
{
    CLogger::getInstance()->LogMessage("gameWorker constructor");

}

gameWorker::~gameWorker()
{
    CLogger::getInstance()-> LogMessage("gameWorker destructor");
}

void gameWorker::process()
{
    CLogger::getInstance()->LogMessage("worker alive and running");

    
    // TODO : connect to server
    // TODO : stay in an infinite loop for communication
    // TODO : periodically check for message and dispatch as needed
    // TODO : use an action variable to exit from the loop and game.

    emit finished();
}