#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#define F 3

void OPT(std::vector<int> page_references, std::vector<int> working_set);

int main (int argc, char* argv[]){
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

	OPT(page_references, working_set);

	return 0;
}