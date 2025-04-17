#include "../inc/Objects.h"

// Obstacle
// private

// public
Obstacle::Obstacle(int row, int col)
{
  this->row = row;
  this->col = col;
}

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

// Asset
// private

// public 
Asset::Asset(int row, int col, int id)
{
  this->row = row;
  this->col = col;
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

// Target
// private

// public
Target::Target(int row, int col, int points, char label)
{
  this->row    = row;
  this->col    = col;
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


