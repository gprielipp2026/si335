/**
 * George Prielipp (265112)
 * Uses selection sort to order the brigade companies
 */
#include <iostream>
#include <string>
#include <array>
#include <vector>

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

// use selection sort to order the brigade
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


// use selection sort to order the brigade
void sort(std::vector<Room>& brigade)
{
  for(int i = 0; i < brigade.size()-1; i++)
  {
    // find the next element to swap into place
    int m = i;
    for(int j = i+1; j < brigade.size(); j++)
    {
      if (brigade[j].company < brigade[m].company)
      {
        m = j;
      } 
    }
    swap(brigade, i, m);
  }
}


