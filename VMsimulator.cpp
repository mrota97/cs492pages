/*
	Matthew Rota and Evan Bedser
	CS-492 Operating Systems
	Assignment 2: Paging
	"I pledge my honor that I have abided by the Stevens Honor System"
*/

#include <iostream>		// std::cout
#include <fstream>		// std::ifstream
#include <sstream>		// std::istringstream
#include <cstdlib>		// std::exit
#include <string>		// std::string
#include <cstring>		// strcmp()
#include <vector>		// std::vector
#include <math.h>		// ciel()
#include <climits>

unsigned long pageid = 0;
unsigned long age = 0;
int total_pages = 0;

// This struct 
struct memory_t {
	unsigned long process_id;
	unsigned long page_number;
};

struct page_t {
	unsigned long page_number;
	int valid_bit;
	unsigned long age;
	unsigned long last_accessed;
};

struct process_t {
	unsigned long pid;
	unsigned long memory;
	unsigned long pagen_start;
	unsigned long pagen_end;
	std::vector<page_t> page_table;
};



int main(int argc, char* argv[]) {
	if (argc != 6) {
		std::cerr << "Usage: " << argv[0] << "plist ptrace <pagesize> <replacement> <+/->" << std::endl;
	}

	std::istringstream ss(argv[3]);														// string to int for page_size
	int x;
	if (!(ss >> x)) {
		std::cerr << argv[3] << " is not a number." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	const char* PLIST 	= argv[1];														// plist file name
	const char* PTRACE 	= argv[2];														// ptrace file name
	const int PAGE_SIZE = x;															// Page size (int)
	const char* RALG	= argv[4];														// Replacement algorithm
	const bool PREPAGE = (strcmp(argv[5], "+")) ? false : true;							// +/- for enabling pre-paging

	const long unsigned MEMORY_SIZE = 512;												// Total memory size
	memory_t* main_memory = (memory_t*)calloc(MEMORY_SIZE/PAGE_SIZE, sizeof(memory_t));	// Our main memory for the programs	
	
	std::vector<process_t> processes;													// Our process list
	std::ifstream ifs_plist (PLIST, std::ifstream::in);									// Stream for plist.txt
	std::ifstream ifs_ptrace (PTRACE, std::ifstream::in);								// Stream for ptrace.txt
	std::string pid, total_locations, referenced_location;								// Strings for storing members of our structs 
	int page_amount;																	// Amount of pages per page table
	unsigned long page_swaps = 0, initial_pages = 0, page_offset = 0;					// Amount of page swaps
	std::size_t delimiter;																// Our delimiter here is the space

	std::vector<process_t>::iterator process_it;										// Iterator for our processes list
	std::vector<page_t>::iterator page_it;												// Iterator for a page table

	if (!ifs_plist.is_open()) {
		std::cerr << "plist.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else if (!ifs_ptrace.is_open()) {
		std::cerr << "ptrace.txt: An error occurred while reading the file." << std::endl;
		std::exit(EXIT_FAILURE);
	} else {
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

			// Set the page offset for easy access later
			process.pagen_start = page_offset;
			process.pagen_end = (page_amount-1) + page_offset;
			page_offset += page_amount;
			for(unsigned long pages = 0; pages < page_amount; pages++) {
				// Populate the page_table with new pages
				page_t page = {pageid++, 0, age++, 0};
				// std::cout << page.page_number << std::endl;
				process.page_table.push_back(page);
			}
			processes.push_back(process);
		}

		// Initial allocation of memory
		initial_pages = MEMORY_SIZE / PAGE_SIZE / processes.size();
		int offset = 0;
		for (std::vector<process_t>::iterator process = processes.begin(); process != processes.end(); process++) {
			for (int location = 0; location < initial_pages; location++) {
				main_memory[location+offset].process_id = process->pid;
				main_memory[location+offset].page_number = process->page_table[location].page_number;
				process->page_table[location].valid_bit = 1;
				if (location == process->page_table.size()-1) 
					break;
				age++;
			}
			offset += initial_pages;
		}

		// Let's read in ptrace now!
		// PTRACE: 	(pId, ReferencedMemoryLocation)
		unsigned long location, page_to_swap, index, temp_age;
		for(std::string line; std::getline(ifs_ptrace, line); ) {
			delimiter = line.find(" ");
			pid = line.substr(0, delimiter);
			referenced_location = line.substr(delimiter+1);
			int ref_loc = std::atoi(referenced_location.c_str());

			location = ceil(ref_loc / (float) PAGE_SIZE);
			for (int i = 0; i < processes.size(); i++) {
				if (location >= processes[i].pagen_start && location <= processes[i].pagen_end) {
					page_to_swap = location - processes[i].pagen_start;

					if (processes[i].page_table[page_to_swap].valid_bit != 1) {
						index = processes[i].page_table.size();

						if (strcmp(RALG, "FIFO") == 0) {
							temp_age = ULONG_MAX;

							for (int j = 0; j < processes[i].page_table.size(); j++) {
								if (processes[i].page_table[j].age < temp_age && processes[i].page_table[j].valid_bit == 1) {
									temp_age = processes[i].page_table[j].age;
									index = j;
								}
							}

							processes[i].page_table[index].valid_bit = 0;
							processes[i].page_table[page_to_swap].valid_bit = 1;
							age++;
						}

						else if (strcmp(RALG, "LRU") == 0) {
							
						}

						page_swaps++;
					}
				}
			}

			// for (process_it = processes.begin(); process_it != processes.end(); process_it++) {
			// 	if (location >= process_it->pagen_start && location <= process_it->pagen_end) {
			// 		// I use the offsets I've provided in the struct definition to calculate the index of the page I need to access in the page table
			// 		page_to_swap = location - process_it->pagen_start;
			// 		if (process_it->page_table[page_to_swap].valid_bit != 1) {
			// 			// A page swap needs to occur
			// 			index = process_it->page_table.size();
			// 			page_swaps++;
			// 			if (strcmp(RALG, "FIFO")) {
			// 				if (PREPAGE) {}
			// 				else {
			// 					temp_age = ULONG_MAX;
								
			// 					// First find the page that we want to swap for. This is the (first) page created
			// 					for (int i = 0; i < process_it->page_table.size(); i++) {
			// 						if (process_it->page_table[i].age < temp_age && process_it->page_table[i].valid_bit == 1) {
			// 							temp_age = process_it->page_table[i].age;
			// 							index = i;
			// 						}
			// 					}

			// 					process_it->page_table[index].valid_bit = 0;
			// 					process_it->page_table[location].valid_bit = 1;
			// 					age++;
			// 				}
			// 			}
			// 			else if (strcmp(RALG, "LRU")) {
			// 				if (PREPAGE) {}
			// 				else {
			// 					unsigned long time_diff = 0;

			// 					for (int i = 0; i < process_it->page_table.size(); i++) {
			// 						if ((age - process_it->page_table[i].last_accessed) > time_diff && process_it->page_table[i].valid_bit == 1) {
			// 							time_diff = (age - process_it->page_table[i].last_accessed);
			// 							index = i;
			// 						}
			// 					}
			// 					process_it->page_table[index].valid_bit = 0; 			// Flip the page we are swapping out
			// 					process_it->page_table[location].valid_bit = 1; 		// "Swap" in the page we want
			// 					process_it->page_table[index].last_accessed = age++;
			// 					process_it->page_table[location].last_accessed = age++;
			// 				}
			// 			}
			// 			else if (strcmp(RALG, "Clock")) {
			// 				if (PREPAGE) {}
			// 				else {}
			// 			}
			// 		}
			// 		break;
			// 	}
			// }

		}

	}

	// std::cout
	// << "Main memory: " << std::endl;
	// for (int i = 0; i < MEMORY_SIZE / PAGE_SIZE; i++) {
	// 	memory_t location = main_memory[i];
	// 	if (i == 0) 
	// 		std::cout << "[(" << location.process_id << ", " << location.page_number << "), ";
	// 	if (i+1 == (MEMORY_SIZE / PAGE_SIZE))
	// 		std::cout << "(" << location.process_id << ", " << location.page_number << ")]";
	// 	else
	// 		std::cout << "(" << location.process_id << ", " << location.page_number << "), ";
	// }
	// std::cout << std::endl;

	for (process_it = processes.begin(); process_it != processes.end(); process_it++) {

		std::cout << "Process " << process_it->pid 
		<< " (" 
		<< process_it->memory << ", "
		<< process_it->pagen_start << ", "
		<< process_it->pagen_end << ", "
		<< process_it->memory << ", "
		<< "[" << "(" << process_it->page_table[0].page_number << ", " << process_it->page_table[0].last_accessed << "), "
		<< "(" << process_it->page_table[1].page_number << ", " << process_it->page_table[1].last_accessed << "), "
		<< "(" << process_it->page_table[2].page_number << ", " << process_it->page_table[2].last_accessed << ")]"
		 ")" << std::endl;
	}


	std::cout
	<< "Program executed successfully." << "\n"
	<< "Page Size:                    " << PAGE_SIZE << "\n"
	<< "Number of processes:          " << processes.size() << "\n"
	<< "Number of pages that fit into memory: " << MEMORY_SIZE / PAGE_SIZE << "\n"
	<< "Initial pages to load for each process: " << initial_pages << "\n"
	<< "Replacement algorithm used:   " << RALG << "\n"
	<< "Pre-paging?:                  " << PREPAGE << "\n"
	<< "Number of page swaps:         " << page_swaps << "\n"
	<< std::endl;

	std::cout << (strcmp("-", "+"))
	<< "Arg 5 is " << argv[5]
	<< std::endl;

	std::exit(EXIT_SUCCESS);
}