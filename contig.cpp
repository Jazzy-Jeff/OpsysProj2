#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>

#define SIZE 256
#define ROW 32

struct my_proccess_t {
  char process_id;
  int mem_size;
  int count; // arrival_time.length && run_time.length
  int * arrival_time;
  int * run_time;
};

struct mem_loc {
  char id;
  int size;
  int expires;
};

void Contig( std:: string );
void printMemory( mem_loc m[] );
void runNextFit ( my_proccess_t p[], int * );
void runBestFit ( my_proccess_t p[], int * );
void runWorstFit( my_proccess_t p[], int * );

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
  my_proccess_t p[ numberProcesses ];
  for( i = 0; i < numberProcesses; i++ )
  {
    std::getline( input, line );
    std::cout << line << std::endl;
    std::regex_search( line, sm, express );
    p[i].process_id = sm[1].str()[0];
    p[i].mem_size = std::stoi( sm[2], nullptr );
    std::regex_search( line, sm, arrival_run_times );
    p[i].count = sm.size()/3;
    p[i].arrival_time = (int *)std::calloc( p[i].count, sizeof(int) );
    p[i].run_time = (int *)std::calloc( p[i].count, sizeof(int) );

    // TODO: not getting all the arrival and run times for any given process

    for( unsigned int j = 0; j < sm.size(); j+=3 )
    {
      p[i].arrival_time[ j/3 ] = std::stoi( sm[j+1] );
      std::cout << "[a" << j << "] " << p[i].arrival_time[j/3] << std::endl;
      p[i].run_time[ j/3 ] = std::stoi( sm[j+2] );
      std::cout << "[r" << j << "] " << p[i].run_time[j/3] << std::endl;
    }
  }
  runNextFit( p, &numberProcesses );
  return;
  //runBestFit( a_times, r_times );
    //runWorstFit( a_times, r_times );
}

void printMemory( mem_loc memory[] )
{
  char c_id;
  int count = 0;
  for( int i = 0; i < ROW; i++ ) std::cout << "=";
  std::cout << std::endl;
  for( int i = 0; i < SIZE; i++, count-- )
  {
    if( count == 0 )
    {
      c_id = memory[i].id;
      count = memory[i].size;
    }
    std::cout << c_id;
    if( i != 0 && (i+1) % ROW == 0 ) std::cout << std::endl;
  }
  for( int i = 0; i < ROW; i++ ) std::cout << "=";
  std::cout << std::endl;
}

void runNextFit( my_proccess_t p[], int * size )
{
  int time = 0, numRemoved = 0, skipped = 0;
  mem_loc memory[ SIZE ];
  memory[0].id = '.';
  memory[0].size = SIZE;
  memory[0].expires = -1;
  std::cout << "time " << time << "ms: Simulator started (Contiguous -- Next-Fit)" << std::endl;
  int step = 1;
  while( (numRemoved + skipped) != *size )
  {
    // adding
    for( int i = 0; i < *size; i++ )
    {
      for( int j = 0; j < p[i].count; j++)
      {
        if( p[i].arrival_time[j] == time )
        {
          std::cout << "time " << time << "ms: Process " << p[i].process_id << " arrived (requires " << p[i].mem_size << " frames)" << std::endl;
          int k;
          for( k = 0; k < SIZE; k += step)
          {
            // std::cout << "checking memory[" << k << "] = {" << memory[k].id << ", " << memory[k].size << ", " << memory[k].expires << "}" << std::endl;
            if( memory[k].id == '.' && memory[k].size >= p[i].mem_size )
            {
              std::cout << "time " << time << "ms: Placed process " << p[i].process_id << std::endl;
              memory[k].id = p[i].process_id;
              if( memory[k].size != p[i].mem_size )
              {
                memory[k+p[i].mem_size].id = '.';
                memory[k+p[i].mem_size].size = memory[k].size - p[i].mem_size;
                memory[k+p[i].mem_size].expires = -1;
              }
              memory[k].size = p[i].mem_size;
              memory[k].expires = time + p[i].run_time[j];
              printMemory( memory );
              break;
            }
            else
            {
              step = memory[k].size ;
            }
          }
          if( k == SIZE )
          {
            std::cout << "time " << time << "ms: Cannot place process " << p[i].process_id << " -- skipped!" << std::endl;
            skipped++;
          }
        }
      }
    }
    // removing
    for( int i = 0; i < SIZE; i += step )
    {
      if( memory[i].expires == time )
      {
        std::cout << "time " << time << "ms: Process " << memory[i].id << " removed:" << std::endl;
        memory[i].id = '.';
        memory[i].expires = -1;
        printMemory( memory );
        numRemoved++;
      }
      step = memory[i].size;
    }
    time++;
  }
  std::cout << "time " << time << "ms: Simulator ended (Contiguous -- Next-Fit)" << std::endl;
}
