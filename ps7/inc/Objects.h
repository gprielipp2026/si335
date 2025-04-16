#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <iostream>

/**
 * NOTE:
 * create constructor functions - private
 * add private fields
 */

// Obstacles
class Obstacle
{
private:
  
public:
  static std::vector<Obstacle*> read(std::istream& is); 
};

// Assets
class Asset
{
private:

public:
  static std::vector<Asset*> read(std::istream& is); 
};

// Targets
class Target
{
private:

public:
  static std::vector<Target*> read(std::istream& is); 
};

#endif//OBJECTS_H
