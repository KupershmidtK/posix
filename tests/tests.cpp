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
  const int SIZE = 1000;
  pthread_t th[SIZE];
  int* tid[SIZE];
  std::vector<int> results;

  for (size_t i = 0; i < SIZE; i++)
  {
    pthread_create(&th[i], NULL, get_tid_func, NULL);
  }

  for (size_t i = 0; i < SIZE; i++)
  {
      pthread_join(th[i], (void**)&tid[i]);
      results.push_back(*tid[i]);
      delete tid[i];
  }

  std::sort(results.begin(), results.end());
  auto duplicate = std::adjacent_find(results.begin(), results.end());

  CHECK( duplicate == results.end() ); // no duplicated tid's
  CHECK( results.back() == SIZE );
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
