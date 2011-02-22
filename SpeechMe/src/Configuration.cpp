/*
 * Configuration.cpp
 *
 *  Created on: 6/12/2010
 *      Author: Edwin Miguel
 */

#include "Configuration.h"

Configuration::Configuration()
	{
	folder = "";
	jsgfModel = true;
	serverPort= 10000;
	samprateIndex = 0;
	samprate = 8000;
	}

Configuration::~Configuration()
	{
	}

string Configuration::getFolder(){
    return folder;
}

bool Configuration::isJsgfModel() const
    {
        return jsgfModel;
    }

int Configuration::getServerPort() const
    {
        return serverPort;
    }

int Configuration::getSampRate() const
    {
        return samprate;
    }

int Configuration::getSampRateIndex() const{
	return samprateIndex;
}

void Configuration::setSampRateIndex(int index){
	samprateIndex = index;
}
void Configuration::setFolder(string folder)
    {
        this->folder = folder;
    }

void Configuration::setJsgfModel(bool jsgfModel)
    {
        this->jsgfModel = jsgfModel;
    }

void Configuration::setServerPort(int serverPort)
    {
        this->serverPort = serverPort;
    }

void Configuration::setSampRate(int samprate)
    {
        this->samprate = samprate;
    }
