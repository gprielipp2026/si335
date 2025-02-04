/**
 * George Prielipp (265112)
 * Uses selection sort to order the brigade companies
 */
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <list>

// I don't think I actually am using this
// baseline parameters
#define MAXROOMS 1_000_000
#define MAX_NAME 40
#define MIN_COMP 1
#define MAX_COMP 100

// what to store in the array
struct Room
{
  int company = -1;
  std::string name = "emptyROOM";

  Room(int c, std::string n) : company(c), name(n) {}
};

// to read in from stdin
std::vector<Room> read_brigade(std::istream& is);
// to print brigade for testing
void print_brigade(const std::vector<Room>& brigade);

// use my sort to order the brigade
void sort(std::vector<Room>& brigade);


int main(int argc, char* argv[])
{
  // read in the whole brigade
  std::vector<Room> brigade = read_brigade(std::cin);
  //print_brigade(brigade);
  
  sort(brigade);
  
  //print_brigade(brigade); 
 
  return EXIT_SUCCESS;
}


// to read in from stdin
std::vector<Room> read_brigade(std::istream& is)
{
  std::vector<Room> brigade;

  int company;
  std::string name;

  while(is >> company >> name)
  {
    brigade.emplace_back(company, name);
  }

  return brigade;
}


// convert a Room to stdout
std::ostream& operator<< (std::ostream& os, Room& room)
{
  os << room.company << ' ' << room.name;
  return os;
}

// to print brigade for testing
void print_brigade(const std::vector<Room>& brigade)
{
  for(auto it = brigade.begin(); it != brigade.end(); it++)
  {
    Room room = *it;
    std::cout << room << std::endl;
  }
}


// for recording the swap (what we are being graded on)
void print_swap(Room& a, Room& b)
{
  std::cout << a.name << ' ' << b.name << std::endl;
}

// used to swap two elements
void swap(std::vector<Room>& brigade, int a, int b)
{
  print_swap(brigade[a], brigade[b]);
  Room temp = brigade[a];
  brigade[a] = brigade[b];
  brigade[b] = temp;
}


// use my sort to order the brigade
// trying merge sort
void sort(std::vector<Room>& brigade)
{
  std::map<int, std::list<int>> indices;
  // pass through the array and record the indices
  for(int i = 0; i < brigade.size(); i++)
  {
    int company = brigade[i].company;
    if (!indices.contains(company))
    {
      // need to make the new list
      indices.insert({company, std::list<int>()});
    }
    indices[company].push_back(i);
  }
 
  // execute the necessary swaps
  int target = brigade[0].company;
  indices[target].pop_front();

  for(int i = 1; i < brigade.size(); i++)
  {
    int cur = brigade[i].company;
    indices[cur].pop_front(); // = i
    // see if the target needs to change
    if(indices[target].empty())
    {
      target = cur;
    }
    // perform a swap if necessary
    if(cur != target)
    {
      // figure out where the swap needs to occur
      int tgtFront = indices[target].front();
      indices[target].pop_front();
      swap(brigade, i, tgtFront);
      // put the 'cur' value's new index into the list in order
      auto it = indices[cur].begin();
      auto end = indices[cur].end();
      while(*it < tgtFront && it != end) { it++; }
      indices[cur].insert(it, tgtFront);
    } 
  }  

}


