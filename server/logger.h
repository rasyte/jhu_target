#pragma once

#include <stdio.h>
#include <stdlib.h>

class CLogger
{
public:
    static CLogger* getInstance(const char* filename = nullptr);
    static void delInstance();
    int LogMessage(const char* fmt, ...);

private:
    CLogger(const char*);
    ~CLogger();
    CLogger(const CLogger&);
    CLogger& operator=(const CLogger&);

    static CLogger*  m_pThis;
    FILE*            m_fp;
};
