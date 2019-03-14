#pragma once

#include <iostream>

using namespace std;

class SingletonWriter {
private:
	//static SingletonWriter *ptr;
	std::ofstream* FOut;

	SingletonWriter() {}
public:
	static SingletonWriter* Create() {
		static SingletonWriter instance;
		return &instance;
	}

	void SetOstream(std::ofstream & MyOut) {
		FOut = &MyOut;
	}

	std::ofstream& ReturnOstream() {
		return *FOut;
	}
};
