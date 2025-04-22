#include "../inc/Objects.h"

/**
 * switch over to the Pos class 
 */


// Position
// private

// public
Pos::Pos(int row, int col) 
{
  std::cerr << "[Pos] initialized with values: (" << row << ", " << col << ")" << std::endl;
  this->row = row;
  this->col = col;
}

// get attributes
int Pos::getRow() 
{ 
  std::cerr << "[Pos::getRow] row = " << this->row << std::endl;
  return this->row; 
}
int Pos::getCol() 
{ 
  std::cerr << "[Pos::getCol] col = " << this->col << std::endl;
  return this->col; 
}

// output
//std::ostream& operator<< (std::ostream& os, Pos& pos)
//{
  //os << "[Pos] " << pos.row << " " << pos.col;
  //return os;
//}
//// input
//std::istream& operator>> (std::istream& is, Pos& pos)
//{
  //is >> pos.row >> pos.col;
  //return is;
//}

// Obstacle
// private

// public
Obstacle::Obstacle(int row, int col)
  : Pos{row, col} {  }

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
  : Pos{row, col}
{
  this->id  = id;
}

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
  : Pos{row, col}
{
  this->points = points;
  this->label  = label;
}

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

