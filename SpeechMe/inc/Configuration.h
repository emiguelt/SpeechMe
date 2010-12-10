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
	string folder;
	bool jsgfModel;
	int serverPort;
public:
	Configuration();
	virtual ~Configuration();
	string getFolder();
    bool isJsgfModel() const;
    int getServerPort() const;
    void setFolder(string folder);
    void setJsgfModel(bool jsgfModel);
    void setServerPort(int serverPort);

};

#endif /* CONFIGURATION_H_ */
