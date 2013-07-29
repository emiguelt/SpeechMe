/*
 * Configuration.cpp
 *
 *  Created on: 6/12/2010
 *      Author: Edwin Miguel
 */

#include "Configuration.h"

Configuration::Configuration()
	{
	configFile = "";
	serverPort= 10000;
	}

Configuration::~Configuration()
	{
	}

string Configuration::getConfigFile(){
    return configFile;
}

int Configuration::getServerPort() const
    {
        return serverPort;
    }


void Configuration::setConfigFile(string file)
    {
        configFile = file;
    }

void Configuration::setServerPort(int serverPort)
    {
        this->serverPort = serverPort;
    }
