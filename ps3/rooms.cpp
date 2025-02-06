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
#include <functional>

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
// it's frustrated at me
std::ostream& operator<< (std::ostream& os, const Room& room)
{
  os << room.company << ' ' << room.name;
  return os;
}

// to print brigade for testing
void print_brigade(const std::vector<Room>& brigade)
{
  int i = 0;
  for(auto it = brigade.begin(); it != brigade.end(); it++)
  {
    Room room = *it;
    std::cout << i++ << ": " << room << std::endl;
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
std::map<int,int> countOccurrences(const std::vector<Room>& brigade)
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

// find the left and right bounds for all elements in the list 
std::map<int,std::pair<int,int>> calcWindows(const std::vector<Room>& brigade, const std::map<int,int>& counts)
{
  std::map<int,std::pair<int,int>> windows;
   
  for(auto it = counts.begin(); it != counts.end(); it++)
  {
    int key = (*it).first;
    //std::cout << "looking for key = " << key << std::endl;
    
    // look through the list for a left and a right bound
    int left = -1;
    int right = -1;
    int li = 0;
    int ri = brigade.size()-1;
    // guarantee it exits eventually
    while( left == -1 || right == -1 )
    {
      if(li < brigade.size() && brigade[li].company == key && left == -1)
      {
        left = li;
      }
      if(ri >= 0 )
      {
        //std::cout << "ri = " << ri << ": " << brigade[ri] << std::endl;
        if(brigade[ri].company == key)
          if(right == -1)
            right = ri;
      }
      li++;
      ri--;
    }
    // window was found
    //std::cout << "company (" << key << ") = " << left << " -> " << right << std::endl;
    windows[key] = std::make_pair(left, right);
  }

  return windows;
}

// count up how many other window edges are inside each window
std::map<int, int> calcIntersections(const std::map<int,std::pair<int,int>>& windows)
{
  std::map<int,int> intersections;

  // go through all of the companies
  for(const auto& pair1: windows)
  {
    int company = pair1.first;
    std::pair<int,int> checkWindow = pair1.second;
    int count = 0;
    for(const auto& pair2 : windows)
    {
      std::pair<int,int> window = pair2.second;
      // check if left is within
      if(window.first > checkWindow.first && window.first < checkWindow.second) count++;
      // check if right is within
      if(window.second > checkWindow.first && window.second < checkWindow.second) count++;
    }
    intersections[company] = count;
  }

  return intersections;
}

// check to see if all intersection counts are 0
bool done(const std::map<int,int> intersections)
{
  bool ret = true;
  for(const auto& pair : intersections)
  {
    if(pair.second != 0) ret = false;
  }
  return ret;
}

/** crucial part in making the greediest choices later
*   need to find two windows that:
*   1. have the highest # of intersections 
*   2. intersect each other
*   - break a tie for second window - (more steps are if previous step fails)
*   1. the one with fewer counts
*   2. the one with a smaller window
*   3. at random (just choose one) 
*/
std::pair<std::pair<int,int>,std::pair<int,int>> getBestMoves(const std::vector<Room>& brigade, 
                                                              const std::map<int,int>& counts, 
                                                              const std::map<int,std::pair<int,int>>& windows, 
                                                              const std::map<int,int>& intersections)
{
  // make a list to sort by # intersections in descending order
  std::vector<std::pair<int,int>> sortedIntersections (intersections.begin(), intersections.end());
  std::sort(sortedIntersections.begin(), sortedIntersections.end(), [](std::pair<int,int> a, std::pair<int,int> b) -> bool { return a.second > b.second; });

  for(int i = 0; i < sortedIntersections.size(); i++)
  {
    const std::pair<int,int> intersection = sortedIntersections.at(i);
    //std::cout << "company (" << intersection.first << ") intersected " << intersection.second << " times" << std::endl;
  }
  //std::cout << "the thing causing me trouble is: " << sortedIntersections[0].first << " " << sortedIntersections[0].second << std::endl; 
  std::pair<int,int> highest = windows.at( sortedIntersections[0].first );
  //std::cout << "did not fail at this line" << std::endl;
  std::pair<int,int> nextHighest = std::make_pair(-1,-1);

  // helper function that didn't feel like a full definition
  std::function<bool(std::pair<int,int>,std::pair<int,int>)> intersects = [](std::pair<int,int> win1, std::pair<int,int> win2) -> bool
  {
    // see if win2 has any edge inside win1
    return (win1.first > win2.first && win1.first < win2.second) || (win1.second > win2.first && win1.second < win2.second);  
  }; 

  // return how much distance a window spans
  // it is how many elements are stored within the window
  std::function<int(std::pair<int,int>)> size = [](std::pair<int,int> window) -> int
  {
    return window.second - window.first + 1; 
  };

  // go through the remaining intersections to see if they're a fit
  //std::cout << "highest: " << highest.first << " -> " << highest.second << std::endl;
  for(int i = 1; i < sortedIntersections.size(); i++)
  {
    if(sortedIntersections[1].first != sortedIntersections[i].first) break; // no longer looking at the same value
    if(intersects(windows.at(sortedIntersections[i].first), highest))
    {
      nextHighest = windows.at(sortedIntersections[i].first);
      
      // check for any ties 
      for(int j = i; j < sortedIntersections.size(); j++)
      {
        // ensure there even is a tie
        if( sortedIntersections[i].second != sortedIntersections[j].second ) break;
        if( !intersects(windows.at(sortedIntersections[j].first), highest) ) continue;
        // pick the one with fewer counts
        // need to use the company here
        if( counts.at(brigade.at(nextHighest.first).company) > counts.at(sortedIntersections[j].first))
        {
          nextHighest = windows.at( sortedIntersections[j].first );
        }
        else if( counts.at(brigade.at(nextHighest.first).company) == counts.at(sortedIntersections[j].first) )
        {
          // need to break second tie
          if( size(nextHighest) > size(windows.at(sortedIntersections[j].first)) )
          {
            nextHighest = windows.at( sortedIntersections[j].first );
          }
          else if( size(nextHighest) == size(windows.at(sortedIntersections[j].first)) )
          {
            // need to break the third tie
            // just pick something
            nextHighest = windows.at( sortedIntersections[j].first );
          }
        }
 
      }
    }
  }

  // if nextHighest is still not found (that means the highest fully contains exactly one other window) (need to find that one window)
  if(nextHighest.first == -1 && nextHighest.second == -1)
  {
    for(int i = 0; i < sortedIntersections.size(); i++)
    {
      std::pair<int,int> intersection = sortedIntersections[i];
      if(intersects(windows.at(intersection.first), highest))
      {
        nextHighest = windows.at(intersection.first);
        break;
      }
    }
  }

  // highest window
  //std::cout << "nextHighest: " << nextHighest.first << " -> " << nextHighest.second << std::endl;

  return std::make_pair(highest, nextHighest);
}


// find the total change from a swap without actually performing the swap
int testSwap(const std::vector<Room>& brigade, int ind1, int ind2, const std::map<int,std::pair<int,int>>& windows)
{
  int company1 = brigade[ind1].company;
  int company2 = brigade[ind2].company;
  // ind1 should always be less than ind2
  // calc change in window1
  int change1 = 0;
  // add up the negative change
  for(int i = ind1 + 1; i < brigade.size(); i++)
  {
    change1--;
    if(brigade[i].company == company1) break;
  }
  // add up the positive change
  int window1RightBound = windows.at(company1).second;
  if(window1RightBound < ind2) change1 += ind2 - window1RightBound;

  // repeat for window2 going in the opposite direction
  // calc change in window2
  int change2 = 0;
  // add up the negative change
  for(int i = ind2 - 1; i >= 0; i--)
  {
    change2--;
    if(brigade[i].company == company2) break;
  }
  // add up the positive change
  int window2LeftBound = windows.at(company2).first;
  if(window2LeftBound > ind1) change2 += window2LeftBound - ind1;
  //std::cout << ind1 << " & " << ind2 << " = " << change1 << " + " << change2 << " = " << change1 + change2 << std::endl;
  return change1 + change2;
}

// use my sort to order the brigade
// came up with a greedy algorithm 
void sort(std::vector<Room>& brigade)
{
  std::map<int,int> counts = countOccurrences(brigade);
  //std::cout << "counts initialized" << std::endl;
  std::map<int,std::pair<int,int>> windows = calcWindows(brigade, counts);
  //std::cout << "windows initialized" << std::endl;
  std::map<int,int> intersections = calcIntersections(windows);
  //std::cout << "intersections initialized" << std::endl;
  while(!done(intersections))
  {
    // get the windows that will reduce the most amount of intersections
    std::pair<std::pair<int,int>,std::pair<int,int>> abcd = getBestMoves(brigade, counts, windows, intersections);
    //std::cout << "abcd initialized" << std::endl;
    int a = abcd.first.first;
    int b = abcd.first.second;
    int c = abcd.second.first;
    int d = abcd.second.second;

    //std::cout << "a = " << a << "\nb = " << b << "\nc = " << c << "\nd = " << d << std::endl;
    
    //std::cout << "testing swap a + d: "; 
    int aANDd = testSwap(brigade, a, d, windows);
    //std::cout << aANDd << "\ntesting swap b + c: "; 
    //std::cout << "testing swap b + c: ";
    int bANDc = testSwap(brigade, c, b, windows);
    //std::cout << bANDc << std::endl;
    
    if (aANDd < bANDc)
    {
      //std::cout << "trying to swap a + d" << std::endl;
      //std::cout << "a = " << a << " d = " << d << std::endl;
      swap(brigade, a, d);
    }
    else if(bANDc < aANDd)
    {
      //std::cout << "trying to swap b + c" << std::endl;
      //std::cout << "b = " << b << " c = " << c << std::endl;
      swap(brigade, b, c);
    }
    else
    {
      // I messed up big time and I didn't think it could reach here
      //std::cerr << "[sort] All differences were positive..." << std::endl;
      //print_brigade(brigade);
      //exit(1);
      // try choosing the worse option?
      swap(brigade, (a + b) / 2, (c + d) / 2); // I don't know just do something
                           //swap(brigade, b, d);
      //if(aANDd >= bANDc) swap(brigade, a, d);
      //else swap(brigade, b, c);
    }
    windows = calcWindows(brigade, counts);
    intersections = calcIntersections(windows);
  }  
} 
