#pragma once
#ifndef ONCLICKFUNCTIONS_H
#define ONCLICKFUNCTIONS_H
#include <string>
using std::string;
#include <functional>
using std::function;
#include <Engine.h>


class OnClickFunctions {
public:
	static void switchScreen(Engine* engine, unsigned int screenId) {
		engine->switchScreen(screenId);
	}

	static void doNothing() {};

	static void exit(Engine* engine) {
		engine->exit();
	}

	//static shared_ptr<OnClickFunctions> getInstance() {
	//	if (!m_initialised) {
	//		m_instance = shared_ptr<OnClickFunctions>(new OnClickFunctions());
	//		//m_instance->intialiseFunctions();
	//		m_instance->initMap();
	//		m_initialised = true;
	//	}
	//	return m_instance;
	//};
	
	//template<typename... types>
	//std::function<void()> getFunction(string functionName, types... args) {
	//	auto type = stringToFuncMap.find(functionName);
	//	if (type == stringToFuncMap.end()) {
	//		//error handeling
	//		return NULL;
	//	}
	//	switch (type->second)
	//	{
	//	case FunctionType::SWITCH_SCREEN:
	//		auto params = std::make_tuple(args...);
	//		std::function<void(types...) func(switchScreen);
	//		return call_helper(func, params);
	//		return std::bind(switchScreen, params);
	//	};
	//};
	
	enum FunctionType
	{
		SWITCH_SCREEN,
		EXIT,
		DO_NOTHING
	};
	static FunctionType getFunctionType(string funcName) {
		if (funcName.compare("switch_screen") == 0 || funcName.compare("SWITCH_SCREEN") == 0) {
			return FunctionType::SWITCH_SCREEN;
		}
		else if (funcName.compare("exit") == 0 || funcName.compare("EXIT") == 0) {
			return FunctionType::EXIT;
		}
		else {
			return FunctionType::DO_NOTHING;
		}
	}

private:
	//OnClickFunctions() {};
	//OnClickFunctions(OnClickFunctions const&) {}; // prevent copies
	//void operator=(OnClickFunctions const&) {}; // prevent assignments
	//static shared_ptr<OnClickFunctions> m_instance;
	//static bool m_initialised;
	//map<string, FunctionType> stringToFuncMap;
	//void initMap() {
	//	stringToFuncMap.emplace(std::pair<string, FunctionType>("SWITCH_SCREEN",SWITCH_SCREEN));
	//	stringToFuncMap.emplace(std::pair<string, FunctionType>("switch_screen", SWITCH_SCREEN));
	//	stringToFuncMap.emplace(std::pair<string, FunctionType>("EXIT", EXIT));
	//	stringToFuncMap.emplace(std::pair<string, FunctionType>("exit", EXIT));
	//};

	
};

//shared_ptr<OnClickFunctions> OnClickFunctions::m_instance;
//bool OnClickFunctions::m_initialised = false;

#endif // !ONCLICKFUNCTIONS_H