#define _CRT_SECURE_NO_WARNINGS

#include <cadex/STEP_Writer.hxx>
#include <cadex/IFC_Reader.hxx>
#include <cadex/IGES_Reader.hxx>
#include <cadex/ACIS_Reader.hxx>
#include <cadex/Para_Reader.hxx>
#include <cadex/JT_Reader.hxx>
#include <cadex/SLD_Reader.hxx>
#include <cadex/Rhino_Reader.hxx>
#include <cadex/OBJ_Reader.hxx>
#include <cadex/STL_Reader.hxx>
#include <cadex/VRML_Reader.hxx>
#include <cadex/BRep_Reader.hxx>

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
	// 
	IFC_Reader aReader;			// for *.ifc
	// IGES_Reader aReader;		// for 	*.igs, *.iges
	// STEP_Reader aReader;		// for *.stp, *.step
	// ACIS_Reader aReader;		// for *.sat, *.sab
	// Para_Reader aReader;		// for *.x_t, *.x_b, *.xmt_txt, *.xmt_bin, *.xmp_txt, *.xmp_bin
	// JT_Reader aReader;		// for *.jt
	// SLD_Reader aReader;		// for *.sldprt, *.sldasm
	// Rhino_Reader aReader;	// for *.3dm
	// OBJ_Reader aReader;		// for *.obj
	// STL_Reader aReader;		// for *.stl
	// VRML_Reader aReader;		// for *.wrl
	// BRep_Reader aReader;		// for *.brep
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

	SingletonWriter*
	SWriter = SingletonWriter::Create();
	SWriter->SetOstream(out);
	
	MyVisitor MyV;
	int count = 0;
	ModelData_Model::ElementIterator FirstIterator(aModel);
	while (FirstIterator.HasNext()) {
		ModelData_SceneGraphElement& aSGE = FirstIterator.Next();
		count++;
	}
	out << count << " ";

	// write model
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

	SingletonReader* SReader = SingletonReader::Create();
	SReader->SetIfstream(in);

	TestVisitor TV;
	ModelData_Model resMod;
	int ModelCount = 1;
	in >> ModelCount;
	for (int i = 0; i < ModelCount; i++) {
		ModelData_SceneGraphElement resSGE = ReadFromFile();
		resMod.AddRoot(resSGE);
	}

	// close input file.
	in.close();

	// write in file
	STEP_Writer STPWrite;
	Base_UTF16String file("p\\result.stp");
	if (!STPWrite.Transfer(resMod) || !STPWrite.WriteFile(file)) {
		cerr << "Last stage error!\n";
		return 1;
	}
	cout << "\nDone" << endl;
	
	return 0;
};
