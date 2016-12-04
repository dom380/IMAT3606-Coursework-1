#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H
class Graphics
{
public:
	virtual void render() = 0;
	virtual bool init() = 0;
	virtual void exit() = 0;
};

#endif // !GRAPHICS_H
