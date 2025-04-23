#include "../inc/Reader.h"

/**
 * Pull out the constituent parts for the problem
 * Store the data for the user in a convenient pointer
 */
Data* read(std::string filename)
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
    std::vector<Obstacle*>* obstacles = Obstacle::read(file);
    std::cerr << "[read] obstacles assigned" << std::endl; 
    std::vector<Asset*>*    assets    = Asset::read(file);
    std::cerr << "[read] assets assigned" << std::endl; 
    std::vector<Target*>*   targets   = Target::read(file);
    std::cerr << "[read] targets assigned" << std::endl; 
  
    // allocate the pointer - user will have to free it
    Data* data = (Data*)malloc(sizeof(*data));

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

/**
 * Free all of the fields in data, then free data
 */
void cleanup(Data* data)
{
  // clear the obstacles
  for(Obstacle* i : *data->obstacles)
  {
    delete i;
  }
  delete data->obstacles;
  // clear the assets 
  for(Asset* i : *data->assets)
  {
    delete i;
  }
  delete data->assets;
  // clear the targets 
  for(Target* i : *data->targets)
  {
    delete i;
  }
  delete data->targets;

  // free the Data struct
  free(data);
}
