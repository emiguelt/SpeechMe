/*
 * Subject.cpp
 *
 *  Created on: 25/10/2010
 *      Author: Edwin Miguel
 */

#include "Observer.h"
#include <stlport/iostream>
#include <stlport/vector>


Subject::Subject()
	{
	}

Subject::~Subject()
	{
	}

void Subject::Attach (Observer* o) { 
 _observers.push_back(o); 
} 

void Subject::Detach (Observer* o) { 
 int count = _observers.size(); 
 int i; 

 for (i = 0; i < count; i++) { 
   if(_observers[i] == o) 
   break; 
 } 
 if(i < count) 
  _observers.erase(_observers.begin() + i); 

} 

void Subject::Notify () { 
 int count = _observers.size(); 

 for (int i = 0; i < count; i++) 
   (_observers[i])->Update(this); 
}

void Subject::NotifyNewSentece(){ 
 int count = _observers.size(); 

 for (int i = 0; i < count; i++) 
   (_observers[i])->UpdateSentence(this); 
}
