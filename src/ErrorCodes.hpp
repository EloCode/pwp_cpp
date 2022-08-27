#pragma once

#include <cstdlib>

constexpr int SUCCESS = EXIT_SUCCESS;
constexpr int ERROR = EXIT_FAILURE;
constexpr int ERROR_INVALID_ARGUMENTS = EXIT_FAILURE + 1;
constexpr int ERROR_PWP_TESTS_FAILED = EXIT_FAILURE + 2;
constexpr int ERROR_PWP_FAILED = EXIT_FAILURE + 3;