#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                     // windows specific for va_list and friends
#include <time.h>

CLogger* CLogger::m_pThis = nullptr;

const char* defName = ".\\client.log";
const char* lpszMonths[]= {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const int  timeLen = 19;


CLogger* CLogger::getInstance(const char* filename)
{
    if (m_pThis == nullptr)
        m_pThis = new CLogger(filename);

    return m_pThis;

}

void CLogger::delInstance()
{
    if (m_pThis != nullptr)
        delete m_pThis;

}

   

int CLogger::LogMessage(const char* fmt, ...)
{
    int cntPrinted = 0;
    struct tm* tm;
    __time64_t long_time;

    char dtbuf[timeLen];
    memset((void*)&dtbuf, '/0', timeLen);
    
    // build date/time buffer: [dd-mmm-yyyy : hhmm]        20-character
    _time64(&long_time);                                  // Get time as 64-bit integer.
    tm = _localtime64(&long_time); 
    sprintf(dtbuf, "[%02d-%03s-%04d:%02d%02d] ", tm->tm_mday, lpszMonths[tm->tm_mon], 1900 + tm->tm_year, tm->tm_hour, tm->tm_min);
    
    va_list   va;
    va_start(va, fmt);

    int strlen = _vscprintf(fmt, va);                   // get space required for substitution
    char* mbuf = new char[strlen + 1];                  // allocate space for our message 
    char* buf = new char[strlen + timeLen + 1];         // allocate space for our final message
    cntPrinted = vsprintf(mbuf, fmt, va);      // construct log message
    
    strcpy(buf, dtbuf);
    strcat(buf, mbuf);

    
    //buf[cntPrinted] = '\0';

    va_end(va);
    fprintf(m_fp, "%s\n", buf);
    delete[] buf;
    return cntPrinted;
}


// These function are private so they can not be invoked by accident.
CLogger::CLogger(const char* filename) 
{ 
    m_fp = fopen(filename, "a");
    if (nullptr == m_fp)
    {
        // TODO : error happened... should not attempt anymore logging functions.
        // TODO : throw an exception here...
    }

}

CLogger::~CLogger()
{
    if (nullptr != m_fp)
        fclose(m_fp);
}


