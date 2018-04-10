#include <iostream>		// std::cout
#include <fstream>		// std::ifstream
#include <sstream>		// std::istringstream
#include <cstdlib>		// std::exit

int main(int argc, char* argv[]) {
	if (argc != 6) {
		std::err << "Usage: " << argv[0] << "plist ptrace <pagesize> <replacement> <+/->" << std::endl;
	}

	// Convert pagesize to an int
	std::istringstream ss(argv[3]);
	int x;
	if (!(ss >> x)) {
		std::cerr << argv[3] << " is not a number." << std::endl;
		exit(EXIT_FAILURE);
	}

	const char* PLIST 	= argv[1];
	const char* PTRACE 	= argv[2];
	const int PAGE_SIZE = x;
	const char* RALG	= argv[4];
	const char* PREPAGE = argv[5];
	
/*	
	We need to run a simulation for a virtual memry management system
	1. Simulate a paging system
	2. Implement three different page replacement algorithms
	3. Implement a variable page size
	4. Implement demand/pre-paging
	5. Record page swaps during a run
*/

	// Let's read in the files we pass from the command line with ifstream
	// PLIST: (pID, Total#MemoryLocation)
	// PTRACE: (pId, ReferencedMemoryLocation)
	std::ifstream ifs ()


	return 0;
}