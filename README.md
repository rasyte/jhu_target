# Clue_Dunit_Client_Server

## Initial Instructions (Work in Progress)

1. Download Visual Studio (for Mac) (or the Windows version) (NOT Visual Studio Code)
2. Navigate to solution "clueless.sln" and open the file (with Visual Studio Code)
2. Select project > build to build the client and server.


Notes from George: "There is a solution file at the top level (clueless.sln) use Visual Studio 2017 to open the solution, and the select project->build or ctrl+shift B (if you left the key bindings at default)  This will build both the client and the server.  However, because Microsoft can't touch something without breaking it (sorry Rasa), the server will not run on Windows.  The included make file will allow the server to be build and run on Linux.  Note 1: the server is a command line. Note 2: While it will build with cygwin, I do not believe it will run on Windows. 7:48 The issue is the Microsoft choose to not allow 'select' to be able to multiplex on the command line, and I use this to be able to control the server when it is running."