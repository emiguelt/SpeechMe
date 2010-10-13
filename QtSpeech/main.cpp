/*
============================================================================
 Name		: QtSpeech.cpp
 Author	  : Miguel
 Copyright   : 
 Description : Main GUI Application
============================================================================
*/

#include "QtSpeech.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSpeech w;
    w.showMaximized();
    return a.exec();
}
