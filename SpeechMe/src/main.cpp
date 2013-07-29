/*
============================================================================
 Name		: SpeechMe.cpp
 Author	  : Edwin Miguel Triana Gómez
 Copyright   : 
 Description : Main GUI Application
============================================================================
*/

#include "SpeechMe.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpeechMe w;
    w.showMaximized();
    return a.exec();
}
