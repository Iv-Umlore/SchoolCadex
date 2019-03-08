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

#include <iostream>
#include "cadex_license_4.cxx"

using namespace std;
using namespace cadex;

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
			}
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
	PartVisitor aVisitor;
	aModel.Accept(aVisitor);
	cout << "Conversion completed..." << endl;

	system("pause");

	return 0;
}
