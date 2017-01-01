#pragma once
#ifndef EVENT_H
#define EVENT_H
#include "MouseEventType.h"

class MouseEvent {
public:
	MouseEvent(MouseEventType type, int posX, int posY) {
		x = posX;
		y = posY;
		eventType = type;
	};
	MouseEventType eventType;
	int x, y;
};

#endif // !EVENT_H
