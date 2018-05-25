#ifndef STRINGFORMAT_H
#define STRINGFORMAT_H
#include <memory>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

template<typename ... Args>

string string_format(const string & format, Args ... args)
{
    size_t size = snprintf(nullptr,0,format.c_str(),args ...) + 1;
    unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(),size,format.c_str(),args ...);
    return string(buf.get(),buf.get()+size-1);
}

#endif // STRINGFORMAT_H
