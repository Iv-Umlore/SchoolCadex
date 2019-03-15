#define _CRT_SECURE_NO_WARNINGS

#include <cadex/STEP_Writer.hxx>

#include "MyVisitor.h"
#include "cadex_license_4.cxx"
#include "Reader.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <input_file>, where:" << endl;
		cerr << "    <input_file>  is a name of the STEP file to be read" << endl;
		return 1;
	}
	//activate the license (aKey must be defined in cadex_license.cxx)
	if (!CADExLicense_Activate(aKey)) {
		cerr << "Failed to activate CAD Exchanger license." << endl;
		return 1;
	}
	const char* aSource = argv[1];
	ModelData_Model aModel;
	cout << "Conversion started..." << endl;
	STEP_Reader aReader;
	bool flag1 = !aReader.ReadFile(aSource);
	bool flag2 = !aReader.Transfer(aModel);
	if (flag1 || flag2) {
		cerr << "Failed to read the file " << aSource << endl;
		return 1;
	}
	
	// create output file. One for ALL
	ofstream out;
	out.open("p\\file.txt");
	if (!out.is_open()) {
		cerr << "Sorry, open Error!" << endl;
		return 1;
	}
	SingletonWriter* SWriter = SingletonWriter::Create();
	SWriter->SetOstream(out);
	
	MyVisitor MyV;

	ModelData_Model::ElementIterator anIterator(aModel);
	while (anIterator.HasNext()) {
		ModelData_SceneGraphElement& aSGE = anIterator.Next();
		aSGE.Accept(MyV);
	}
	// close output file.
	out.close();

	ifstream in;
	in.open("p\\file.txt");
	if (!in.is_open()) {
		cerr << "Sorry, open Error!" << endl;
		return 1;
	}

	/*

	SingletonReader* SReader = SingletonReader::Create();
	SReader->SetIfstream(in);
	
	ModelData_SceneGraphElement resSGE = ReadFromFile();
	ModelData_Model resMod;
	resMod.AddRoot(resSGE);

	// close input file.
	in.close();

	// запись в файл
	STEP_Writer STPWrite;
	Base_UTF16String file("p\\result.stp");
	if (!STPWrite.Transfer(resMod) || !STPWrite.WriteFile(file)) {
		cerr << "Last stage error!\n";
		return 1;
	}
	*/
	cout << "\n\nDone" << endl;

	

	return 0;
};
