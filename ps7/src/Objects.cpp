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

// access member fields safely
int Obstacle::getRow() { return pos.row; }
int Obstacle::getCol() { return pos.col; }

// reads in from the file
std::vector<std::shared_ptr<Obstacle>> Obstacle::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<std::shared_ptr<Obstacle>> data(num);

  while (num-- > 0)
  {
    int row, col;
    is >> row >> col;
    data.emplace_back( std::make_unique<Obstacle>(row, col) );
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

// access member fields safely
int Asset::getRow() { return pos.row; }
int Asset::getCol() { return pos.col; }


std::vector<std::shared_ptr<Asset>> Asset::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<std::shared_ptr<Asset>> data(num);

  int id = 1;
  while (num-- > 0)
  {
    int row, col;
    is >> row >> col;
    data.emplace_back( std::make_unique<Asset>(row, col, id) );
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

// access member fields safely
int Target::getRow() { return pos.row; }
int Target::getCol() { return pos.col; }

std::vector<std::shared_ptr<Target>> Target::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num; 

  std::vector<std::shared_ptr<Target>> data(num);

  char stop = 'A' + num;
  for(char id = 'A'; id < stop; id++) 
  {
    int row, col, points;
    is >> row >> col >> points;
    data.emplace_back( std::make_unique<Target>(row, col, points, id) );
  }

  return data;
}

// outputs data to the output stream
std::ostream& operator<< (std::ostream& os, Target& target)
{
  os << "[Target] " << target.label << ": " << target.getRow() << " " << target.getCol() << " for " << target.points << " points";
  return os;
}

