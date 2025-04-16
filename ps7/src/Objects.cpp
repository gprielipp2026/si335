#include "../inc/Objects.h"

// Obstacle
std::vector<Obstacle*> Obstacle::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<Obstacle*> data(num);
  
  while (num-- > 0)
  {
    int row, col;
    file >> row >> col;
    data.append( new Obstacle(row, col) )
  }

  return data;
}

// Asset
std::vector<Asset*> Asset::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<Asset*> data(num);

  while (num-- > 0)
  {
    int row, col;
    file >> row >> col;
    data.append( new Asset(row, col) )
  }

  return data;
}

// Target
std::vector<Target*> Target::read(std::istream& is)
{
  // read in the number of items
  unsigned int num;
  is >> num;

  std::vector<Target*> data(num);
 
  for(char label = 'A'; label < 'A' + num; label++) 
  {
    int row, col, points;
    file >> row, col, points;
    data.append( new Target(row, col, points, label) )
  }

  return data;
}


