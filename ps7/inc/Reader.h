#ifndef READER_H
#define READER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <exception>

#include "Objects.h"

typedef struct
{
  unsigned int rows, cols;
  std::vector<Obstacle*>* obstacles;
  std::vector<Asset*>*    assets;
  std::vector<Target*>*   targets;
} Data;

Data* read(std::string filename); // read necessary data from a file
void  cleanup(Data* data);        // free all memory associated with the data

#endif//READER_H
