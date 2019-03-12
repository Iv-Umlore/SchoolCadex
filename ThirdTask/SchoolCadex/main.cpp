#define _CRT_SECURE_NO_WARNINGS

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
#include "cadex_license_4.cxx"

using namespace std;
using namespace cadex;

static void PrintFaceInfo(const ModelData_Face& theFace)
{
	ModelData_Surface aSurf = theFace.Surface(); // theFace is a ModelData_Face
	ModelData_Direction no;
	aSurf.Normal(0., 0., no);
	// Calculates the application point of the normal
	ModelData_Point pt;
	pt = aSurf.Value(0.0, 0.0);
	if (theFace.Orientation() == ModelData_SO_Forward) {
		std::cout << "Face orientation: forward" << std::endl;
	}
	else {
		std::cout << "Face orientation: reverse" << std::endl;
	}
}

class PartVisitor : public ModelData_Model::VoidElementVisitor
{
public:
	virtual void operator() (const ModelData_Part& thePart) override
	{
		auto aBRep = thePart.BRepRepresentation();
		const auto& aList = aBRep.Get();
		for (size_t i = 0, n = aList.Size(); i < n; ++i) {
			const auto& aBody = aList[i];
			ModelData_Shape::Iterator anIterator(aBody, ModelData_ST_Face);
			while (anIterator.HasNext()) {
				auto aSubshape = anIterator.Next();
				const auto& aFace = ModelData_Face::Cast(aSubshape);
				if (!aFace || !aFace.Surface()) {
					continue;
				}
				PrintFaceInfo(aFace);
			}
		}
	}
};

void RecursVR(ModelData_SceneGraphElement &SGE, std::list<ModelData_Part*>* result) {
	
	ModelData_Model::ElementIterator MI(SGE);
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
	}

};


int main(int argc, char *argv[])
{
	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " <input_file> <output_file>, where:" << endl;
		cerr << "    <input_file>  is a name of the STEP file to be read" << endl;
		cerr << "    <output_file> is a name of the STL file to be written" << endl;
		return 1;
	}
	//activate the license (aKey must be defined in cadex_license.cxx)
	if (!CADExLicense_Activate(aKey)) {
		cerr << "Failed to activate CAD Exchanger license." << endl;
		return 1;
	}
	const char* aSource = argv[1];
	//const char* aDest   = argv[2];
	ModelData_Model aModel;
	cout << "Conversion started..." << endl;
	STEP_Reader aReader;
	bool flag1 = !aReader.ReadFile(aSource);
	bool flag2 = !aReader.Transfer(aModel);
	if (flag1 || flag2) {
		cerr << "Failed to read the file " << aSource << endl;
		return 1;
	}
	
	cout << flag1 << " " << flag2 << endl;

	// import file
	std::list<ModelData_Part*> MDP_List;

	ModelData_Model::ElementIterator anIterator(aModel);
	if (anIterator.HasNext()) {
		ModelData_SceneGraphElement& aSGE = anIterator.Next();
		RecursVR(aSGE, &MDP_List);
	}

	cout << MDP_List.size() << endl;
	int i = 0;
	for (auto iter = MDP_List.begin(); iter != MDP_List.end(); iter++) {
		i++;
		cout << i << " ";
	}

	// part list completed

	ModelAlgo_BRepMesherParameters aParam;
	MeshAlgo_NetgenFactory aMF;
	MeshAlgo_NetgenFactory::Parameters aP;
	aP.SetGranularity(MeshAlgo_NetgenFactory::Parameters::Fine);
	aMF.SetParameters(aP);
	aParam.ComputationalMeshAlgo() = aMF;

	ModelAlgo_BRepMesher aMesher(aParam);

	

	STL_Writer aWriter;
	int k = 'a';
	char* aDest = new char[12];
	strcpy(aDest, "p/Part_0.stl");
	
	Base_UTF16String* UTFStr;
	
	for (auto iter = MDP_List.begin(); iter != MDP_List.end(); iter++) {

		ModelData_BRepRepresentation aBRep = (*iter)->BRepRepresentation();
		ModelData_PolyRepresentation aPoly(aBRep);
		ModelData_Part Npart(aPoly);		
		ModelData_Model nMod;
		nMod.AddRoot(Npart);

		aDest[7] = static_cast<char> (k);
		for (int i = 0; i < 12; i++)
			cout << aDest[i];
		cout << endl;
		UTFStr = new Base_UTF16String(aDest);
		if (!aWriter.Transfer(nMod) || !aWriter.WriteFile(*UTFStr)){
			return 1;
		}
		k++;
	}

	cout << "Done" << endl;

	return 0;
};
