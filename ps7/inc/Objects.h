#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <iostream>
#include <memory>

// Object that stores a Position
class Pos
{
private:
  int row, col; 
public:
  Pos(int row, int col);
 
  int getRow(); // return the row
  int getCol(); // return the col

  //friend std::ostream& operator<< (std::ostream& os, Pos& pos);
  //friend std::istream& operator>> (std::istream& is, Pos& pos); // may not use
};

// Obstacles
class Obstacle : public Pos
{
private:
  
public:
  Obstacle(int row, int col);

  static std::vector<std::shared_ptr<Obstacle>> read(std::istream& is); 
  friend std::ostream& operator<< (std::ostream& os, Obstacle& obstacle);
};

// Assets
class Asset : Pos
{
private:
  int id;  // unique numerical identifier
  
public:
  Asset(int row, int col, int id);
  
  int getId();

  static std::vector<std::shared_ptr<Asset>> read(std::istream& is); 
  friend std::ostream& operator<< (std::ostream& os, Asset& asset);
};

// Targets
class Target : Pos
{
private:
  char label; // unique alphabetical identifier
  int points; // value earned for reaching this target 
  
public:
  Target(int row, int col, int points, char label); 

  char getLabel();
  int  getPoints();
  void updatePoints(int value);

  static std::vector<std::shared_ptr<Target>> read(std::istream& is); 
  friend std::ostream& operator<< (std::ostream& os, Target& target);
};

#endif//OBJECTS_H
