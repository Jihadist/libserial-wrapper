#pragma once
#include "libserialport.h"

#define __PRETTY_FUNCTION__ __FUNCSIG__
class helper {
public:
    helper() = delete;
    /* Helper function for error handling. */
    static int check(sp_return result);
};
