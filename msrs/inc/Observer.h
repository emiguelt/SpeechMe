/*
 * Subject.h
 *
 *  Created on: 25/10/2010
 *      Author: Edwin Miguel
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <iostream>
#include <vector>

class Observer;
class Subject
	{
public:
	virtual ~Subject();
	
	virtual void Attach(Observer *o);
	virtual void Detach(Observer *o);
	virtual void Notify();
	virtual void NotifyNewSentece();
protected:
	Subject();
private:
	std::vector<Observer*> _observers;
	};

class Observer{
public:
	virtual ~Observer(){};
	virtual void Update(Subject *subject)=0;
	virtual void UpdateSentence(Subject *subject)=0;
protected:
	Observer(){};
};

#endif /* OBSERVER_H_ */
