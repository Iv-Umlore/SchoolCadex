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

class SingletonReader {
private:
	//static SingletonWriter *ptr;
	std::ifstream* FOut;

	SingletonReader() {}
public:
	static SingletonReader* Create() {
		static SingletonReader instance;
		return &instance;
	}

	void SetIfstream(std::ifstream & MyOut) {
		FOut = &MyOut;
	}

	std::ifstream& ReturnIfstream() {
		return *FOut;
	}
};
