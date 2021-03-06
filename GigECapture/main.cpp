#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <windows.h>

#include <QApplication>
#include <QSizePolicy>
#include "mainwindow.h"
#include <QThreadPool>

void Console()
{
	AllocConsole();
	FILE *pFileCon = NULL;
	pFileCon = freopen("CONOUT$", "w", stdout);

	COORD coordInfo;
	coordInfo.X = 130;
	coordInfo.Y = 9000;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
}


int main(int argc, char *argv[])
{
	QThreadPool *threadPool = QThreadPool::globalInstance();
	Console();

	QApplication a(argc, argv);
	MainWindow *w = new MainWindow;

	return a.exec();
}