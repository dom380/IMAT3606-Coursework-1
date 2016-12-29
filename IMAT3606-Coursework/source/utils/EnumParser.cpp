#include <utils\EnumParser.h>
#include <Engine.h>
#include <utils\OnClickFunctions.h>

EnumParser<Engine::GraphicsContext>::EnumParser()
{
	map["OPEN_GL"] = Engine::GraphicsContext::OPEN_GL;
	map["open_gl"] = Engine::GraphicsContext::OPEN_GL;
}

EnumParser<EventType>::EnumParser()
{
	map["CLICK"] = EventType::CLICK;
	map["click"] = EventType::CLICK;
}

EnumParser<OnClickFunctions::FunctionType>::EnumParser()
{
	map["SWITCH_SCREEN"] = OnClickFunctions::FunctionType::SWITCH_SCREEN;
	map["switch_screen"] = OnClickFunctions::FunctionType::SWITCH_SCREEN;
	map["EXIT"] = OnClickFunctions::FunctionType::EXIT;
	map["exit"] = OnClickFunctions::FunctionType::EXIT;
	map["DO_NOTHING"] = OnClickFunctions::FunctionType::DO_NOTHING;
	map["do_nothing"] = OnClickFunctions::FunctionType::DO_NOTHING;
}