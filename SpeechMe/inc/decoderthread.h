#ifndef DECODERTHREAD_H
#define DECODERTHREAD_H

#include <QThread>

class DecoderThread : public QThread
{
    Q_OBJECT
private:
  bool isolated;
public:
    explicit DecoderThread(QObject *parent = 0);
    void run();
    void setIsolated(bool opt);
    bool isIsolated();

signals:

public slots:

};

#endif // DECODERTHREAD_H
