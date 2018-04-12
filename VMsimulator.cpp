#include <iostream>		// std::cout
#include <fstream>		// std::ifstream
#include <sstream>		// std::istringstream
#include <cstdlib>		// std::exit
#include <string>		// std::string
#include <vector>		// std::vector
#include <array>		// std::array

unsigned long pageid;
unsigned long age = 0;

struct page_t {
	unsigned long page_number;
	int valid_bit;
	unsigned long last_accessed;
}

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

	const int MEMORY_SIZE = 512;
	
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

/*	
	Data Structure Strategy
	Hierarchy
	Processes 			- List of page tables
		-> Page Tables 	- List of pages
			-> page 	- Page struct
*/
	std::vector<string> lines ();
	std::vector<page_t[]> processes ();
	std::vector<page_t> temp ();

	std::ifstream ifs_plist (PLIST, std::ifstream::in);
	std::ifstream ifs_ptrace (PTRACE, std::ifstream::in);
	std::string pid, total_locations;
	std::size_t delimiter;									// Our delimiter here is the space
	if (!ifs_plist.is_open()) {
		std::cerr << "plist.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else if (!ifs_ptrace.is_open()) {
		std::cerr << "ptrace.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else {
		
		for(std::string line; std::getline(ifs_plist, line); ) {
			lines.push_back(line);
		}

		unsigned long proc_num = lines.size();
		for(std::string line: lines) {
			delimiter = line.find(" ");
			total_locations = line.substr(delimiter+1);
			for(unsigned long pages = 0; pages < )
		}


			// Create the pages and page table for the processes here.
			for (unsgined long i = 0; i < ; i++) {
				page_t page = {pageid++, 0, age};
				temp.push_back(page);
			}
			page_t[] page_table = &temp[0];
			processes.push_back(page_table);
		}
		
		std::cout 
		<< "Number of page tables: " << processes.size()
		<< "Number of pages in page table 0: " << (sizeof(processes[0])/sizeof(*processes[0]))
		<< std::endl;
	}


	return 0;
}