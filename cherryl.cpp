#include <string>
#include <iostream>
#include "chText.hpp"
#include "chInter.cpp"

using namespace std;

const int STANDARD = 1;
const int VERSION = 1;
const int SUBVERSION = 0;

void versionInfo(){
	cout << "\nThis is Cherryl version " << VERSION << ", subversion " << SUBVERSION;
	cout << "(v" << STANDARD << "." << VERSION << "." << SUBVERSION << ").\n\n";
	cout << "Copyright 2019, Martín del Río.\n\n";
	cout << "Cherryl may be copied only un the terms of the GNU General Public License 3.0,\n";
	cout << "which may be found in the Cherrly repository.\n";
	cout << "Complete documentation for Cherryl, including FAQ lists, should be found on\n";
	cout << "this system using \"man cherryl\". If you have access to the Internet, point\n";
	cout << "your browser at http://cherryl.org/, the Cherryl Home Page.\n\n";
    exit(1);
}

void helpInfo() {
	cout << "\nUsage:\n";
	cout << "    cherryl <source file>\n";
	cout << "    cherryl [-v|-h]\n";
	cout << "\nOptions:\n";
	cout << "    -v --version              Display Cherryl version information\n";
	cout << "    -h --help                 Display this information\n\n";
	cout << "Complete documentation for Cherryl, including FAQ lists, should be found on\n";
	cout << "this system using \"man cherryl\". If you have access to the Internet, point\n";
	cout << "your browser at http://cherryl.org/, the Cherryl Home Page.\n\n";
	exit(0);
}

void showError(chText msg) {
	cout << "Cherryl Error: ";
    cout << msg << endl;
	exit(1);
}

int main(int argc, char** argv){
    if (argc <= 1) {
		showError("script file expected.");
	} else {
        chText sourceFile = "";
		for (unsigned int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
				versionInfo();
			} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
				helpInfo();
			} else if (sourceFile == "") {
				if(!sourceFile.loadFile(argv[i])){
					showError("file not found: '" + (string)argv[i] + "'.");
				}else{
					cherryInterpreter chIn;
					chIn.run(sourceFile);
				}
			}
		}
	}
    return 0;
}