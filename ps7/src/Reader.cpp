#include "../inc/Reader.h"

/**
 * Pull out the constituent parts for the problem
 * Store the data for the user in a convenient pointer
 */
std::shared_ptr<Data> read(std::string filename)
{
  // construct the input stream
  std::ifstream file(filename); 
  if (!file)
  {
    std::cerr << "Error opening file '" << filename << "'" << std::endl;
    return NULL;
  }  

  std::string junk; // read in the empty lines

  // read in the dimensions: rows cols
  unsigned int rows, cols;
  file >> rows >> cols; 
  
  std::cerr << "[read] " << rows << " x " << cols << " sized board" << std::endl;

  // read in the obstacles
  try
  {
    std::vector<std::shared_ptr<Obstacle>> obstacles = Obstacle::read(file);
    std::cerr << "[read] obstacles assigned" << std::endl; 
    std::vector<std::shared_ptr<Asset>>    assets    = Asset::read(file);
    std::cerr << "[read] assets assigned" << std::endl; 
    std::vector<std::shared_ptr<Target>>   targets   = Target::read(file);
    std::cerr << "[read] targets assigned" << std::endl; 
  
    // allocate the pointer - user will have to free it
    std::shared_ptr<Data> data = std::make_shared<Data>();

    // assign the data and return it
    data->rows      = rows;
    data->cols      = cols;
    data->obstacles = obstacles;
    data->assets    = assets;
    data->targets   = targets;

    // all set
    return data;
  }
  catch(std::exception& e)
  {
    std::cerr << "[ERROR] Parsing the file failed: " << e.what() << std::endl;
    return NULL;
  }

  // in case it gets here somehow
  return NULL; 
}

