#pragma once

#include <cassert>
#include <cstddef>
#include <utility>
#include <memory>

class Exp;

enum Type
{
    kTypeUnknown,
    kTypeBool,
    kTypeInt,
    kTypeFloat
};


const char* toString(Type type );


