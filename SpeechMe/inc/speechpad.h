#ifndef SPEECHPAD_H
#define SPEECHPAD_H

#include <QWidget>
#include <Msrs.h>
#include <Observer.h>

namespace Ui {
    class SpeechPad;
}

class SpeechPad : public QWidget, public Observer
{
    Q_OBJECT

public:
    explicit SpeechPad(QWidget *parent = 0);
    ~SpeechPad();
    void setMsrs(Msrs* msrs);
    virtual void Update(Subject* subject);
    virtual void UpdateSentence(Subject* subject);

private:
    Ui::SpeechPad *ui;
    Msrs* msrs;
};

#endif // SPEECHPAD_H
