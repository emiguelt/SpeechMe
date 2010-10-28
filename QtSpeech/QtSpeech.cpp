/*
============================================================================
 Name		: QtSpeech.cpp
 Author	  : Miguel
 Copyright   : 
 Description : Class Implementation
============================================================================
*/
#include <qfiledialog.h>
#include <qstring.h>
#include "QtSpeech.h"
#include <iostream>
using namespace std;

QtSpeech::QtSpeech(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	dc = new DecoderControl(this);
	
	connect(ui.hmmBrowseButton, SIGNAL(clicked()), this, SLOT(browseHmm()));
	connect(ui.lmBrowseButton, SIGNAL(clicked()), this, SLOT(browseLm()));
	connect(ui.dictBrowseButton, SIGNAL(clicked()), this, SLOT(browseDict()));
	connect(ui.decodingButton, SIGNAL(clicked()), this, SLOT(startDecoding()));
}

QtSpeech::~QtSpeech()
{

}

void QtSpeech::browseHmm(){
	QFileDialog* qfd = new QFileDialog;
	ui.hmm->setText(qfd->getExistingDirectory(this, "Hmm folder", "c:\\", QFileDialog::ShowDirsOnly));
	delete qfd;
}
void QtSpeech::browseLm(){
	QFileDialog* qfd = new QFileDialog;
		ui.language->setText(qfd->getOpenFileName(this, "LM file", "c:\\", "*"));
		delete qfd;
}
void QtSpeech::browseDict(){
	QFileDialog* qfd = new QFileDialog;
		ui.dict->setText(qfd->getOpenFileName(this, "Dict file", "c:\\", "*"));
		delete qfd;
}

void QtSpeech::startDecoding(){
	string  hmm = ui.hmm->text().toStdString();
	string  lm = ui.language->text().toStdString();
	string  dict = ui.dict->text().toStdString();
	dc->initDecoder(hmm,lm,dict);
	
}

void QtSpeech::addSentence(const char* sentence){
	QString newSentence(sentence);
	ui.plainTextEdit->appendPlainText(newSentence);
}
