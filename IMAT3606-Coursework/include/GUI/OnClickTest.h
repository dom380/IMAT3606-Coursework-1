#pragma once
#ifndef ONCLICKTEST_H
#define ONCLICKTEST_H
#include <iostream>

class OnClickTest {
public:
	void operator() () {
		std::cout << "Button Clicked!" << std::endl;
	};
};

#endif // !ONCLICKTEST_H
