#include <iostream>
#include <string>
#include <sstream>
#include "producer_consumer.h"

bool check_atribute(int argc) {
  if (argc < 3) {
    std::cout << "Usage:" << std::endl
              << "./posix <Number of consumer> <Max sleep timeout> [-debug]"
              << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (!check_atribute(argc)) {
    return -1;
  }

  unsigned long t_cnt = 0;
  unsigned long timeout = 0;
  try {
    t_cnt = std::stoul(argv[1]);
    if (t_cnt < 1) {
      throw std::invalid_argument("Invalid parameters");
    }

    timeout = std::stoul(argv[2]);
  } catch (...) {
    std::cout << "Invalid parameters" << std::endl;
    return -1;
  }

  bool debug_mode = false;
  std::string debug_str("-debug");
  if(argc >= 4)
      debug_mode = debug_str.compare(argv[3]) == 0;

//----------------------------------
  std::vector<int> values; 
  std::string data_string;
  std::getline(std::cin, data_string);
  std::stringstream iss(data_string);

  int number;
  while (iss >> number) {
    values.push_back(number);
  }
//----------------------------------
  
  std::cout << run_threads(t_cnt, timeout, debug_mode, values) << std::endl;
  return 0;
}
