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

#include <cadex/Para_Writer.hxx>

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

void RecursVR(ModelData_SceneGraphElement SGE, std::list<ModelData_Part>* result) {
	
	ModelData_Model::ElementIterator MI(SGE);
	while (MI.HasNext()) {
		const ModelData_SceneGraphElement & aSGE = MI.Next();
		if (aSGE.TypeId() == ModelData_Assembly::GetTypeId())
			RecursVR(aSGE,result);
		if (aSGE.TypeId() == ModelData_Instance::GetTypeId()) {
			const ModelData_Instance & MDI = static_cast <const ModelData_Instance &> (aSGE);
			RecursVR(MDI.Reference(),result);
		}
		if (aSGE.TypeId() == ModelData_Part::GetTypeId()) {
			const ModelData_Part & MDI = static_cast <const ModelData_Part &> (aSGE);
			result->push_back(MDI);
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
	if (!aReader.ReadFile(aSource) || !aReader.Transfer(aModel)) {
		cerr << "Failed to read the file " << aSource << endl;
		return 1;
	}

	// import file
	std::list<ModelData_Part> MDP_List;

	ModelData_Model::ElementIterator anIterator(aModel);
	while (anIterator.HasNext()) {
		const ModelData_SceneGraphElement& aSGE = anIterator.Next();
		RecursVR(aSGE,&MDP_List);
	}
	// part list completed

	ModelAlgo_BRepMesherParameters aParam;
	MeshAlgo_NetgenFactory aMF;
	MeshAlgo_NetgenFactory::Parameters aP;
	aP.SetGranularity(MeshAlgo_NetgenFactory::Parameters::Fine);
	aMF.SetParameters(aP);
	aParam.ComputationalMeshAlgo() = aMF;
	ModelAlgo_BRepMesher aMesher(aParam);
	ModelData_BRepRepresentation aBRep;
	ModelData_PolyRepresentation aPoly;

	ModelData_Model nMod;

	Para_Writer aWriter;
	bool res;
	int k = 0;
	char aDest[10];
	string str = "Part 0.stp";
	for (int i = 0; i < 10; i++) {
		aDest[i] = str[i];
	}
	Base_UTF16String* UTFStr;
	
	for (auto iter = MDP_List.begin(); iter != MDP_List.end(); iter++) {
		aBRep = iter->BRepRepresentation();
		aPoly = aMesher.Compute(aBRep);
		ModelData_Part Npart(aPoly);
		nMod.AddRoot(Npart);

		aDest[5] = static_cast<char> (k);
		UTFStr = new Base_UTF16String(aDest);
		cout << UTFStr << endl;
		if (res = aWriter.Transfer(aModel) && aWriter.WriteFile(*UTFStr)){
			return 1;
		}
		k++;
		MDP_List.pop_front();
	}

	cout << "Done" << endl;

	return 0;
};
