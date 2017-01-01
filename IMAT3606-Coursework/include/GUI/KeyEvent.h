#pragma once
#ifndef KEYEVENT_H
#define KEYEVENT_H
#include "KeyEventType.h"

class KeyEvent
{
public:
	KeyEvent(KeyEventType eventType, int keyCode, int modifierCode)
	{
		type = eventType;
		key = keyCode;
		modifier = modifierCode;
	};
	KeyEventType type;
	int key;
	int modifier;

};

#endif // !KEYEVENT_H
