#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <iostream>
#include <memory>

// Object that stores a Position
typedef struct
{
  int row, col;
} Pos;

// Obstacles
class Obstacle 
{
private:
  Pos pos; 
public:
  Obstacle(int row, int col);
  Obstacle(const Obstacle& other); // copy constructor
  /*Obstacle(Obstacle&&      other); // move constructor*/

  int getRow();
  int getCol();

  static std::vector<Obstacle*>* read(std::istream& is); 
  friend std::ostream& operator<< (std::ostream& os, Obstacle& obstacle);
};

// Assets
class Asset
{
private:
  Pos pos; 
  int id;  // unique numerical identifier
   
public:
  Asset(int row, int col, int id);
  Asset(const Asset& other); // copy constructor
  /*Asset(Asset&&      other); // move constructor*/
  
  int getRow();
  int getCol();

  int getId();

  static std::vector<Asset*>* read(std::istream& is); 
  friend std::ostream& operator<< (std::ostream& os, Asset& asset);
};

// Targets
class Target
{
private:
  Pos pos;
  char label; // unique alphabetical identifier
  int points; // value earned for reaching this target 
  
public:
  Target(int row, int col, int points, char label); 
  Target(const Target& other); // copy constructor
  /*Target(Target&&      other); // move constructor*/
  
  int getRow();
  int getCol();

  char getLabel();
  int  getPoints();
  void updatePoints(int value);

  static std::vector<Target*>* read(std::istream& is); 
  friend std::ostream& operator<< (std::ostream& os, Target& target);
};

#endif//OBJECTS_H
