#pragma once
#include "libserialport.h"
class helper {
public:
    helper() = delete;
    /* Helper function for error handling. */
    static int check(sp_return result);
};
