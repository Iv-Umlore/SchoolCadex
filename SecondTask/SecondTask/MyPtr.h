#pragma once

#include <memory>

using namespace std;

template <class T>
class MyPtr {
private:
	T* ptr;

public:

	MyPtr(T* pointer = nullptr) {
		ptr = pointer;
	}

	// This overload allows you to pass the value to another pointer. (c) Google Translate ))
	void operator=(MyPtr old) {
		cout << "It's imposible" << endl;
	}
	
	T* operator->() {
		if (ptr == nullptr) throw 1;
		return ptr;
	}					 
			
	void SetValue(T* point) {
		ptr = point;
	}

	void Move(MyPtr<T>* old) {
		ptr = old->ptr;
		old->ptr = nullptr;
	}

	~MyPtr() { 
		ptr->~T();
		ptr = nullptr;
		delete ptr; 
	}
};
