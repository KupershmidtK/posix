#ifndef _P_C_H_
#define _P_C_H_

#include <pthread.h>
#include <vector>

struct context {
  bool stop_flag;

  pthread_t* consumers_tid;
  size_t consumers_number;

  bool debug_mode;
  unsigned long max_sleep_timeout;
};

struct app_data {
  int value;
  std::vector<int> values;
};

int get_tid();

// the declaration of run threads can be changed as you like
int run_threads(unsigned long consumers_number, unsigned long max_sleep_timeout,
                bool debug_mode, std::vector<int>& values);

#endif