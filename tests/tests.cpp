#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <producer_consumer.h>

void* get_tid_func(void* arg) {
  (void)arg;
  int* tid = new int;
  *tid = get_tid();
  return (void*)tid;
}

TEST_CASE("check get_tid()") {
  CHECK(get_tid() == 1);
  CHECK(get_tid() == 1);

  pthread_t th1, th2;
  int* tid;

  pthread_create(&th1, NULL, get_tid_func, NULL);
  pthread_join(th1, (void**)&tid);
  CHECK(*tid == 2);
  delete tid;

  pthread_create(&th2, NULL, get_tid_func, NULL);
  pthread_join(th2, (void**)&tid);
  CHECK(*tid == 3);
  delete tid;
}

TEST_CASE("check run_threads()") {
  std::vector<int> test_vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  CHECK(run_threads(5, 1000, false, test_vector) == 55);
}

TEST_CASE("check run_threads() 2") {
  std::vector<int> test_vector{};

  CHECK(run_threads(5, 1000, false, test_vector) == 0);
}

TEST_CASE("check run_threads() 3") {
  std::vector<int> test_vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  CHECK(run_threads(5, 0, false, test_vector) == 55);
}
