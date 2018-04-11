#include <iostream>		// std::cout
#include <fstream>		// std::ifstream
#include <sstream>		// std::istringstream
#include <cstdlib>		// std::exit
#include <string>		// std::string

int main(int argc, char* argv[]) {
	if (argc != 6) {
		std::cerr << "Usage: " << argv[0] << "plist ptrace <pagesize> <replacement> <+/->" << std::endl;
	}

	// Convert pagesize (argv[3]) to an integer
	std::istringstream ss(argv[3]);
	int x;
	if (!(ss >> x)) {
		std::cerr << argv[3] << " is not a number." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// So we have easy access to our command-line args
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
	// PLIST: 	(pID, Total#MemoryLocation)
	// PTRACE: 	(pId, ReferencedMemoryLocation)
	std::ifstream ifs_plist (PLIST, std::ifstream::in);
	std::ifstream ifs_ptrace (PTRACE, std::ifstream::in);
	std::string pid, total_locations;
	std::size_t pos;

	if (!ifs_plist.is_open()) {
		std::cerr << "plist.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else if (!ifs_ptrace.is_open()) {
		std::cerr << "ptrace.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else {
		for(std::string line; std::getline(ifs_plist, line); ) {
			pid = line.substr(0, 1); 			// PID
			total_locations = line.substr(2); 	// Total number of memory locations
			std::cout << "The pid is: " << pid
			<< " and the total number of memory locations needed is: " 
			<< total_locations << std::endl;
		}
	}


	return 0;
}