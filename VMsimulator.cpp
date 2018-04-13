#include <iostream>		// std::cout
#include <fstream>		// std::ifstream
#include <sstream>		// std::istringstream
#include <cstdlib>		// std::exit
#include <string>		// std::string
#include <vector>		// std::vector
#include <math.h>

unsigned long pageid = 0;
unsigned long age = 0;
int total_pages = 0;

struct page_t {
	unsigned long page_number;
	int valid_bit;
	unsigned long last_accessed;
};

struct process_t {
	unsigned long pid;
	unsigned long memory;
	std::vector<page_t> page_table;
};

int main(int argc, char* argv[]) {
	if (argc != 6) {
		std::cerr << "Usage: " << argv[0] << "plist ptrace <pagesize> <replacement> <+/->" << std::endl;
	}

	std::istringstream ss(argv[3]);							// string to int for page_size
	int x;
	if (!(ss >> x)) {
		std::cerr << argv[3] << " is not a number." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	/*	
		We need to run a simulation for a virtual memry management system
		1. Simulate a paging system
		2. Implement three different page replacement algorithms
		3. Implement a variable page size
		4. Implement demand/pre-paging
		5. Record page swaps during a run
	*/

	const char* PLIST 	= argv[1];							// plist file name
	const char* PTRACE 	= argv[2];							// ptrace file name
	const int PAGE_SIZE = x;								// Page size (int)
	const char* RALG	= argv[4];							// Replacement algorithm
	const char* PREPAGE = argv[5];							// +/- for enabling pre-paging

	const int MEMORY_SIZE = 512;							// Total memory size

	std::vector<process_t> processes;						// Our process list
	std::ifstream ifs_plist (PLIST, std::ifstream::in);		// Stream for plist.txt
	std::ifstream ifs_ptrace (PTRACE, std::ifstream::in);	// Stream for ptrace.txt
	std::string pid, total_locations, referenced_location;	// Strings for storing members of our structs 
	// std::string line;										// Line for temporary storage
	int page_amount;										// Amount of pages per page table
	std::size_t delimiter;									// Our delimiter here is the space

	if (!ifs_plist.is_open()) {
		std::cerr << "plist.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else if (!ifs_ptrace.is_open()) {
		std::cerr << "ptrace.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else {
		unsigned long page_swaps = 0;

		// Reading in plist.txt
		// PLIST: 	(pID, Total#MemoryLocation)
		for(std::string line; std::getline(ifs_plist, line); ) {
			process_t process = {0};
			delimiter = line.find(" ");
			// Get the total memory locations that we need for each process
			pid = line.substr(0, delimiter);
			total_locations = line.substr(delimiter+1);

			process.pid = std::atoi(pid.c_str());
			// Pages per table = Locations needed / page size in locations;
			page_amount = std::atoi(total_locations.c_str()) / PAGE_SIZE;
			for(unsigned long pages = 0; pages < page_amount; pages++) {
				// Populate the page_table with new pages
				page_t page = {pageid++, 0, age};
				// std::cout << page.page_number << std::endl;
				process.page_table.push_back(page);
			}
		}

		// Let's read in ptrace now!
		// PTRACE: 	(pId, ReferencedMemoryLocation)
		for(std::string line; std::getline(ifs_ptrace, line); ) {
			delimiter = line.find(" ");
			pid = line.substr(0, delimiter);
			referenced_location = line.substr(delimiter+1);

			unsigned long location = ceil(std::atoi(referenced_location.c_str()) / (float) PAGE_SIZE);
			for (std::vector<process_t>::iterator process = processes.begin(); process != processes.end(); process++) {
				bool found = false;
				if (process->pid == std::atoi(pid.c_str())) {
					for (std::vector<page_t>::iterator page = process->page_table.begin(); page != process->page_table.end(); page++) {
						if (page->page_number == location) {
							int stored_bit = page->valid_bit;

							if (stored_bit == 1) {
								found = true;
								break;
							}
							else {
								// A page swap needs to occur
								page_swaps++;
								/*
									Our swapping algorithm goes here!
									Our choices are: 
									Clock Based policy 
									First In, First Out
									Least Recently Used
								*/
							}
						}
					}
				}
				if (found) break;
			}

		}

	}


	std::exit(EXIT_SUCCESS);
}