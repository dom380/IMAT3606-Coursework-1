#pragma once
#ifndef EVENT_H
#define EVENT_H
#include "EventType.h"

class Event {
public:
	Event(EventType type, int posX, int posY) {
		x = posX;
		y = posY;
		eventType = type;
	};
	EventType eventType;
	int x, y;
};

#endif // !EVENT_H
