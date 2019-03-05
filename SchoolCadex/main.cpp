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

static std::string PrintSurfType(const ModelData_Surface& theSurf)
{
	switch (theSurf.Type()) {
	case cadex::ModelData_ST_Plane:             return "Plane";
	case cadex::ModelData_ST_Cylinder:          return "Cylinder";
	case cadex::ModelData_ST_Cone:              return "Cone";
	case cadex::ModelData_ST_Sphere:            return "Sphere";
	case cadex::ModelData_ST_Torus:             return "Torus";
	case cadex::ModelData_ST_LinearExtrusion:   return "LinearExtrusion";
	case cadex::ModelData_ST_Revolution:        return "Revolution";
	case cadex::ModelData_ST_Bezier:            return "Bezier";
	case cadex::ModelData_ST_BSpline:           return "BSpline";
	case cadex::ModelData_ST_UserDefined:       return "UserDefined";
	case cadex::ModelData_ST_Trimmed: {
		const auto& aTrimSurf = static_cast<const ModelData_RectangularTrimmedSurface&> (theSurf);
		return "Trimmed " + PrintSurfType(aTrimSurf.BasisSurface());
	}
	case cadex::ModelData_ST_Offset: {
		const auto& anOffsetSurf = static_cast<const ModelData_OffsetSurface&> (theSurf);
		return "Offset " + PrintSurfType(anOffsetSurf.BasisSurface());
	}
	default:
		break;
	}
	return "Undefined";
}
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
	std::cout << "Surface type: " << PrintSurfType(aSurf) << std::endl;
	//std::cout << "Axis location[" << baseLocation[0] << "," << baseLocation[1] << "," << baseLocation[2] << "]" << std::endl;
	std::cout << "DirX[" << no.XDir().X() << "," << no.XDir().Y() << "," << no.XDir().Z() << "]" << std::endl;
	std::cout << "DirY[" << no.YDir().X() << "," << no.YDir().Y() << "," << no.YDir().Z() << "]" << std::endl;
	std::cout << "DirZ[" << no.ZDir().X() << "," << no.ZDir().Y() << "," << no.ZDir().Z() << "]" << std::endl;
	std::cout << "Normal[" << no.X() << "," << no.Y() << "," << no.Z() << "]" << std::endl;
	std::cout << "Normal position[" << pt[0] << "," << pt[1] << "," << pt[2] << "]" << std::endl;
	cout << "\n" << endl;
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
