#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <producer_consumer.h>

TEST_CASE("check get_tid()") {

    pthread_key_t tid_number_key;
    int tid = 100;
    pthread_key_create(&tid_number_key, NULL);
    pthread_setspecific(tid_number_key, (void*)&tid);
    
    CHECK( get_tid() == tid );
}

TEST_CASE("check run_threads()") {
    std::vector<int> test_vector {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    CHECK( run_threads(5, 1000, false, test_vector) == 55 );
}

TEST_CASE("check run_threads() 2") {
    std::vector<int> test_vector {};

    CHECK( run_threads(5, 1000, false, test_vector) == 0 );
}

TEST_CASE("check run_threads() 3") {
    std::vector<int> test_vector {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    CHECK( run_threads(5, 0, false, test_vector) == 55 );
}

