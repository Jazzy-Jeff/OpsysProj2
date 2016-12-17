#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#define F 3

void working_set_printer(std::vector<int> working_set, int index){
	std::cout << "referencing page ";
	std::cout << working_set[index];
	std::cout << " [mem: ";
	for(unsigned int a=0; a<working_set.size(); a++){
		//std::cout << working_set[a] << std::endl; 
		if (working_set[a] == -1){
			std::cout << ".";
			if (a+1 < working_set.size()){
				std::cout << " ";
			}
		}
		else if (a+1 == working_set.size()){
			std:: cout << working_set[a];
		}
		else{
			std::cout << working_set[a];
			std::cout << " ";
		}
	}
	std::cout << "] PAGE FAULT";
}

bool in_working_set(std::vector<int> working_set, int element){
	bool b=false;
	for (unsigned int a=0; a<F; a++){
		if (working_set[a] == element){
			b = true;
		}
	}
	return b;
}

void OPT(std::vector<int> page_references, std::vector<int> working_set){
	bool in_mem;
	bool init = false;
	bool detected;
	int prev_undetected = 0;
	int needs_swap;
	int victim;
	unsigned int max_distance;
	int page_faults = 0;

	for (unsigned int i=0; i<F; i++){
		working_set.push_back(-1);
	}

	for (unsigned int i=0; i<page_references.size(); i++){
		in_mem = in_working_set(working_set, page_references[i]);
		if (!in_mem){
			//page fault detected
			page_faults++;
			for (unsigned int a=0; a<working_set.size(); a++){
				if(working_set[a] == -1){
					working_set[a] = page_references[i];
					//std::cout << working_set[a] << std::endl;
					working_set_printer(working_set, a);
					std::cout << " (no victim page)" << std::endl;
					init = true;
					break;
				}
				init = false;
			}
			if(!init){
				prev_undetected = false;
				//page replacement needed
				for (unsigned int a=0; a<working_set.size(); a++){
					detected = false;
					for (unsigned int j=i; j<page_references.size(); j++){
						//we have detected the closest refernce of the frame in mem currently
						if (page_references[j] == working_set[a]){
							detected = true;
							if (prev_undetected != 0){
								continue;
							}
							if (a == 0){
								max_distance = j;
								needs_swap = a;
							}
							else{
								if (j > max_distance){
									max_distance = j;
									needs_swap = a;
								}
								else if (j == max_distance){
									if (working_set[needs_swap] > working_set[a]){
										needs_swap = a;
									}
								}
							}
							break;
						}
					}
					if (!detected){
						if (prev_undetected == 0){
							needs_swap = a;
						}
						else if (prev_undetected > working_set[a]){
							needs_swap = a;
						}
						prev_undetected = working_set[a];
					}
				}

				victim = working_set[needs_swap];
				working_set[needs_swap] = page_references[i];
				working_set_printer(working_set, needs_swap);
				std::cout << " (victim page ";
				std::cout << victim << ")" << std::endl;
			}
		}
	}
	std::cout << "End of OPT simulation (" << page_faults << " page faults)" << std::endl;
}

void LRU(std::vector<int> page_references, std::vector<int> working_set){
	bool in_mem;
	bool init = false;
	int needs_swap;
	int victim;
	unsigned int max_distance;
	int page_faults = 0;

	for (unsigned int i=0; i<F; i++){
		working_set.push_back(-1);
	}

	for (unsigned int i=0; i<page_references.size(); i++){
		in_mem = in_working_set(working_set, page_references[i]);
		if (!in_mem){
			//page fault detected
			page_faults++;
			for (unsigned int a=0; a<working_set.size(); a++){
				if(working_set[a] == -1){
					working_set[a] = page_references[i];
					//std::cout << working_set[a] << std::endl;
					working_set_printer(working_set, a);
					std::cout << " (no victim page)" << std::endl;
					init = true;
					break;
				}
				init = false;
			}
			if(!init){
				//page replacement needed
				for (unsigned int a=0; a<working_set.size(); a++){
					for (unsigned int j=i; j>=0; j--){
						//we have detected the closest refernce of the frame in mem currently
						if (page_references[j] == working_set[a]){
							if (a == 0){
								max_distance = i-j;
								needs_swap = a;
							}
							else{
								if (i-j > max_distance){
									max_distance = i-j;
									needs_swap = a;
								}
								else if (i-j == max_distance){
									if (working_set[needs_swap] > working_set[a]){
										needs_swap = a;
									}
								}
							}
							break;
						}
					}
				}
				victim = working_set[needs_swap];
				working_set[needs_swap] = page_references[i];
				working_set_printer(working_set, needs_swap);
				std::cout << " (victim page ";
				std::cout << victim << ")" << std::endl;
			}
		}
	}
	std::cout << "End of LRU simulation (" << page_faults << " page faults)" << std::endl;
}

void LFU(std::vector<int> page_references, std::vector<int> working_set){
	bool in_mem;
	bool init = false;
	int needs_swap;
	int victim;
	int min_frequency;
	int page_faults = 0;
	std::vector<bool> page_faults_list;
	std::vector<int> working_set_frequencies;

	for (unsigned int i=0; i<F; i++){
		working_set.push_back(-1);
		working_set_frequencies.push_back(0);
	}

	for (unsigned int i=0; i<page_references.size(); i++){
		in_mem = in_working_set(working_set, page_references[i]);
		if (!in_mem){
			//page fault detected
			page_faults_list.push_back(true);
			for (unsigned int b=0; b<F; b++){
				working_set_frequencies[b] = 0;
			}
			page_faults++;
			for (unsigned int a=0; a<working_set.size(); a++){
				if(working_set[a] == -1){
					working_set[a] = page_references[i];
					//std::cout << working_set[a] << std::endl;
					working_set_printer(working_set, a);
					std::cout << " (no victim page)" << std::endl;
					init = true;
					break;
				}
				init = false;
			}
			if(!init){
				//page replacement needed
				for (unsigned int a=0; a<working_set.size(); a++){
					for (unsigned int j=i; j>=0; j--){
						//we have detected the closest refernce of the frame in mem currently
						if (page_references[j] == working_set[a]){
							if (!page_faults_list[j]){
								working_set_frequencies[a]++;
							}
							else{
								break;
							}
						}
					}
				}
				min_frequency = working_set_frequencies[0];
				needs_swap = 0;
				for(unsigned int a=1; a<working_set_frequencies.size(); a++){
					if (working_set_frequencies[a] < min_frequency){
						needs_swap = a;
						min_frequency = working_set_frequencies[a];
					}
				}
				victim = working_set[needs_swap];
				working_set[needs_swap] = page_references[i];
				working_set_printer(working_set, needs_swap);
				std::cout << " (victim page ";
				std::cout << victim << ")" << std::endl;
			}
		}
		else{
			page_faults_list.push_back(false);
		}
	}
	std::cout << "End of LFU simulation (" << page_faults << " page faults)" << std::endl;
}