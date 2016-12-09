#pragma once
#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H
#include "Event.h"

class EventListener {
public:
	virtual void  handle(Event event) = 0;
};
#endif // !EVENTLISTENER_H
