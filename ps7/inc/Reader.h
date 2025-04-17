#ifndef READER_H
#define READER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <exception>
#include <memory>

#include "Objects.h"

typedef struct
{
  unsigned int rows, cols;
  std::vector<std::shared_ptr<Obstacle>> obstacles;
  std::vector<std::shared_ptr<Asset>>    assets;
  std::vector<std::shared_ptr<Target>>   targets;
} Data;

std::shared_ptr<Data> read(std::string filename); // read necessary data from a file

#endif//READER_H
