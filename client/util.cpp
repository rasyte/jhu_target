#include "util.h"

#include <iostream>

#include <QByteArray>

void printBuffer(const char* buf, int len)
{
    std::cout << "[";
    for (int ndx = 0; ndx < len; ndx++)
    {
        std::cout << std::uppercase << std::showbase << std::hex << buf[ndx] << " ";
    }
    std::cout << "]" << std::endl;
}


void printQBA(QByteArray* pqba)
{
    std::cout << "[";
    for (char c : *pqba)
    {
        std::cout <<  std::uppercase << std::showbase << std::hex << c << " ";
    }
    std::cout << "]" << std::endl;
    
}