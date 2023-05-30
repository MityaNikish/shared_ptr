#pragma once

#include <exception>

class Shared_ptr_exception : public std::exception {};

class Invalid_value_exception : public Shared_ptr_exception {
    const char* what() const noexcept override;
};