#pragma once

class SimpleClass {
private:
	int _numb;
public:
	SimpleClass( int numb = 10) {
		_numb = numb;
	}

	int GetValue() {
		return _numb;
	}

	void SetValue(int new_numb) {
		_numb = new_numb;
	}

	~SimpleClass() {
		cout << "Goodbye \n";
	}

};
