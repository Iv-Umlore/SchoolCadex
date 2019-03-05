
#include <iostream>
#include "MyPtr.h"
#include "SimpleClass.h"

int main() {

	unique_ptr<SimpleClass> test(new SimpleClass(100));

	SimpleClass SC(15);
	MyPtr<SimpleClass> point(new SimpleClass(50));

	MyPtr<SimpleClass> temp;

	cout << "It's class: " << SC.GetValue() << endl;

	MyPtr<SimpleClass> temp2;
	temp2.SetValue(&SC);
	cout << "It's MyPointer to class: " << SC.GetValue() << endl;

	cout << "It's pointer with constructor: " << point->GetValue() << endl;
	
	cout << "It's Unique: " << test->GetValue() << endl;
	
	try {
		cout << "It's empty pointer: " << temp->GetValue();
	}
	catch (int i) {
		if (i == 1) cout << "\nSorry, friend. It's empty pointer (( \n";
	}
	// operations: 
	temp.Move(&point); 

	cout << "It's removed value: " << temp->GetValue() << endl;
	try {
		cout << "It's old pointer: " << point->GetValue() << endl;
	}
	catch (int i) {
		if (i == 1) cout << "\nSorry, friend. It's empty pointer (( \n";
	}

	system("pause");

	return 0;
}
