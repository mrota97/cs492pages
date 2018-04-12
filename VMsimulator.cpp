#include <iostream>		// std::cout
#include <fstream>		// std::ifstream
#include <sstream>		// std::istringstream
#include <cstdlib>		// std::exit
#include <string>		// std::string
#include <vector>		// std::vector

unsigned long pageid;
unsigned long age = 0;
int total_pages = 0;

struct page_t {
	unsigned long page_number;
	int valid_bit;
	unsigned long last_accessed;
};

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
	std::vector<std::string> lines;
	std::vector<std::vector<page_t>> processes;
	std::vector<page_t> temp;

	std::ifstream ifs_plist (PLIST, std::ifstream::in);
	std::ifstream ifs_ptrace (PTRACE, std::ifstream::in);
	std::string pid, total_locations;
	int page_amount;
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
			// Get the total memory locations that we need for each process
			total_locations = line.substr(delimiter+1);
			// Pages per table = Locations needed / page size in locations;
			page_amount = (std::atoi(total_locations.c_str()) / PAGE_SIZE);
			for(unsigned long pages = 0; pages < page_amount; pages++) {
				// Populate the page_table with new pages
				unsigned long new_page_number = pageid++;
				page_t page = {new_page_number, 0, age};
				// std::cout << page.page_number << std::endl;
				temp.push_back(page);
				total_pages++;
			}
			// Push our new page table to the list of processes
			// page_t* page_table = temp.data();
			// temp.clear();
			processes.push_back(temp);
			temp.clear();
		}
		
		std::cout 
		<< "Number of page tables: " << processes.size() << "\n"
		<< std::endl;

		std::cout
		<< "Number of pages in each page table: " << std::endl; 

		unsigned long pages = 0;
		for (int i = 0; i < processes.size(); i++) {	
			std::cout << "Process " << i << " has ";
			for (int j = 0; j < processes[i].size(); j++) {
				pages++;
			}
			std::cout << pages << " pages in their page table." << std::endl;
			pages = 0;


			// while (ptr != NULL) {
			// 	std::cout << ptr[pages].page_number << std::endl;
			// 	pages++;
			// 	ptr++;	
			// }

			// std::cout 
			// << "Process " << i << " has " << pages << " pages in their page table"
			// << std::endl;
		}
	}


	return 0;
}