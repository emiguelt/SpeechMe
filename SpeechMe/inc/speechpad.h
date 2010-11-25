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

    void updateContButton();

  public slots:
    void on_decoder_configured(bool status);
    void on_isolatedButton_clicked();
    void on_contButton_clicked();
};

#endif // SPEECHPAD_H
