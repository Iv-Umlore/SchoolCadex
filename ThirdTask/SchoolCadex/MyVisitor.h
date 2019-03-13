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

using namespace cadex;
using namespace std;

class MyVisitor : public ModelData_Model::ElementVisitor {
private:
	char k;
	char* aDest = new char[12];
	STL_Writer aWriter;
	ModelAlgo_BRepMesherParameters aParam;
	MeshAlgo_NetgenFactory aMF;
	MeshAlgo_NetgenFactory::Parameters aP;
public:
	MyVisitor() {
		k = 'a';
		strcpy(aDest, "p/Part_0.stl");		
		aP.SetGranularity(MeshAlgo_NetgenFactory::Parameters::Fine);
		aMF.SetParameters(aP);
		aParam.ComputationalMeshAlgo() = aMF;
		ModelAlgo_BRepMesher aMesher(aParam);
	}

	void operator() (const ModelData_Part& thePart) {
		
		auto UTFStr = new Base_UTF16String(aDest);

		for (int i = 0; i < 12; i++) {
			cout << aDest[i];
		}
		cout << endl;

		ModelData_BRepRepresentation aBRep = thePart.BRepRepresentation();
		ModelData_PolyRepresentation aPoly(aBRep);
		ModelData_Part Npart(aPoly);
		ModelData_Model nMod;
		nMod.AddRoot(Npart);



		if (!aWriter.Transfer(nMod) || !aWriter.WriteFile(*UTFStr)) {
			cout << "Error in writing\n";
		}
		else cout << "New file created!\n";
		k++;
		aDest[7] = k;

	}
	bool VisitEnter(const ModelData_Assembly& theAssembly) {
		cout << "Enter in Assambly\n";

		ModelData_Model::ElementIterator MI(theAssembly);
		while (MI.HasNext()) {
			ModelData_SceneGraphElement & aSGE = MI.Next();
			aSGE.Accept(*this);
		}

		return true;
	}
	void VisitLeave(const ModelData_Assembly& theAssembly) {
		cout << "Exit from Assembly\n";
	}
	bool VisitEnter(const ModelData_Instance& theInstance) {
		cout << "Enter in Instance\n";

		ModelData_Model::ElementIterator MI(theInstance);
		while (MI.HasNext()) {
			ModelData_SceneGraphElement & aSGE = MI.Next();
			aSGE.Accept(*this);
		}

		return true;
	}
	void VisitLeave(const ModelData_Instance& theInstance) {
		cout << "Exit from Instance\n";
	}
};

/*ModelData_Model::ElementIterator MI(SGE);
	while (MI.HasNext()) {
		
		ModelData_SceneGraphElement & aSGE = MI.Next();
		if (aSGE.TypeId() == ModelData_Assembly::GetTypeId()) {
			RecursVR(aSGE, result);
		}			
		if (aSGE.TypeId() == ModelData_Instance::GetTypeId()) {
			ModelData_Instance & MDI = static_cast <ModelData_Instance &> (aSGE);
			RecursVR(MDI,result);
		}
		if (aSGE.TypeId() == ModelData_Part::GetTypeId()) {
			ModelData_Part & MDI = static_cast <ModelData_Part &> (aSGE);
			result->push_back(&MDI);
		}
	}*/