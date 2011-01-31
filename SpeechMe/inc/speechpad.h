#ifndef SPEECHPAD_H
#define SPEECHPAD_H

#include <QWidget>
#include <Observer.h>
#include "SpeechMe.h"

namespace Ui {
    class SpeechPad;
}

class SpeechPad : public QWidget, public Observer
{
    Q_OBJECT

public:
    explicit SpeechPad(QWidget *parent = 0);
    ~SpeechPad();
    virtual void Update(Subject* subject);
    virtual void UpdateSentence(Subject* subject);

private:
    Ui::SpeechPad *ui;
    SpeechMe* speechMe;
    bool localDecoding;

    void updateContButton();
    void decode(bool isolated);
    void enableButtons(bool opt);

  public slots:
    void on_decoder_configured(bool status);
    void on_isolatedButton_clicked();
    void on_contButton_clicked();
    
  private slots:
  	void on_new_sentence_ready(const QString*);
  	void on_update_decoder_status(bool status);
  	
  signals:
  	void newSentenceReady(const QString*);
  	void updateDecoderStatus(bool status);
};

#endif // SPEECHPAD_H
