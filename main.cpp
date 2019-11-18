#define private public

#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <chrono>
#include <variant>
#include <cmath>
#include <boost/container/small_vector.hpp>
#include <memory>
#include <cassert>
#include "small_vector.h"


struct Foo {
    Foo() {
        std::cout << "default ctor: " << this << std::endl;
    }

    Foo(const std::string& s) : _s(s) {
        std::cout << "Creating from string: " << this << std::endl;
    }
    
    Foo(const char* c) : _s(c) {
        std::cout << "Creating from literal: " << this << std::endl;
    }

    Foo(const Foo& f)
        : _s(f._s) {
            std::cout << "copying from: " << &f << " into: " << this << " value:" << _s << std::endl;
    }

    Foo(Foo&& f)
        : _s(std::move(f._s)) {
            std::cout << "moving from: " << &f << " into: " << this << " value:" << _s << std::endl;
    }

    Foo& operator=(const Foo& f) {
        _s = f._s;
        std::cout << "copy asign from: " << &f << " into: " << this << " value:" << _s << std::endl;
        return *this;
    }

    Foo& operator=(Foo&& f) {
        _s = std::move(f._s);
        std::cout << "move asign from: " << &f << " into: " << this << " value:" << _s << std::endl;
        return *this;
    }

    ~Foo() {
        std::cout << "destructing: " << this << std::endl;
    }
    
    const std::string& get() const {
        return _s;
    }

    std::string _s;
};

/*
int main(int argc, const char * argv[]) {
    
    small_vector<const char*, 10> sv1 = {"abc", "def", "gh", "a", "b", "c"};
//    small_vector<const char*, 3> sv1(sv);
    sv1.push_back("bla");
    sv1.pop_back();
    sv1.push_back("bla");
    std::cout << sv1.front() << " " << sv1.back();
    std::cout << std::endl;

    small_vector<int, 5> sv_int = {2, 5, 4, 3, 1, 6};

    std::sort(sv_int.begin(), sv_int.end());
    for(auto &x : sv_int) {
        x *= 2;
        std::cout << x << " ";
    }
    
//    {
//        auto start = std::chrono::system_clock::now();
//        small_vector<Foo, 10> vec;
//        // boost::container::small_vector<Foo, 10> vec;
//    //    std::vector<Foo> vec;
//        for(int i = 0; i < 1000; ++i) {
//            vec.push_back("d");
//            vec.pop_back();
//        }
//        auto end = std::chrono::system_clock::now();
//        auto result = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//        std::cout << "Time: " << result.count();
//        std::cout << std::endl;
//    }
//    std::cout << "Size Diff :" << sizeof(boost::container::small_vector<int, 10>) - sizeof(small_vector<int, 10>);

//    sv_int.emplace_back(6);
//    sv_int.pop_back();
//    sv_int.push_back(6);
//    std::cout << sv_int.front() << "  " << sv_int.back() << "  " << sv_int.size();
//    small_vector<std::pair<int, std::string>> v;
//    v.emplace_back(2, "d");
//    std::cout << std::endl << v[0].first << "  " << v[0].second;
    return 0;
}*/

struct A {
    int x;
    std::string y;
    float c;
    ~A() {
        std::cout << "Dtor\n";
    }
};


//int main() {
//
//    auto start1 = std::chrono::system_clock::now();
//    for (int j = 0; j < 100; ++j) {
//    small_vector<std::string, 10> v = {"a", "b", "c", "d", "e", "f"};
//    for(int i = 0; i < 1000; ++i) {
//        v.push_back(std::to_string(i));
//    }
//    }
//    auto end1 = std::chrono::system_clock::now();
//    auto result_our = static_cast<std::chrono::duration<double>>(end1 - start1);
//
//    auto start2 = std::chrono::system_clock::now();
//    for (int j = 0; j < 100; ++j) {
//    boost::container::small_vector<std::string, 10> bv = {"a", "b", "c", "d", "e", "f"};
//    for(int i = 0; i < 1000; ++i) {
//        bv.push_back(std::to_string(i));
//    }
//    }
//    auto end2 = std::chrono::system_clock::now();
//    auto result_boost = static_cast<std::chrono::duration<double>>(end2 - start2);
//    std::cout <<"Our result: " << result_our.count() << "sec" << '\n';
//    std::cout << "Boosts result: " <<result_boost.count() << "sec" << '\n';
//    std::cout << "Portion :" << result_boost / result_our;
//    return 0;
//}


int main() {
    small_vector<std::string, 10> v = {"a", "b", "c", "d", "e"};
    small_vector<std::string, 10> q;
    
    assert(!v.empty());
    assert(q.empty());
    
    assert(v != q);
    q = v;
    assert(v == q);
    assert(v.size() == q.size());
    
    q.push_back("a");
    q.push_back("b");
    q.push_back("c");
    q.push_back("d");
    q.push_back("e");
    
    assert(v.size() == q.size() - 5);
    q.pop_back();
    q.pop_back();
    q.pop_back();
    q.pop_back();
    q.pop_back();

    assert(v == q);
    
    v.resize(3);
    q.resize(3);
    
    assert(v.size() == q.size());
    assert(v == q);
    
    return 0;
}
