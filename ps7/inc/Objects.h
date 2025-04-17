#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <iostream>
#include <memory>

// Obstacles
class Obstacle
{
private:
  int row, col; // the position
  
public:
  Obstacle(int row, int col);
  static std::vector<std::shared_ptr<Obstacle>> read(std::istream& is); 
};

// Assets
class Asset
{
private:
  int row, col; // the position
  int id;       // unique numerical identifier
  
public:
  Asset(int row, int col, int id);
  static std::vector<std::shared_ptr<Asset>> read(std::istream& is); 
};

// Targets
class Target
{
private:
  int row, col; // the position
  char label;   // unique alphabetical identifier
  int points;   // value earned for reaching this target 
  
public:
  Target(int row, int col, int points, char label); 
  static std::vector<std::shared_ptr<Target>> read(std::istream& is); 
};

#endif//OBJECTS_H
