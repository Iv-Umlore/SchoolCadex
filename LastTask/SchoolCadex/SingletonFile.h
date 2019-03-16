#pragma once

#include <iostream>

using namespace std;

class SingletonWriter {
private:
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
	std::ifstream* FInput;

	SingletonReader() {}
public:
	static SingletonReader* Create() {
		static SingletonReader instance;
		return &instance;
	}

	void SetIfstream(std::ifstream & MyInput) {
		FInput = &MyInput;
	}

	std::ifstream& ReturnIfstream() {
		return *FInput;
	}
};
