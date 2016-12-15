#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>

#define SIZE 256
#define ROW 32

struct conatiner {
  char process_id;
  int mem_size;
  int count;
  int arrival_time[];
  int run_time[];
} temp;

void Contig( std::string filename ) {
  std::ifstream input(filename.c_str());
  int
    numberProcesses,
    i
  ;
  std::string
  line,
  process_id
  ;

  input >> numberProcesses;
  std::smatch sm;
  std::regex express("([A-Z]) ([0-9]+)");
  std::regex arrival_run_times("([0-9]+)/([0-9]+)");
  std::getline( input, line ); // Clears newline of the firstline
  char memory[ SIZE ];
  for( i = 0; i < numberProcesses; i++ )
  {
    std::getline( input, line );
    std::cout << line << std::endl;
    std::regex_search( line, sm, express );
    temp.process_id = sm[1].str()[0];
    temp.mem_size = std::stoi( sm[2], nullptr );
    std::regex_search( line, sm, arrival_run_times );
    temp.count = sm.size()/3;
    int a_times[ temp.count ];
    int r_times[ temp.count ];

    for( unsigned int j = 0; j < sm.size(); j+=3 )
    {
      a_times[ j/3 ] = std::stoi( sm[j+1] );
      std::cout << "[a" << j << "] " << a_times[j/3] << std::endl;
      r_times[ j/3 ] = std::stoi( sm[j+2] );
      std::cout << "[r" << j << "] " << r_times[j/3] << std::endl;
    }
  }
  runNextFit( a_times, r_times );
  return;
  runBestFit( a_times, r_times );
  runWorstFit( a_times, r_times );
}

void runNextFit( a_times, r_times )
{
  
}
