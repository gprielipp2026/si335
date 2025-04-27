/**
 * everything will have to go in this file unfortunately
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <utility>
#include <set>

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
  output(moves);
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


  // a copy of the assets (since I'm going to change their positions for the algorithm)
  vector<Asset*> assetCopies;
  for(Asset* asset : board->assets)
  {
    Asset* copy = (Asset*)malloc(sizeof(*copy));
    copy->pos = (Pos*)malloc(sizeof(*copy->pos));
    copy->pos->row = asset->pos->row;
    copy->pos->col = asset->pos->col;
    copy->id = asset->id;
    assetCopies.push_back(copy);
  } // TODO - clean up at end (or don't, I don't care)


  int numtgts = board->targets.size();
  int numassets = board->assets.size();
  unordered_map<int, vector<Target*>> paths;
  for(int id = 0; id < numassets; id++)
  {
    paths.insert(make_pair<int, vector<Target*>>(static_cast<int>(id), vector<Target*>()));
  } 

  set<char> remainingTargets;
  for(char l = 'A'; l < 'A' + numtgts; l++)
  {
    remainingTargets.insert(l);
  }

  // Main loop to find the targets
  while(!remainingTargets.empty())
  {
    cerr << "[ALGORITHM] targets remaining assignment = " << remainingTargets.size() << endl;

    // get the closest based on the heuristic
    pair<int, Target*> closest[numassets];
    for(Asset* asset : board->assets)
    {
      // sort the targets based on the heuristic for a given asset
      //bool (* compare)(Target*, Target*) = [&asset] (Target* a, Target* b) -> bool { 
      auto compare = [&asset] (Target* a, Target* b) -> bool { 
        return heuristic(asset, a) > heuristic(asset, b);
      };
      std::sort(board->targets.begin(), board->targets.end(), compare);
      int score = heuristic(asset, board->targets[0]);
      closest[asset->id] = make_pair<int, Target*>(static_cast<int>(score), static_cast<Target*>(board->targets[0]));
    }

    // deconflict the closest
    int countSame = 0;
    bool cases[numassets] = {false}; 
    for(Asset* asset : board->assets)
    {
      for(int i = 0; i < numassets; i++)
      {
        if(closest[asset->id].second == closest[i].second && asset->id != i && !cases[asset->id])
        {
          countSame++;
          cases[asset->id] = true;
          cases[i] = true;
        }
      }
    }
    
    // assign the target to an asset
    // update the asset->pos = target->pos // "moves the asset to the target"
    // remove the target from the list 
    if(countSame == numassets)
    {
      // all the same
      // have to find the best one
      
      pair<int, Target*> min = closest[0];
      int asset = 0;
      for(int i = 0; i < numassets; i++)
      {
        if (closest[i].first < min.first) 
        {
          asset = i;
          min = closest[i];
        }
      }

      board->assets[asset]->pos = min.second->pos;
      remainingTargets.erase(min.second->label);

      // remove-erase idiom
      // remove moves the item (min.second) to the end, 
      // removes the element at the iterator (the end) 
      board->targets.erase(remove(board->targets.begin(), board->targets.end(), min.second), board->targets.end());
    }
    else if (countSame == numassets - 1)
    {
      // 1 different
      // have to find the two that are the same and choose the best one
      // this can only happen when numassets == 3
      int posTrue1, posTrue2, posFalse; 
      for(int i = 0; i < numassets; i++)
      {
        if(!cases[i]) posFalse = i;
      } 
      posTrue1 = (posFalse + 1) % numassets;
      posTrue2 = (posFalse + 2) % numassets;

      // add the "outsider" - the different one
      pair<int, Target*> other = closest[posFalse];
      board->assets[posFalse]->pos = other.second->pos;
      remainingTargets.erase(other.second->label);
      board->targets.erase(remove(board->targets.begin(), board->targets.end(), other.second), board->targets.end());

      // find the smallest same value to add
      pair<int, Target*> same;
      int idUsed;
      if(closest[posTrue1].first > closest[posTrue2].first)
      {
        same = closest[posTrue2];
        idUsed = posTrue2;
      } 
      else
      {
        same = closest[posTrue1];
        idUsed = posTrue1;
      } 
      board->assets[idUsed]->pos = same.second->pos;
      remainingTargets.erase(same.second->label);
      board->targets.erase(remove(board->targets.begin(), board->targets.end(), same.second), board->targets.end());
    
    }
    else
    {
      // all different
      // can add them all
      for(int i = 0; i < numassets; i++)
      {
        pair<int, Target*> cur = closest[i];
        board->assets[i]->pos = cur.second->pos;
        remainingTargets.erase(cur.second->label);
        board->targets.erase(remove(board->targets.begin(), board->targets.end(), cur.second), board->targets.end());
      }
    }
  }

  // return computed paths
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

