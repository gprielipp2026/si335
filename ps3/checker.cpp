/* SI 335 Room Swap Project
 *
 * This program checks whether a set of swaps is correct
 * for a given input file, and if so, how many swaps were used.
 *
 * usage: ./checker INPUT_FILE [SWAPS_FILE]
 *
 * If SWAPS_FILE is not given, read from standard in.
 *
 * To compile this file, run
 *   g++ -std=c++14 -Ofast checker.cpp -o checker
 */

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

void usage(const char* arg0) {
  std::cerr << "usage: " << arg0 << " INPUT_FILE [SWAPS_FILE]" << std::endl;
  exit(1);
}

template <typename SwapStream>
bool run(std::ifstream& data, SwapStream& swaps, bool show=false) {
  std::unordered_map<std::string, std::size_t> locations;
  std::unordered_map<std::string, short> companies;

  // read in all Mids and record original index from file location
  std::size_t size = 0;
  {
    std::string name;
    short company;
    for (; data >> company >> name; ++size) {
      locations.emplace(std::make_pair(name, size));
      companies.emplace(std::make_pair(name, company));
    }
  }

  // read in swaps and update locations as we go
  std::size_t swap_count = 0;
  for (std::string name1, name2; swaps >> name1 >> name2; ++swap_count) {
    auto found1 = locations.find(name1);
    auto found2 = locations.find(name2);
    if (found1 == locations.end()) {
      std::cout << "Unrecognized name '" << name1 << "' in swap file" << std::endl;
      return false;
    }
    if (found2 == locations.end()) {
      std::cout << "Unrecognized name '" << name2 << "' in swap file" << std::endl;
      return false;
    }
    std::swap(found1->second, found2->second);
  }

  // move Mids into vector based on locations
  std::vector<short> sorted(size, -1);
  for (auto mid_loc : locations) {
    sorted.at(mid_loc.second) = companies.at(mid_loc.first);
  }

  if (show) { // secret option for those who look at the source code
    std::vector<std::string> snames(size, "");
    for (auto mid_loc : locations) {
      snames.at(mid_loc.second) = mid_loc.first;
    }
    for (std::size_t i = 0; i < snames.size(); ++i) {
      std::cout << sorted.at(i) << "\t" << snames.at(i) << std::endl;
    }
  }

  std::unordered_set<short> company_done;
  {
    auto iter = sorted.cbegin();
    auto prev = *iter;
    company_done.emplace(prev);
    for (; iter != sorted.cend(); ++iter) {
      if (*iter != prev) {
        prev = *iter;
        auto placed = company_done.emplace(prev);
        if (!placed.second) {
          std::cout << "Company " << prev << " is not together" << std::endl;
          return false;
        }
      }
    }
  }

  // all good if we get here!
  std::cout << "Placed " << size << " Mids into "
    << company_done.size() << " companies using "
    << swap_count << " swaps" << std::endl;
  return true;
}

int main(int argc, const char*const* argv) {
  if (argc < 2 || argc > 3)
    usage(argv[0]);

  std::ifstream infile(argv[1]);
  if (!infile) {
    std::cerr << "ERROR: could not open input file '" << argv[1] << "'" << std::endl;
    exit(1);
  }

  bool result;
  if (argc < 3)
    result = run(infile, std::cin);
  else {
    std::ifstream swapfile(argv[2]);
    if (!swapfile) {
      std::cerr << "ERROR: could not open swaps file '" << argv[2] << "'" << std::endl;
      exit(1);
    }
    result = run(infile, swapfile);
  }

  return result ? 0 : 2;
}
