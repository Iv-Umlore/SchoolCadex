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

#include <cadex/ModelAlgo_TopoPrimitives.hxx>

#include <iostream>
#include "SingletonFile.h"
#include "MyVisitor.h"

const bool All_Write = false;
const bool Assamble_Write = false;
const bool Instance_Write = false;
const bool Part_Write = false;

// Convert string to CadexFormat(UTF16String)
cadex::Base_UTF16String StrToUTF(string str) {
	wstring Cstr;
	Cstr.resize(str.length());
	for (int i = 0; i < str.length(); i++) {
		Cstr[i] = static_cast<wchar_t>(str[i]);
	}
	Base_UTF16String res(Cstr);
	return res;
}


cadex::ModelData_SceneGraphElement ReadFromFile() {

	cadex::ModelData_SceneGraphElement Result;
	ifstream& in = SingletonReader::Create()->ReturnIfstream();

	// read parameters
	int type;
	string name;
	int numberOfSubSGE;
	in >> type >> name >> numberOfSubSGE;
	
	if (All_Write) cout << endl << type << " " << name << " " << numberOfSubSGE << endl;
	
	switch (type) {
	case 1: {
		if (Assamble_Write) cout << type << " " << name << " " << numberOfSubSGE << endl;
		cadex::ModelData_Assembly thatAssamble(StrToUTF(name));
		for (int i = 0; i < numberOfSubSGE; i++) {
			cadex::ModelData_SceneGraphElement SGE = ReadFromFile();
			thatAssamble.AddInstance(SGE);
			
		}

		Result = thatAssamble;
		
		break;
	}
	case 2: {
		if (Instance_Write) cout << type << " " << name << " " << numberOfSubSGE << endl;
		cadex::ModelData_SceneGraphElement Elem = ReadFromFile();
		string instance = "Instance";
		if (name == instance) name = ToString(Elem.Name().ToWString());
		cadex::ModelData_Instance thatInstance(Elem,StrToUTF(name));	
		Result = thatInstance;

		break;
	}
	case 3: {
		if (Part_Write) cout << type << " " << name << " " << numberOfSubSGE << endl;

		ModelAlgo_TopoPrimitives tmp;
		ModelData_Solid MDS = tmp.CreateBox(1.0, 1.0, 1.0);
		ModelData_BRepRepresentation BRep(MDS);
		cadex::ModelData_Part thatPart(BRep,StrToUTF(name));

		Result = thatPart;
		break;
		}
	default: {
		cerr << "Attention, i have a problem!!!\n";
		cout << type << endl;
		break;
	}
	}	

	return Result;
}
