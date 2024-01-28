#pragma once

#include <cassert>
#include <cstddef>

enum Type
{
    kTypeUnknown,
    kTypeBool,
    kTypeInt,
    kTypeFloat
};


struct VarType{
    Type type;
    bool isArray;
    int arraySize;

    explicit VarType(Type type) : type(type), isArray(false), arraySize(0) {}
    explicit VarType(Type type, int size) : type(type), isArray(true), arraySize(size) {}

};


const char* toString(Type type );


