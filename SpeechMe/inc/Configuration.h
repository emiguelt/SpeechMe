/*
 * Configuration.h
 *
 *  Created on: 6/12/2010
 *      Author: Edwin Miguel
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>

using namespace std;


class Configuration
	{
private:
	string configFile;
	int serverPort;
public:
	Configuration();
	virtual ~Configuration();
	string getConfigFile();
    int getServerPort() const;
    void setConfigFile(string file);
    void setServerPort(int serverPort);
};

#endif /* CONFIGURATION_H_ */
