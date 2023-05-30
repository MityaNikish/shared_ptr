#include "pch.h"
#include "Exception.hpp"

const char* Invalid_value_exception::what() const noexcept {
    return "Cannot return value of invalid object";
}