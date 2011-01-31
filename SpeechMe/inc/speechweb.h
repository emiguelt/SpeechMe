#ifndef SPEECHWEB_H
#define SPEECHWEB_H

#include <QWidget>
#include <Observer.h>
#include "SpeechMe.h"

namespace Ui {
    class SpeechWeb;
}

class SpeechWeb : public QWidget, public Observer
{
    Q_OBJECT

public:
    explicit SpeechWeb(QWidget *parent = 0);
    ~SpeechWeb();
    virtual void Update(Subject* subject);
    virtual void UpdateSentence(Subject* subject);

private:
    Ui::SpeechWeb *ui;
    SpeechMe* speechMe;
    
public slots:
    void on_backButton_clicked();
    void on_speechButton_clicked();
    
private slots:
    void on_sentence_arrived(const char * sentence);
    void on_update_decoder_status(bool status);
    
signals:
	void sentenceArrived(const char * sentence);
	void updateDecoderStatus(bool status);
};

#endif // SPEECHWEB_H
