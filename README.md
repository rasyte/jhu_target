# Clue_Dunit_Client_Server

## Prerequisites
1. Windows 10 (or Windows 7+)
2. Download Microsoft Visual Studio Community 2017 (Visual Studio Community 2017 (version 15.9) (x86 and x64): https://my.visualstudio.com/Downloads?q=visual%20studio%202017&wt.mc_id=o~msft~vscom~older-downloads
    * Make sure you have selected "VC++ 2017 version 15.9 v14.16 latest v141 tools"
    * Make sure you have selected "Windows 10 SDK 10.0.17134.0"
3. Download Qt 5.12.3 with the MSVC options selected: https://www.qt.io/download-qt-installer?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4
4. Setup Microsoft Visual Studio Community 2017 to use "msvc2017_64"
    * In Visual Studio look for the menu "QT VS Tools" And navigae to QT Options > Qt Versions > Name: "msvc2017_64"
5. Ubuntu 16.04 & g++ version 5.4.0

## Note: George's GitHub repo:
https://github.com/thurizas/jhu_project

## Initial Instructions (Work in Progress)

1. Open Visual Studio Community 2017, navigate to solution "clueless.sln" and open the file (with Visual Studio Code)
2. Select project > build to build the client and server.


Notes from George: "There is a solution file at the top level (clueless.sln) use Visual Studio 2017 to open the solution, and the select project->build or ctrl+shift B (if you left the key bindings at default)  This will build both the client and the server.  However, because Microsoft can't touch something without breaking it (sorry Rasa), the server will not run on Windows.  The included make file will allow the server to be build and run on Linux.  Note 1: the server is a command line. Note 2: While it will build with cygwin, I do not believe it will run on Windows. 7:48 The issue is the Microsoft choose to not allow 'select' to be able to multiplex on the command line, and I use this to be able to control the server when it is running."

## Launch the Client
1. Navigate to the client build, under <repository_folder>/x64/Debug/client.exe
2. OR Launch the client from the command line, navigate to the project folder, run as "client.exe" (OR enter the IP address at the command line "client.exe -d -n -s <server_IP_address> -p 1337") 

## Launch the Server
### Had connection issues running a VM inside Windows 10
1. Download the Ubuntu Terminal application from the Microsoft Store: https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6?activetab=pivot:overviewtab
2. Download this repository and switch to "minimal"
3. Run "make" in the server directory to build the executable
4. Run the executable: $ ./clserver
5. Check if it's up and running by entering in one of the keyboard commands