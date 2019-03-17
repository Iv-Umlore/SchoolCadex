#pragma once

#include <cadex/Base_ProgressScope.hxx>
#include <cadex/LicenseManager_Activate.h>
#include <cadex/ModelData_Body.hxx>
#include <cadex/ModelData_BodyList.hxx>
#include <cadex/ModelData_BRepRepresentation.hxx>
#include <cadex/ModelData_Direction.hxx>
#include <cadex/ModelData_Face.hxx>
#include <cadex/ModelData_Model.hxx>
#include <cadex/ModelData_Model.hxx>
#include <cadex/ModelData_OffsetSurface.hxx>
#include <cadex/ModelData_Part.hxx>
#include <cadex/ModelData_RectangularTrimmedSurface.hxx>
#include <cadex/ModelData_Shape.hxx>
#include <cadex/ModelData_Surface.hxx>
#include <cadex/STEP_Reader.hxx>
#include <cadex/STEP_ReaderParameters.hxx>
#include <cadex/ModelAlgo_BRepMesher.hxx>

#include <cadex/ModelAlgo_BRepMesherParameters.hxx>
#include <cadex/MeshAlgo_NetgenFactory.hxx>
#include <cadex/ModelData_PolyRepresentation.hxx>

#include <cadex/ModelData_Instance.hxx>
#include <cadex/ModelData_Assembly.hxx>
#include <cadex/ModelData_Part.hxx>

#include <cadex/ModelData_Model.hxx>
#include <iostream>
#include <fstream>

#include "SingletonFile.h"

using namespace cadex;
using namespace std;

string ToString(wstring wstr) {
	string result = "";
	for (auto iterator = 0; iterator < wstr.length(); iterator++) {
		if (static_cast<char>(wstr.at(iterator)) == ' ') result += '_';
		else result += static_cast<char>(wstr.at(iterator));
	}
	return result;
}


// 1 - Assamble, 2 - Instance, 3 - Part

class MyVisitor : public ModelData_Model::ElementVisitor {
public:
	MyVisitor() {	
	}

	void operator() (const ModelData_Part& thePart) {
		ofstream& out = SingletonWriter::Create()->ReturnOstream();
		string name = ToString(thePart.Name().ToWString());
		if (name.size() == 0) {
			name = "part";
		}
		out << "3 " << name << " 0 ";
	}
	 
	bool VisitEnter(const ModelData_Assembly& theAssembly) {
		int count = 0;

		string name = ToString(theAssembly.Name().ToWString());
		if (name.size() == 0) {
			name = "Assembly";
		}

		ModelData_Model::ElementIterator EI(theAssembly);
		while (EI.HasNext()) {
			EI.Next();
			count++;
		}
		ofstream& out = SingletonWriter::Create()->ReturnOstream();
		out << "1 " << name << " " << count << " ";
		return true;
	}
	void VisitLeave(const ModelData_Assembly& theAssembly) {
		
	}
	bool VisitEnter(const ModelData_Instance& theInstance) {
		ofstream& out = SingletonWriter::Create()->ReturnOstream();

		string name = ToString(theInstance.Name().ToWString());
		if (name.size() == 0) {
			name = "Instance";
		}
		out << "2 "<< name << " 1 ";
		return true;
	}
	void VisitLeave(const ModelData_Instance& theInstance) {
		
	}
};

// need for test Model structure
class TestVisitor : public ModelData_Model::ElementVisitor {
public:
	TestVisitor() {};

	void operator() (const ModelData_Part& thePart) {
		cout << "It's Part: " << ToString(thePart.Name().ToWString()) << endl;
	}

	bool VisitEnter(const ModelData_Assembly& theAssembly) {
		cout << "It's Assembly: " << ToString(theAssembly.Name().ToWString());
		ModelData_Model::ElementIterator EI(theAssembly);
		int count = 0;
		while (EI.HasNext()) {
			count++;
			EI.Next();
		}
		cout << " his SubSGEcount: " << count << endl;

		return true;
	}
	void VisitLeave(const ModelData_Assembly& theAssembly) {

	}
	bool VisitEnter(const ModelData_Instance& theInstance) {
		cout << "It's Instance: " << ToString(theInstance.Name().ToWString()) << endl;
		return true;
	}
	void VisitLeave(const ModelData_Instance& theInstance) {

	}
};
