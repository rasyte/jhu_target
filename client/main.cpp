

#define WIN32_LEAN_AND_MEAN


#include <stdio.h>
#include <iostream>
#include <fstream>

#include <QApplication>
#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QThread>

#ifdef __WIN
#include "XGetopt.h"
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#endif

#include "mainWnd.h"
#include "loginDlg.h"
#include "logger.h"

const char* defLogFile = ".\\client_exec.log";
const char* version = "0.0.5";

void showUsage(const char*);
void showVersion(const char*);

void allocConsole();
bool initWinSock();
bool deinitWinsock();
int networkError();

// issues
// (1) modifying network setting are not persisited to the registry
// (2) gear-icon is not displayed on the log-in dialog, settings button
// (3) change socket code to support IPv4/IPv6
// (4) add error level to logger

enum g_errorLevel { DBG_DEBUG = 0, DBG_INFO, DBG_WARNING, DBG_ERROR, DBG_FATAL };
int g_dbgLevel = DBG_FATAL;

int main(int argc, char *argv[])
{
    int   nRet = 0;                                   // return value from main.
    char  c;
    char* logFile = nullptr;
    char* serverIP = nullptr;
    short sPort = -1;
    bool  bNoLogin = false;                           

    allocConsole();                                   // get a console up and running....
    
    QApplication theApp(argc, argv);                  
    QSettings  setting("JHUProj", "clue-less");       // getting settings, can be overridden by cmd line
                                                      // found at: HKEY_USERS\<user>\Software\JHUProj\clue-less
    // process command line arguments...
    while ((c = getopt(argc, argv, "dnl:s:p:hv")) != EOF)
    {
        switch (c)
        {
            case 'd':                                 // setting debug level
                g_dbgLevel -= 1;                
                if (g_dbgLevel < 0) g_dbgLevel = 0;
                break;
            case 'n':                                 // disable login for testing
                bNoLogin = true;
                break;
            case 'l':                                 // setting custom log location
                logFile = new char[strlen(optarg)+1];
                memset((void*)logFile, '\0', strlen(optarg) + 1);
                strcpy(logFile, optarg);
                break;
            case 'h':                                 // displaying help and exiting
                showUsage(argv[0]);
                exit(0);
            case 'v':                                 // displaying version and information
                showVersion(argv[0]);
                exit(0);
            case 's':                                 // define server IP address on command line
                serverIP = new char[strlen(optarg) + 1];
                memset((void*)serverIP, '\0', strlen(optarg) + 1);
                strcpy(serverIP, optarg);
                break;
            case 'p':
                sPort = atoi(optarg);
                break;
            case '?':                                 // unknown switch or error
            default:                                  // fall-through is expected
                fprintf(stderr, "unexpected flag %c", c);
                showUsage(argv[0]);
        }
    }


    // set up logging, placed here incase default logging name was changed on cmd-line.
    if (nullptr == logFile) logFile = const_cast<char*>(defLogFile);
    CLogger::getInstance(logFile);                    // get logging up and running .....
    CLogger::getInstance()->LogMessage("Client starting up");

    if (initWinSock())                                       // insure WinSock is setup on Windows
    {
        loginDlg  dlg(nullptr, &setting);
        if (bNoLogin || QDialog::Accepted == dlg.exec())     // display login dialog
        {
            QString qstrUid = dlg.getUid();
            QString qstrPwd = dlg.getPwd();

            mainWnd gameWnd(qstrUid, qstrPwd);               // main game window 
            // TODO : clear cred data here...
            gameWnd.show();
            nRet = theApp.exec();
        }
        else
        {
            CLogger::getInstance()->LogMessage("User cancelled, exitting\n");
        }
    }
    else
    {
        CLogger::getInstance()->LogMessage("Can not initialize WinSock");
    }

    deinitWinsock();


    CLogger::delInstance();

    return nRet;
}



// dnl:hv
void showUsage(const char* name)
{
    std::cout << name << "A client for the clue-less game.    " << std::endl;
    std::cout << "Usage: " << name << "[options]              " << std::endl;
    std::cout << "options:                                    " << std::endl;
    std::cout << "d          increase the logging output,     " << std::endl;
    std::cout << "           can be used five times           " << std::endl;
    std::cout << "n          bypassing login into server      " << std::endl;
    std::cout << "l <file>   use <file> as custom logging file" << std::endl;
    std::cout << "           instead of .\\client_exec.log    " << std::endl;
    std::cout << "h          display this screen and exit     " << std::endl;
    std::cout << "v          show version info and exit       " << std::endl;
}



void showVersion(const char* name)
{
    std::cout << name << "A client for the clue-less game.    " << std::endl;
    std::cout << "Version: " << version << std::endl;
}



// printing debugging information, status information is common and helpful...
// except if you are brain-dead idiots from Microsoft who disable this stuff
// So, lets fix this.  (taken from Adding Console I/O to a Win32 GUI App,
// Windows Developer Journal, Dec 1997; and updated from 
// https://smacdo.com/programming/redirecting-standard-output-to-console-in-windows/)
//
void allocConsole()
#ifdef __WIN                 // code for Windows...
{
    // Create the console window and set the window title.
    if (AllocConsole() == 0)
    {
        // Handle error here. Use ::GetLastError() to get the error.
    }

    // Redirect CRT standard input, output and error handles to the console window.
    FILE * pNewStdout = nullptr;
    FILE * pNewStderr = nullptr;
    FILE * pNewStdin = nullptr;

    ::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
    ::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);
    ::freopen_s(&pNewStdin, "CONIN$", "r", stdin);

    // Clear the error state for all of the C++ standard streams. Attempting to accessing the streams before they refer
    // to a valid target causes the stream to enter an error state. Clearing the error state will fix this problem,
    // which seems to occur in newer version of Visual Studio even when the console has not been read from or written
    // to yet.

    std::cout.clear();
    std::cerr.clear();
    std::cin.clear();

    std::wcout.clear();
    std::wcerr.clear();
    std::wcin.clear();
}
#else                              // code for Linux...wait there is none, Linux is not brain-dead
{

}
#endif


bool initWinSock()
{
    bool      bRet = false;
#ifdef __WIN
    WORD      wVerReq;
    int       nRet = 0;

    wVerReq = MAKEWORD(2, 2);
    WSADATA   wsaData;

    if (0 == (nRet = WSAStartup(wVerReq, &wsaData)))   // return zero on success...only MS would return false on success
    {
        if ((LOBYTE(wsaData.wVersion) == 2) && (HIBYTE(wsaData.wVersion) == 2))
        {
            bRet = true;
        }
        else
        {
            std::cout << "Unable to find acceptable winsock dll" << std::endl;
            WSACleanup();
        }
    }
    else
    {
        std::cout << "WSAFailed, error is: " << nRet << std::endl;
    }
#else
    bRet = true;
#endif

    return bRet;
}


bool deinitWinsock()
{
    bool  bRet = false;

#ifdef __WIN
    if (0 == WSACleanup())
    {
        bRet = true;
    }
    else
    {
        CLogger::getInstance()->LogMessage("Failed to cleanup WSACleanup, %d\n", WSAGetLastError());
        bRet = false;
    }
#else                 
    bRet = true;       // stub-out function on Linux,                   
#endif

    return bRet;
}


int networkError()
{
    int nRet = 0;

#ifdef __WIN
    nRet = WSAGetLastError();
#else
    nRet = errno;
#endif

    return nRet;
}
