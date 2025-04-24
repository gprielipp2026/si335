/**
 * everything will have to go in this file unfortunately
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

typedef struct
{
  int row, col;
} Pos;

typedef struct
{
  Pos* pos;
  int id;
} Asset;

typedef struct
{
  Pos* pos;
  char label;
  int points;
} Target;

typedef struct
{
  int rows, cols;
  vector<Pos*>    obstacles;
  vector<Asset*>  assets;
  vector<Target*> targets;
} Board;

typedef union
{
  char moves[3] = {0}; // max of 3 assets
  struct
  {
    char a, b, c;
  };
} Move;

Board* read(istream& file);
vector<Move*> ALGORITHM_NAME(Board* board);
void output(vector<Move*> moves);
void cleanup(Board* board, vector<Move*> moves);

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    cerr << "usage: " << argv[0] << " <map file>" << endl;
    return EXIT_FAILURE;
  }

  ifstream file(argv[1]);
  if (!file)
  {
    cerr << "issue opening file '" << argv[1] << "'" << endl;
    return EXIT_FAILURE;
  }
  
  // run the algorithm
  Board* board = read(file);
  
  cerr << "Obstacles:" << endl;
  for(Pos* o : board->obstacles)
  {
    cerr << o->row << " " << o->col << endl;
  } 
  cerr << endl;
  cerr << "Assets:" << endl;
  for(Asset* a : board->assets)
  {
    cerr << a->id << ": " << a->pos->row << " " << a->pos->col << endl;
  }
  cerr << endl;
  cerr << "Targets:" << endl;
  for(Target* t : board->targets)
  {
    cerr << t->label << ": " << t->pos->row << " " << t->pos->col << " for " << t->points << " points" << endl;
  }
  cerr << endl;
  
  vector<Move*> moves = ALGORITHM_NAME(board);
  output();
  cleanup(board, moves);

  return EXIT_SUCCESS;
}

/**
 * reformats a file into a struct
 */
Board* read(istream& file)
{
  // construct the board object
  Board* b = (Board*)malloc(sizeof(*b));
  
  // read in the rows x cols
  file >> b->rows >> b->cols;

  // read in the obstacles
  int numObstacles;
  file >> numObstacles;
  for(int i = 0; i < numObstacles; i++)
  {
    Pos* p = (Pos*)malloc(sizeof(*p));
    file >> p->row >> p->col;
    
    b->obstacles.push_back(p);
  }

  // read in the assets
  int numAssets;
  file >> numAssets;
  for(int id = 0; id < numAssets; id++)
  {
    Asset* a = (Asset*)malloc(sizeof(*a));
    Pos*   p = (Pos*)malloc(sizeof(*p));
    file >> p->row >> p->col;
    a->pos = p;
    a->id = id;
    
    b->assets.push_back(a);
  }

  // read in the targets
  int numTargets;
  file >> numTargets;
  for(char label = 'A'; label < 'A'+numTargets; label++)
  {
    Target* t = (Target*)malloc(sizeof(*t));
    Pos* p = (Pos*)malloc(sizeof(*p));
    file >> p->row >> p->col >> t->points;
    t->pos = p;
    t->label = label; 
    b->targets.push_back(t);
  }

  return b; 
}

Move* move_create(char moves[3])
{
  Move* m = (Move*)malloc(sizeof(*m));
  m->a = moves[0];
  m->b = moves[1];
  m->c = moves[2]; 
  return m;
}

/**
 * provide a heuristic to sort the options for a decision
 */
int heuristic(Asset* a, Target* b)
{
  // max points is 1000
  // store distance score past 1000
  // distance and pointscore optimizing in different directions though...
  int distance = abs(a->pos->row - b->pos->row) + abs(a->pos->col - b->pos->col);
  int pointscore = b->points - distance;
  if (pointscore < 0) pointscore = 0;
  
  // "matches" pointscore and distance to optimize in the same direction
  pointscore = 1000 - pointscore;

  return distance * 10000 + pointscore;
}

vector<Move*> ALGORITHM_NAME(Board* board)
{
  vector<Move*> moves;
 
  /**
   * TODO algorithm
   * for each asset:
   *  choose the order of the targets based on the heuristic
   *
   * deconflict targets (time permitting)
   *  ie assets don't need to visit all targets, especially if target point is 0
   *
   * compute paths to each target and save that as moves
   *
   * return computed paths
   */ 
  
  return moves;
}

void output(vector<Move*> moves)
{
  for(Move* m : moves)
  {
    cerr << m->moves << endl;
  }
}

void cleanup(Board* board, vector<Move*> moves)
{
  // obstacles
  for(Pos* o : board->obstacles)
  {
    free(o);
  }
  // assets
  for(Asset* a : board->assets)
  {
    free(a->pos);
    free(a);
  }
  // targets
  for(Target* t : board->targets)
  {
    free(t->pos);
    free(t);
  } 
  // clean the board
  free(board); 
  // moves
  for(Move* m : moves)
  {
    free(m);
  }
}

