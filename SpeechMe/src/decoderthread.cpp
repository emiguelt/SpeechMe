#include "inc/decoderthread.h"
#include "Msrs.h"

DecoderThread::DecoderThread(QObject *parent) :
    QThread(parent)
{
}

void DecoderThread::run(){
  Msrs::getInstance()->startLiveDecoding(isIsolated());
}

bool DecoderThread::isIsolated(){
  return isolated;
}

void DecoderThread::setIsolated(bool opt){
  isolated = opt;
}
