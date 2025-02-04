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
#include <utility>

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

// record how many times a company occurs in the brigade
std::map<int,int> countOccurrences(std::vector<Room>& brigade)
{
  std::map<int,int> counts;
  for(auto it = brigade.begin(); it != brigade.end(); it++)
  {
    int company = (*it).company;
    if(counts.contains(company)) counts[company] = counts[company] + 1;
    else counts[company] = 1;
  } 
  return counts;
}

// return the left and right index (int a std::pair) of the largest window
std::pair<int,int> largestWindow(std::vector<Room>& brigade, std::map<int,int> counts)
{
  int retLeft = -1, retRight = -1;
  
  // just found out you can do range-based loops in cpp (maps return std::pairs)
  for(const auto& pair : counts)
  {
    int key = pair.first;
    int val = pair.second;
    int left = -1, right = -1;
    // find left and right bounds for a given company
    for(int i = 0; i < brigade.size(); i++)
    {
      if(brigade[i].company == key)
      {
        if(left == -1) left = i;
        else right = i;
      }
    }
    // check the current window and see if it needs updated 
    if( (right - left) > (retRight - retLeft) && (right - left) + 1 != val )
    {
      // new bigger window
      retLeft = left;
      retRight = right;
    }
  }

  return std::make_pair(retLeft, retRight);
}

// heart of the program here:
// greedy portion is making the largest change possible (from one of the edges)
std::pair<int,int> findBestSwap(std::vector<Room>& brigade, int left, int right)
{
  int target = brigade[left].company;
  int changeLeft = 0, changeRight = 0;
  // find out how much we can get by moving the left in
  for(int i = left + 1; i < right; i++)
  {
    if(brigade[i].company == target)
    {
      changeLeft = i - left;
      break;
    }
  }
  // find out the right side
  for(int i = right - 1; i > left; i--)
  {
    if(brigade[i].company == target)
    {
      changeRight = right - i;
      break;
    }
  }

  // use the edge with the biggest change (greedy part)
  // find the longest sequence from the new edge
  // newEdge choice and how I find the swap point is what can be optimized
  int newEdge;
  // if the left side is better
  if (changeLeft > changeRight)
  {
    newEdge = left + changeLeft;
    for(int i = newEdge; i < right; i++)
    {
      if (brigade[i].company != target)
      {
        return std::make_pair(left, i);
      }
    }
    // if it got here, need to add to the outside edge (ie middle is full)
    return std::make_pair(left, newEdge-1);
  }
  // if the right side was better
  else
  {
    newEdge = right - changeRight;
    for(int i = left; i < newEdge; i++)
    {
      if(brigade[i].company != target)
      {
        return std::make_pair(i, right);
      }
    }
    // if it got here, need to add to the outside edge (ie middle is full)
    return std::make_pair(newEdge+1, right);
  }

  // should not make it here
  std::cout << "\n[findBestSwap] made it to a bad spot" << std::endl;
  return std::make_pair(-1, -1);
}

// use my sort to order the brigade
// came up with a greedy algorithm 
void sort(std::vector<Room>& brigade)
{
  std::map<int,int> counts = countOccurrences(brigade);
  for(const auto& pair : counts)
  while(true)
  {
    std::pair<int,int> edges = largestWindow(brigade, counts);
    int left = edges.first;
    int right = edges.second;
    if(left == -1 && right == -1) break; // exit condition = no overlapping windows (all sorted)
    std::pair<int,int> bestSwaps = findBestSwap(brigade, left, right);
    print_brigade(brigade);
    swap(brigade, bestSwaps.first, bestSwaps.second);
  }
} 
