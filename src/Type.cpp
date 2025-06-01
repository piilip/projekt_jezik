#include "Type.h"

const char *toString(Type type) {
  switch (type) {
  case kTypeUnknown:
    return "<unknown>";
  case kTypeBool:
    return "bool";
  case kTypeInt:
    return "int";
  case kTypeFloat:
    return "float";
  }
  assert(false && "Unhandled type");
}