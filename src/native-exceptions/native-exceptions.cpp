//
// Created by Jerry Ding on 2023/3/11.
//

#include "native-exceptions.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;


// THROW EXCEPTION
void normal_throw() {
    throw std::logic_error("This is a normal exception");
}

//*EXC_BAD_ACCESS (SIGSEGV) | 0xc0000005 System.AccessViolationException
void null_pointer() {
    int *_p = NULL;
    *_p = 100;
}

// *EXC_CRASH (SIGABRT) pointer being freed was not allocated | 0xc0000374
void delete_pointer_twice() {
    int *p = new int;
    delete p;
    delete p;
}

//EXCEPTION_STACK_OVERFLOW
void infinite_recursion() {
    infinite_recursion();
}

/*
void heap_corruption() {
    char *cp = new char[10];
    (*(cp - 200))++;
    *cp = 'd';
}

//EXCEPTION_ACCESS_VIOLATION
void access_none_initialize() {
    int *p;
    std::cout << p[0];
}

void array_out_of_range() {
    int _nArr[10] = {0};
    std::cout << (_nArr[10]);
}

void pointer_out_of_range() {
    int _nArr[10] = {0};
    int *_p = _nArr;
    std::cout << (_p[10]);
}

void char_out_of_range() {
    char _szStr1[3] = {0};
    char _szStr2[3] = {'a', 'b', 'c'};
    strncpy(_szStr1, _szStr2, 10);
}

void iterator_out_of_range() {
    std::vector<int> m_nVer;
    m_nVer.push_back(1);
    m_nVer.push_back(2);
    std::vector<int>::iterator _iter;
    _iter = m_nVer.begin();
    int nNumber = *(_iter + 5);
    cout << nNumber;
}

void dangling_pointer() {
    int i, n;
    int *p;                 //define a pointer

    p = new int;   //allocate new block of memory that is of type integer and assign address to pointer
    *p = 10;                 //initialise the variable p points to, with the value 10
    cout << p << endl;
    cout << *p << endl;
    delete p;               //free the block of memory p points to.
    cout << p << endl;      // p stills holds the address of the block of memory it was previously pointing to
    cout << *p << endl;     // p now points to an undefined value. Thus p is a "dangling pointer"
}

void wild_pointer() {
    char *_p;
    *_p = 'a';
}
//EXCEPTION_STACK_OVERFLOW
//EXCEPTION_FLT_DIVIDE_BY_ZERO
//EXCEPTION_INT_DIVIDE_BY_ZERO
 */


void throw_exception(ErrorNumber number) {
    switch (number) {
        case ErrorNumber::NormalThrow:
            normal_throw();
        case ErrorNumber::NullPointer:
            null_pointer();
        case ErrorNumber::DeletePointerTwice:
            delete_pointer_twice();
        case ErrorNumber::InfiniteRecursion:
            infinite_recursion();
        default:
            throw std::out_of_range("Error number is not supported.");

    }
}
