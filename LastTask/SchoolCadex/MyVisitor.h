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

#include <cadex/STL_Writer.hxx>
#include <iostream>
#include <fstream>

#include "SingletonFile.h"

using namespace cadex;
using namespace std;

string ToString(wstring wstr) {
	string result = "";
	for (auto iterator = 0; iterator < wstr.length(); iterator++) {
		result += static_cast<char>(wstr.at(iterator));
		// One of more problems
	}
	return result;
}


// 1 - Assamble, 2 - Instance, 3 - Part

class MyVisitor : public ModelData_Model::ElementVisitor {
private:
	string str;
public:
	MyVisitor() {		
		str = "";
	}

	void operator() (const ModelData_Part& thePart) {
		ofstream& out = SingletonWriter::Create()->ReturnOstream();
		out << "3 " << ToString(thePart.Name().ToWString()) << " 0 ";
	}
	 
	bool VisitEnter(const ModelData_Assembly& theAssembly) {
		int count = 0;

		ModelData_Model::ElementIterator EI(theAssembly);
		while (EI.HasNext()) {
			EI.Next();
			count++;
		}
		ofstream& out = SingletonWriter::Create()->ReturnOstream();
		out << "1 " << ToString(theAssembly.Name().ToWString()) << " " << count << " ";
		
		return true;
	}
	void VisitLeave(const ModelData_Assembly& theAssembly) {
		
	}
	bool VisitEnter(const ModelData_Instance& theInstance) {
	/*	ofstream& out = SingletonWriter::Create()->ReturnOstream();
		out << "2 Instance 1 ";
		*/
		return true;
	}
	void VisitLeave(const ModelData_Instance& theInstance) {
		
	}
};
