#include <chrono>
#include <set>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"
#include "lib/splay_tree.h"

void BasicOperations(int maxNumber) {
    std::vector<std::pair<int, int>> operations;
    for (int i = 0; i < 100'000; ++i) {
        operations.emplace_back(rand() % 2, rand() % maxNumber);
    }


    SplayTree tree = {NULL};
    std::set<int> set;

    for (auto i: operations) {
        if (i.first == 0) {
            set.emplace(i.second);
            Insert(&tree, i.second);
        } else if (i.first == 1) {
            set.erase(i.second);
            Erase(&tree, i.second);
        }

        // check equality of containers
        for (int j = 0; j < maxNumber; ++j) {
            ASSERT_EQ(set.contains(j), Contains(&tree, j));
        }
    }
}

TEST(FunctionalTest, BasicOperationsTest) {
    for (int maxNumber = 5; maxNumber < 200; maxNumber += 10) {
        BasicOperations(maxNumber);
    }
}

TEST(PerformaceTest, ManyInsertions) {
    const int maxNumber = 100'000;
    std::vector<int> operations;
    for (int i = 0; i < 1'000'000; ++i) {
        operations.emplace_back(rand() % maxNumber);
    }

    SplayTree tree = {NULL};

    auto splayTreeBegin = std::chrono::steady_clock::now();
    for (auto i: operations) {
        Insert(&tree, i);
    }
    auto splayTreeEnd = std::chrono::steady_clock::now();

    std::set<int> set;
    auto setBegin = std::chrono::steady_clock::now();
    for (auto i: operations) {
        set.emplace(i);
    }
    auto setEnd = std::chrono::steady_clock::now();

    for (size_t i = 0; i < maxNumber; ++i) {
        ASSERT_EQ(set.contains(i), Contains(&tree, i));
    }

    size_t splay_time = std::chrono::duration_cast<std::chrono::nanoseconds>(splayTreeEnd - splayTreeBegin).count();
    size_t set_time = std::chrono::duration_cast<std::chrono::nanoseconds>(setEnd - setBegin).count();
    ASSERT_LT(splay_time / 2, set_time);
}


TEST(PerformaceTest, ManyInsertionsAndEraseOnFullTree) {
    const int maxNumber = INT_MAX;
    std::vector<int> operations;
    for (int i = 0; i < 1'000'000; ++i) {
        operations.emplace_back(rand() % maxNumber);
    }

    SplayTree tree = {NULL};

    auto splayTreeBegin = std::chrono::steady_clock::now();
    for (auto i: operations) {
        Insert(&tree, i);
    }
    for (auto i: operations) {
        Erase(&tree, i);
    }
    auto splayTreeEnd = std::chrono::steady_clock::now();

    std::set<int> set;
    auto setBegin = std::chrono::steady_clock::now();
    for (auto i: operations) {
        set.emplace(i);
    }
    for (auto i: operations) {
        set.erase(i);
    }
    auto setEnd = std::chrono::steady_clock::now();


    size_t splay_time = std::chrono::duration_cast<std::chrono::nanoseconds>(splayTreeEnd - splayTreeBegin).count();
    size_t set_time = std::chrono::duration_cast<std::chrono::nanoseconds>(setEnd - setBegin).count();
    ASSERT_LT(splay_time / 2, set_time);
}
