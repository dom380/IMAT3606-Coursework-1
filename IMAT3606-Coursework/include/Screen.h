#pragma once
#ifndef SCREEN_H
#define SCREEN

class Screen {
public:
	virtual void render(float dt) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void dispose() = 0;
	void setID(unsigned int id) {
		screenId = id;
	};
	unsigned int getID() {
		return screenId;
	}
protected:
	unsigned int screenId;
};

#endif // !SCREEN_H

