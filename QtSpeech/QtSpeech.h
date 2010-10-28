/*
============================================================================
 Name		: QtSpeech.cpp
 Author	  : Miguel
 Copyright   : 
 Description : Declares Class
============================================================================
*/

#ifndef QTSPEECH_H
#define QTSPEECH_H

#include <QtGui/QMainWindow>
#include "ui_QtSpeech.h"
#include "DecoderControl.h"

class DecoderControl;
class QtSpeech : public QMainWindow
{
    Q_OBJECT

public:
	QtSpeech(QWidget *parent = 0);
    ~QtSpeech();
    void addSentence(const char * sentence);

private:
    Ui::QtSpeech ui;
    DecoderControl* dc;
    
public slots:
	void browseHmm();
	void browseLm();
	void browseDict();
	void startDecoding();
};

#endif // QTSPEECH_H
