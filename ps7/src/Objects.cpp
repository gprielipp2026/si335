#include "../inc/Objects.h"
/**
 * switch over to the Pos class 
 */

// Obstacle
// private

// public
Obstacle::Obstacle(int row, int col)
{
  pos = Pos{row, col};
}
// copy constructor
Obstacle::Obstacle(const Obstacle& other)
{
  std::cerr << "[Obstacle] copy constructor" << std::endl;
  pos = other.pos;
}

// access member fields safely
int Obstacle::getRow() { return pos.row; }
int Obstacle::getCol() { return pos.col; }

// reads in from the file
std::vector<Obstacle*>* Obstacle::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<Obstacle*>* data = new std::vector<Obstacle*>(num);

  while (num-- > 0)
  {
    int row, col;
    is >> row >> col;
    data->push_back( new Obstacle(row, col) );
  }

  return data;
}

// outputs data to the output stream
std::ostream& operator<< (std::ostream& os, Obstacle& obstacle)
{
  os << "[Obstacle] " << obstacle.getRow() << " " << obstacle.getCol();
  return os;
}

// Asset
// private

// public 
Asset::Asset(int row, int col, int id)
{
  pos = Pos{row, col};
  this->id  = id;
}
// copy constructor
Asset::Asset(const Asset& other)
{
  std::cerr << "[Asset] copy constructor" << std::endl;
  pos = other.pos;
  id = other.id;
}

// access member fields safely
int Asset::getRow() { return pos.row; }
int Asset::getCol() { return pos.col; }


std::vector<Asset*>* Asset::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<Asset*>* data = new std::vector<Asset*>(num);

  int id = 1;
  while (num-- > 0)
  {
    int row, col;
    is >> row >> col;
    data->push_back( new Asset(row, col, id) );
    id ++;
  }

  return data;
}

// outputs data to the output stream
std::ostream& operator<< (std::ostream& os, Asset& asset)
{
  os << "[Asset] " << asset.id << ": " << asset.getRow() << " " << asset.getCol();
  return os;
}

// Target
// private

// public
Target::Target(int row, int col, int points, char label)
{
  pos = Pos{row, col};
  this->points = points;
  this->label  = label;
}

// Copy constructor
Target::Target(const Target& other)
{
  std::cerr << "[Target] copy constructor" << std::endl;
  pos = other.pos;
  label = other.label;
  points = other.points;
}

// access member fields safely
int Target::getRow() { return pos.row; }
int Target::getCol() { return pos.col; }

std::vector<Target*>* Target::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num; 

  std::vector<Target*>* data = new std::vector<Target*>(num);

  char stop = 'A' + num;
  for(char id = 'A'; id < stop; id++) 
  {
    int row, col, points;
    is >> row >> col >> points;
    data->push_back( new Target(row, col, points, id) );
  }

  return data;
}

// outputs data to the output stream
std::ostream& operator<< (std::ostream& os, Target& target)
{
  os << "[Target] " << target.label << ": " << target.getRow() << " " << target.getCol() << " for " << target.points << " points";
  return os;
}

