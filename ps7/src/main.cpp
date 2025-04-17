#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>
#include "../inc/Reader.h"
using namespace std;

typedef union
{
  char dir[3]; // can have up to 3 assets
  struct {
    char a, b, c;
  };
} Move;

vector<Move> find_moves(shared_ptr<Data> data);


int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    cerr << "usage: " << argv[0] << " <input map>" << std::endl;
    return EXIT_SUCCESS;
  }

  shared_ptr<Data> data = read(argv[1]);

  Move move;

  move.a = 'L';
  move.b = 'U';
  move.c = 'R';

  cerr << "Movement directions: ";
  for(int i = 0; i < 3; i++)
  {
    cerr << move.dir[i];
  }
  cerr << endl;
  

  return EXIT_SUCCESS;
}

vector<Move> find_moves(shared_ptr<Data> data)
{

  return vector<Move>();
}
