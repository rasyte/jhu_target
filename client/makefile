QT_DIR=/opt/QT/Qt-5.12.3/5.12.3/gcc_64

CC=g++
CCFLAGS=-std=c++11 -pedantic -Wall -fPIC
CCINC=-I $(QT_DIR)/include -I $(QT_DIR)/include/QtCore -I $(QT_DIR)/include/QtGui -I $(QT_DIR)/include/QtWidgets

LK=g++
LKFLAGS=-L $(QT_DIR)/lib -lQt5Core -lQt5Gui -lQt5Widgets

UIC=$(QT_DIR)/bin/uic
MOC=$(QT_DIR)/bin/moc
RCC=$(QT_DIR)/bin/rcc

PROJ=clclient

OBJS=gameWorker.o logger.o loginDlg.o main.o mainWnd.o settingDlg.o qrc_mainWnd.o
MOCS=moc_mainWnd.o moc_loginDlg.o moc_gameWorker.o


$(PROJ) : $(OBJS)
	(LK) $(LKFLAGS) $(OBJS) -o $(PROJ)

all : clean $(PROJ)

gameWorker.o : gameWorker.cpp gameWorker.h
	$(CC) -c $(CCFLAGS) $(CCINC) gameWorker.cpp -o gameWorker.o

moc_gameWorker.o : gameWorker.h
	$(MOC) gameWorker.h -o moc_gameWorker.cpp
	$(CC) -c $(CCFLAGS) $(CCINC) moc_gameWorker.cpp -o moc_gameWorker.o

logger.o : logger.cpp logger.h
	$(CC) -c $(CCFLAGS) $(CCINC) logger.cpp -o logger.o

loginDlg.o : loginDlg.cpp loginDlg.h
	$(CC) -c $(CCFLAGS) $(CCINC) loginDlg.cpp -o loginDlg.o

moc_loginDlg.o : loginDlg.h
	$(MOC) loginDlg.h -o moc_loginDlg.cpp
	$(CC) -c $(CCFLAGS) $(CCINC) moc_loginDlg.cpp -o moc_loginDlg.o

main.o : main.cpp
	$(CC) -c $(CCFLAGS) $(CCINC) main.cpp -o main.o

mainWnd.o : mainWnd.cpp mainWnd.h
	$(CC) -c $(CCFLAGS) $(CCINC) mainWnd.cpp -o mainWnd.o

moc_mainWnd.o : mainWnd.h
	$(MOC) mainWnd.h -o moc_mainWnd.cpp
	$(CC) -c $(CCFLAGS) $(CCINC) moc_mainWnd.cpp -o moc_mainWnd.o

settingDlg.o : settingDlg.cpp settingDlg.h
	$(CC) -c $(CCFLAGS) $(CCINC) settingDlg.cpp -o settingDlg.o

moc_settingDlg.o : settingDlg.h
	$(MOC) settingDlg.h -o moc_settingDlg.cpp
	$(CC) -c $(CCFLAGS) $(CCINC) moc_settingDlg.cpp -o moc_settingDlg.o
	
mainWnd.o : mainWnd.qrc
	$(RCC) -name mainWnd mainWnd.qrc -o mainWnd.cpp	
	$(CC) -c $(CCFLAGS) $(CCINC) mainWnd.cpp -o mainWnd.o
	
clean :
	rm -f *.o
	rm -f moc_*
	rm -f qrc_*
	rm -f *.*~
	rm -f $(PROJ)

