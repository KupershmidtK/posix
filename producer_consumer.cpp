#include "producer_consumer.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>
#include <iostream>

static pthread_mutex_t counter_mutex;
static sem_t read_mutex;
static sem_t write_mutex;

static struct context context;
static struct app_data data;

int get_tid() {
    static int next_id = 0;
    static thread_local int tid = 0;

    if(tid == 0) {
      pthread_mutex_lock(&counter_mutex);
      tid = ++next_id;
      pthread_mutex_unlock(&counter_mutex);
    }

    return tid;
}


void* producer_routine(void* arg) {
  (void)arg;

  for (auto& value : data.values) {
    sem_wait(&write_mutex);

    data.value = value;

    sem_post(&read_mutex);
  }

  sem_wait(&write_mutex);
  context.stop_flag = true;
  sem_post(&read_mutex);

  return nullptr;
}

void print_result(int value) {
  char buffer[128];
  sprintf(buffer, "(%d, %d)\n", get_tid(), value);
  std::cout << buffer;
}

void* consumer_routine(void* arg) {
  (void)arg;

  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  int* result = new int;

  while (true) {
    sem_wait(&read_mutex);

    if (context.stop_flag) {
      sem_post(&read_mutex);
      break;
    }
    *result += data.value;

    if (context.debug_mode) {
      print_result(*result);
    }

    sem_post(&write_mutex);

    if (context.max_sleep_timeout != 0) {
      int sleep_timeout = std::rand() % context.max_sleep_timeout;
      usleep(sleep_timeout * 1000);
    }
  }

  return result;
}

void* consumer_interruptor_routine(void* arg) {
  (void)arg;


  std::srand(std::time(nullptr));

  while (!context.stop_flag) {
    int rand_customer = std::rand() % context.consumers_number;
    pthread_cancel(context.consumers_tid[rand_customer]);

    usleep(200 * 1000);
  }

  return nullptr;
}

int run_threads(unsigned long consumers_number, unsigned long max_sleep_timeout,
                bool debug_mode, std::vector<int> &values) {

  if (values.size() == 0) return 0;

  //------------------------------------------------
  context.stop_flag = false;
  context.consumers_number = consumers_number;
  context.debug_mode = debug_mode;
  context.max_sleep_timeout = max_sleep_timeout;
  context.consumers_tid = new pthread_t[context.consumers_number];

  //---------------------------------------------------
  data.value = 0;
  data.values = values;

  //----------------------------------------------------
  pthread_mutex_init(&counter_mutex, NULL);
  sem_init(&write_mutex, 0, 1);
  sem_init(&read_mutex, 0, 0);

  //----------------------------------------------------

  pthread_t producer_tid;
  pthread_create(&producer_tid, NULL, producer_routine, NULL);

  //---------------------------------------------------

  for (size_t i = 0; i < context.consumers_number; i++) {
    pthread_create(&context.consumers_tid[i], NULL, consumer_routine, NULL);
  }

  //-----------------------------------------------------
  pthread_t interruptor_tid;
  pthread_create(&interruptor_tid, NULL, consumer_interruptor_routine, NULL);

  //-----------------------------------------------------
  int** ret_value = new int*[context.consumers_number];

  for (size_t i = 0; i < context.consumers_number; i++) {
    pthread_join(context.consumers_tid[i], (void**)&ret_value[i]);
  }

  //===============================================================
  int result = 0;

  for (size_t i = 0; i < context.consumers_number; i++) {
    result += *ret_value[i];
    delete ret_value[i];
  }
  //================================================================

  delete[] context.consumers_tid;
  delete[] ret_value;

  pthread_mutex_destroy(&counter_mutex);
  sem_destroy(&read_mutex);
  sem_destroy(&write_mutex);

  return result;
}
