#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>

#define SIZE 256
#define ROW 32
#define T_MEMMOVE 1 // Time it takes to move one frame

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
int is_enoughRoom( mem_loc m[], int );
void runNextFit ( my_proccess_t p[], int );
void runBestFit ( my_proccess_t p[], int );
void runWorstFit( my_proccess_t p[], int );

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
  runNextFit( p, numberProcesses );
  //runBestFit( p, numberProcesses );
  return;
  //runWorstFit( p, numberProcesses );
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

// Checks if there is enough room for the memory to take the proccess
int is_enoughRoom( mem_loc m[SIZE], int mem_size )
{
  int size = 0, step = 1;
  for( int i = 0; i < SIZE; i+= step )
  {
    if( m[i].id == '.' ) size += m[i].size;
    step = m[i].size;
  }
  if( size == 0 || size < mem_size ) return 0; // false
  return 1; // true
}

void runNextFit( my_proccess_t p[], int size )
{
  int time = 0, numRemoved = 0, skipped = 0;
  mem_loc memory[ SIZE ];
  memory[0].id = '.';
  memory[0].size = SIZE;
  memory[0].expires = -1;
  std::cout << "time " << time << "ms: Simulator started (Contiguous -- Next-Fit)" << std::endl;
  int step = 1, defrag_time = 0;
  while( (numRemoved + skipped) != size )
  {
    // adding
    for( int i = 0; i < size; i++ )
    {
      for( int j = 0; j < p[i].count; j++)
      {
        if( (p[i].arrival_time[j]+defrag_time) == time )
        {
          std::cout << "time " << time << "ms: Process " << p[i].process_id << " arrived (requires " << p[i].mem_size << " frames)" << std::endl;
          int k;
          for( k = 0; k < SIZE; k += step)
          {
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
              step = memory[k].size;
            }
          }
          if( k == SIZE )
          {
            if( is_enoughRoom( memory, p[i].mem_size ) )
            {
              int freeIndex = 0, moved = 0;
              std::string procs_moved;
              std::cout << "time " << time << "ms: Cannot place process " << p[i].process_id << " -- starting defragmentation" << std::endl;
              for( int i = 0; i < SIZE; i += step )
              {
                step = memory[i].size;
                if( memory[i].id != '.' && freeIndex == i )
                {
                  freeIndex += memory[i].size;
                  // moved += 0;
                }
                else if( memory[i].id != '.' && freeIndex != i )
                {
                  if( procs_moved.length() != 0 ) procs_moved += ", ";
                  procs_moved += memory[i].id;
                  memory[freeIndex].id = memory[i].id;
                  memory[freeIndex].size = memory[i].size;
                  memory[freeIndex].expires = memory[i].expires;
                  freeIndex += memory[i].size;
                  moved += memory[i].size;
                }
              }
              memory[freeIndex].id = p[i].process_id;
              memory[freeIndex].size = p[i].mem_size;
              memory[freeIndex].expires = p[i].run_time[j];
              time += moved;
              defrag_time += (moved*T_MEMMOVE);
              std::cout << "time " << time << "ms: Defragmentation complete (moved " << moved << " frames: " << procs_moved << ")" << std::endl;
              printMemory( memory );
              std::cout << "time " << time << "ms: Placed process " << p[i].process_id << std::endl;
              freeIndex += p[i].mem_size;
              if( freeIndex != SIZE )
              {
                memory[freeIndex].id = '.';
                memory[freeIndex].size = SIZE - freeIndex;
                memory[freeIndex].expires = -1;
              }
            }
            else
            {
              std::cout << "time " << time << "ms: Cannot place process " << p[i].process_id << " -- skipped!" << std::endl;
              skipped++;
            }
            printMemory( memory );
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
