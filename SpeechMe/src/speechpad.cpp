#include "inc\speechpad.h"
#include "ui_speechpad.h"
#include <Observer.h>

SpeechPad::SpeechPad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeechPad)
{
    ui->setupUi(this);
}

SpeechPad::~SpeechPad()
{
    delete ui;
}

void SpeechPad::Update(Subject* subject){
}

void SpeechPad::UpdateSentence(Subject* subject){
	const QString* sent = new QString(msrs->getLastSentence());
	ui->textSP->appendPlainText(*sent);
	ui->textSP->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void SpeechPad::setMsrs(Msrs* msrs){
	this->msrs=msrs;
	msrs->Attach(this);
}
