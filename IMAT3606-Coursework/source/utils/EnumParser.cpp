#include <utils\EnumParser.h>
#include <Engine.h>
#include <utils\OnClickFunctions.h>
#include <Graphics\Camera.h>

EnumParser<Engine::GraphicsContext>::EnumParser()
{
	map["OPEN_GL"] = Engine::GraphicsContext::OPEN_GL;
	map["open_gl"] = Engine::GraphicsContext::OPEN_GL;
}

EnumParser<MouseEventType>::EnumParser()
{
	map["LEFT_CLICK"] = MouseEventType::LEFT_CLICK;
	map["left_click"] = MouseEventType::LEFT_CLICK;
	map["MOUSE_MOVE"] = MouseEventType::MOUSE_MOVE;
	map["mouse_move"] = MouseEventType::MOUSE_MOVE;
	map["RIGHT_CLICK"] = MouseEventType::RIGHT_CLICK;
	map["right_click"] = MouseEventType::RIGHT_CLICK;
	//map["KEY_PRESS"] = MouseEventType::KEY_PRESS;
	//map["key_press"] = MouseEventType::KEY_PRESS;
	//map["KEY_UP"] = MouseEventType::KEY_UP;
	//map["key_up"] = MouseEventType::KEY_UP;
}

EnumParser<OnClickFunctions::FunctionType>::EnumParser()
{
	map["SWITCH_SCREEN"] = OnClickFunctions::FunctionType::SWITCH_SCREEN;
	map["switch_screen"] = OnClickFunctions::FunctionType::SWITCH_SCREEN;
	map["REPLACE_SCREEN"] = OnClickFunctions::FunctionType::REPLACE_SCREEN;
	map["replace_screen"] = OnClickFunctions::FunctionType::REPLACE_SCREEN;
	map["EXIT"] = OnClickFunctions::FunctionType::EXIT;
	map["exit"] = OnClickFunctions::FunctionType::EXIT;
	map["DO_NOTHING"] = OnClickFunctions::FunctionType::DO_NOTHING;
	map["do_nothing"] = OnClickFunctions::FunctionType::DO_NOTHING;
}

EnumParser<Camera::CameraClass>::EnumParser()
{
	map["PERSPECTIVE"] = Camera::CameraClass::PERSPECTIVE;
	map["perspective"] = Camera::CameraClass::PERSPECTIVE;
	map["ORTHOGRAPHIC"] = Camera::CameraClass::ORTHOGRAPHIC;
	map["orthographic"] = Camera::CameraClass::ORTHOGRAPHIC;
}