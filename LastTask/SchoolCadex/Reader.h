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

#include <iostream>
#include "SingletonFile.h"

cadex::Base_UTF16String StrToUTF(string str) {
	auto Cstr = static_cast<char*>(new char[str.length()]);
	for (int i = 0; i < str.length(); i++) 
		Cstr[i] = str[i];

	for (int i = 0; i < str.length(); i++) {
		cout << Cstr[i];
	}

	return Base_UTF16String(Cstr);
}

cadex::ModelData_SceneGraphElement ReadFromFile() {

	cadex::ModelData_SceneGraphElement Result;
	ifstream& in = SingletonReader::Create()->ReturnIfstream();

	// read parameters
	int type;
	in >> type;
	string name;
	int numberOfSubSGE;
	in >> name >> numberOfSubSGE;
	
	switch (type) {
	case 1: {
		
		cout << type << " " << name << " " << numberOfSubSGE << endl;
		cadex::ModelData_Assembly thatAssamble(StrToUTF(name));
		
		for (int i = 0; i <= numberOfSubSGE; i++)
			thatAssamble.AddInstance(ReadFromFile());

		// some code
		Result = static_cast<ModelData_SceneGraphElement> (thatAssamble);
		
		break;
	}
	case 2: {

		cout << type << " " << name << " " << numberOfSubSGE << endl;
		cadex::ModelData_Instance thatInstance(ReadFromFile(), StrToUTF(name));		
		// some code
		Result = static_cast<ModelData_SceneGraphElement> (thatInstance);

		break;
	}
	case 3: {


		cout << type << " " << name << " " << numberOfSubSGE << endl;
		cadex::ModelData_Assembly thatPart(StrToUTF(name));

		Result = static_cast<ModelData_SceneGraphElement> (thatPart);
		break;
		}
	}
	

	return static_cast<ModelData_SceneGraphElement>(Result);
}
