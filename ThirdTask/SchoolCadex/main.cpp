#define _CRT_SECURE_NO_WARNINGS

#include "MyVisitor.h"
#include "cadex_license_4.cxx"

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
	
	MyVisitor MyV;

	ModelData_Model::ElementIterator anIterator(aModel);
	if (anIterator.HasNext()) {
		ModelData_SceneGraphElement& aSGE = anIterator.Next();
		aSGE.Accept(MyV);
	}
		
	cout << "Done" << endl;

	return 0;
};
