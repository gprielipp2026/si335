/**
 * everything will have to go in this file unfortunately
 */
#include <cstring>
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
  //vector<Pos*>    obstacles;
  bool** obstacles; // it is rows x cols 
  vector<Asset*>  assets;
  vector<Target*> targets;
} Board;

typedef union
{
  char moves[4] = {'\0','\0','\0','\0'}; // max of 3 assets
  struct
  {
    char a, b, c, end; // end should never be accessed
  };
} Move;

Board* read(istream& file);
vector<Move*> ALGORITHM_NAME(Board* board);
void output(ostream& os, vector<Move*> moves);
void cleanup(Board* board, vector<Move*> moves);

int main(int argc, char* argv[])
{
  string ifn;
  string ofn; 

  cout << "Map: ";
  cin >> ifn;
  cout << "Moves: ";
  cin >> ofn;

  ifstream ifile(ifn);
  if (!ifile)
  {
    cerr << "issue opening file '" << ifn << "'" << endl;
    return EXIT_FAILURE;
  }
  ofstream ofile(ofn);
  if(!ofile)
  {
    cerr << "issue opening file '" << ofn << "'" << endl;
    return EXIT_FAILURE;
  }

  // run the algorithm
  Board* board = read(ifile);
  
  vector<Move*> moves = ALGORITHM_NAME(board);
  output(ofile, moves);
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

  // initialize all of the board->obstacles to be false
  b->obstacles = (bool**)malloc(sizeof(bool*)*b->rows);
  for(int row = 0; row < b->rows; row++)
  {
    b->obstacles[row] = (bool*)malloc(sizeof(bool)*b->cols);
    for(int col = 0; col < b->cols; col++)
    {
      b->obstacles[row][col] = false;
    }
  }

  // read in the obstacles
  int numObstacles;
  file >> numObstacles;

  for(int i = 0; i < numObstacles; i++)
  {
    //Pos* p = (Pos*)malloc(sizeof(*p));
    //file >> p->row >> p->col;
    
    int row, col;
    file >> row >> col;
    b->obstacles[row][col] = true;
    
    //b->obstacles.push_back(p);
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
   * NEW TODO
   * incorporate total distance traveled into the heurstic to decrease the average penalty
   * per asset (ie all assets will travel roughly the same distance)
   */

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
    // get the closest based on the heuristic
    pair<int, Target*> closest[numassets];
    for(Asset* asset : board->assets)
    {
      // sort the targets based on the heuristic for a given asset
      //bool (* compare)(Target*, Target*) = [&asset] (Target* a, Target* b) -> bool { 
      auto compare = [&asset] (Target* a, Target* b) -> bool { 
        return heuristic(asset, a) < heuristic(asset, b);
      };
      sort(board->targets.begin(), board->targets.end(), compare);

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

      paths[asset].push_back(min.second);
      board->assets[asset]->pos = min.second->pos;
      remainingTargets.erase(min.second->label);

      // remove-erase idiom
      // remove moves the item (min.second) to the end, 
      // removes the element at the iterator (the end) 
      board->targets.erase(remove(board->targets.begin(), board->targets.end(), min.second), board->targets.end());
    }
    else if (countSame == 2 && numassets == 3)
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
      paths[posFalse].push_back(other.second);

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
      paths[idUsed].push_back(same.second);
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
        paths[i].push_back(cur.second); 
      }
    }
    }// end of while loop

    // generate the moves from the assigned paths   
    for(Asset* asset : assetCopies)
    {
      vector<Target*> path = paths[asset->id];

      int numSteps = 0;
      int aRow = asset->pos->row;
      int aCol = asset->pos->col;
      for (Target* target : path)
      {
        // get the asset to this target
        // TODO - obstacle avoidance
        // add a new move if at 
        int tRow = target->pos->row;
        int tCol = target->pos->col;
        while((aRow != tRow) || (aCol != tCol))
        {
          // move in the bigger direction
          int magRow = abs(aRow - tRow);
          int magCol = abs(aCol - tCol);
          if(magRow > magCol)
          {
            // determine the direction
            // can move UP or DOWN
            int direction = (tRow - aRow) / magRow;
            // add the move (if one exists in the vector, update it for the current asset)
            int testRow = aRow + direction;
            if(board->obstacles[testRow][aCol]) 
            {
              // move LEFT/RIGHT
              direction = (tCol - aCol) / magCol;
              // bad copy/pasted code, but whatever - it works already
              if(moves.size() == numSteps)
              {
                Move* move = (Move*)malloc(sizeof(*move));
                memset(move->moves, 0, sizeof(move->moves)); 
                move->moves[asset->id] = (direction < 0 ? 'L':'R');
                moves.push_back(move);
                aCol += direction;
              }
              else
              {
                moves[numSteps]->moves[asset->id] = (direction < 0 ? 'L':'R');
                aCol += direction;
              }
            } 
            else if(moves.size() == numSteps)
            {
              // need to add a new move
              Move* move = (Move*)malloc(sizeof(*move));
              memset(move->moves, 0, sizeof(move->moves)); 
              move->moves[asset->id] = (direction < 0 ? 'U':'D');
              moves.push_back(move);
              aRow += direction; 
            }
            else
            {
              // it already exists (update it)
              moves[numSteps]->moves[asset->id] = (direction < 0 ? 'U':'D');
              aRow += direction;
            }
          } 
          else
          {
            // if they're the same, it's going to favor horizontal movement
            // determine the direction
            // can move LEFT or RIGHT
            int direction = (tCol - aCol) / magCol;
            int testCol = aCol + direction;
            // add the move or update it
            if(board->obstacles[aRow][testCol])
            {
              // go UP/DOWN
              direction = (tRow - aRow) / magRow;
              // bad copy/pasted code, but whatever - it works already 
              if(moves.size() == numSteps)
              {
                // need to add a new move
                Move* move = (Move*)malloc(sizeof(*move));
                memset(move->moves, 0, sizeof(move->moves)); 
                move->moves[asset->id] = (direction < 0 ? 'U':'D');
                moves.push_back(move);
                aRow += direction; 
              }
              else
              {
                // it already exists (update it)
                moves[numSteps]->moves[asset->id] = (direction < 0 ? 'U':'D');
                aRow += direction;
              }
            }
            else if(moves.size() == numSteps)
            {
              Move* move = (Move*)malloc(sizeof(*move));
              memset(move->moves, 0, sizeof(move->moves)); 
              move->moves[asset->id] = (direction < 0 ? 'L':'R');
              moves.push_back(move);
              aCol += direction;
            }
            else
            {
              moves[numSteps]->moves[asset->id] = (direction < 0 ? 'L':'R');
              aCol += direction;
            }
          }
          // you took a step, update the count
          numSteps++;
        }
        // update the assets position -- done in the movement code
      }
    }

    // make sure each move has the same amount
    for(Move* move : moves)
    {
      for(int i = 0; i < numassets; i++)
      {
        if(move->moves[i] == '\0')
        {
          move->moves[i] = 'L';
        }
      }
    }

    // return computed paths
    return moves;
  } // this is actually the end of the function, somehow my autoformatter is messed up 

  void output(ostream& os, vector<Move*> moves)
  {
    for(Move* m : moves)
    {
      os << m->moves << endl;
    }
  }

  void cleanup(Board* board, vector<Move*> moves)
  {
    // obstacles
    for(int i = 0; i < board->rows; i++)
    {
      free(board->obstacles[i]);
    } 
    free(board->obstacles);

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

