#pragma once
#include <cstdint>

#define MAKE_INT(id) using id = id##_t

MAKE_INT(uint8);
MAKE_INT(uint16);
MAKE_INT(uint32);
MAKE_INT(uint64);

MAKE_INT(int8);
MAKE_INT(int16);
MAKE_INT(int32);
MAKE_INT(int64);