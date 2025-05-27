#include <cassert>
#include <iostream>
#include "SkipList.h"

void TestConstructorAndContains() {
    int data[] = {1, 2, 3};
    SkipList<int> sl(data, 3);
    assert(sl.Size() == 3);
    assert(!sl.Empty());
    assert(sl.Contains(1));
    assert(sl.Contains(2));
    assert(sl.Contains(3));
    assert(!sl.Contains(4));
    assert(sl.ToString() == "1 2 3");
    std::cout << "ConstructorAndContains - DONE\n";
}

void TestInsert() {
    int data[] = {5};
    SkipList<int> sl(data, 1);
    sl.Insert(3);
    assert(sl.Size() == 2);
    assert(sl.Contains(3));
    assert(sl.ToString() == "3 5");
    sl.Insert(7);
    assert(sl.Size() == 3);
    assert(sl.Contains(7));
    assert(sl.ToString() == "3 5 7");
    std::cout << "Insert - DONE\n";
}

void TestErase() {
    int data[] = {10, 20, 30};
    SkipList<int> sl(data, 3);
    bool erased = sl.Erase(20);
    assert(erased);
    assert(!sl.Contains(20));
    assert(sl.Size() == 2);
    assert(!sl.Erase(40));
    assert(sl.Size() == 2);
    std::cout << "Erase - DONE\n";
}

void TestSearchAndClear() {
    int data[] = {100, 200, 300};
    SkipList<int> sl(data, 3);
    auto node = sl.Search(200);
    assert(node && node->get_value() == 200);
    sl.Clear();
    assert(sl.Empty());
    assert(sl.Size() == 0);
    assert(sl.ToString().empty());
    std::cout << "SearchAndClear - DONE\n";
}

int main() {
    TestConstructorAndContains();
    TestInsert();
    TestErase();
    TestSearchAndClear();
    std::cout << "All tests passed\n";
    return 0;
}
