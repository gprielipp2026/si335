#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>
#include "../inc/Reader.h"

#define MAX_ASSETS 3

using namespace std;

typedef union
{
  char dir[MAX_ASSETS] = {0}; 
  struct {
    // would need to change this to remap the variables based on MAX_ASSETS
    char a, b, c;
  };

  
} Move;
ostream& operator<<(ostream& os, Move& m)
{
  for(int i = 0; i < MAX_ASSETS; i++)
  {
    os << m.dir[i];
  } 
  return os;
}


vector<Move> find_moves(Data* data);

int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    cerr << "usage: " << argv[0] << " <input map>" << std::endl;
    return EXIT_SUCCESS;
  }
  
  // parse the file
  Data* data = read(argv[1]);
  
  // run the algorithm
  vector<Move> moves = find_moves(data);
 
  // testing purposes: TODO REMOVE
  cerr << "Obstacles:" << endl;
  for(Obstacle* obstacle : data->obstacles)
  {
    cerr << (*obstacle) << endl;
  }
  cerr << endl << "Assets:" << endl;
  for(Asset* asset : data->assets)
  {
    cerr << (*asset) << endl;
  }
  cerr << endl << "Targets:" << endl;
  for(Target* target : data->targets)
  {
    cerr << (*target) << endl;
  }
  // end TODO

  // output the moves  
  for(Move m : moves)
  {
    cerr << m << endl;
  } 

  cleanup(data);

  return EXIT_SUCCESS;
}


// heuristic for determining who is closer
int heuristic(Pos a, Pos b)
{
  return 0;
}


// main algorithm
vector<Move> find_moves(Data* data)
{
  vector<Move> moves; 
    
  /**
   * For each asset:
   *   Choose the closest target until all targets are exhausted 
   */ 



  return moves;
}
