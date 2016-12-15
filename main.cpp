#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string>

#define F 3

void Contig( std::string );

void OPT(std::vector<int> page_references, std::vector<int> working_set);
void LRU(std::vector<int> page_references, std::vector<int> working_set);
void LFU(std::vector<int> page_references, std::vector<int> working_set);

int main (int argc, char* argv[]){
	Contig( std::string(argv[1]) );
	return 0;
	std::vector<int> page_references;
	std::vector<int> working_set;
	std::ifstream input(argv[1]);
	int holder;
	while(input >> holder){
		page_references.push_back(holder);
	}
	/*
	for (unsigned int i=0; i<pages_references.size(); i++){
		std::cout << page_references[i] << std::endl;
	}
	*/
	std::cout << "Simulating OPT with fixed frame size of " << F << std::endl;
	OPT(page_references, working_set);
	std::cout << std::endl;
	std::cout << "Simulating LRU with fixed frame size of " << F << std::endl;
	LRU(page_references, working_set);
	std::cout << std::endl;
	std::cout << "Simulating LFU with fixed frame size of " << F << std::endl;
	LFU(page_references, working_set);

	return 0;
}
