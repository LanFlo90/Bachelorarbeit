#pragma once

#include <cstdint>
#include <string_view>



using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;


/*
    This file contains all necessary constants for the calculation.
*/


namespace btlf
{

constexpr static std::uint8_t LONG_DOUBLE_DISPLAY_ACCURACY = 10U;

const static Kernel::FT INVALID_INTERSECTION_VALUE = -100.0;

const static Kernel::FT INVALID_ZERO_POINT = -200.0;

constexpr static std::uint8_t ARRAY_SIZE = 4U;

constexpr static std::uint8_t TUPLE_VALUE_ZERO = 0U;

constexpr static std::uint8_t TUPLE_VALUE_ONE = 1U;

constexpr static std::uint8_t TUPLE_VALUE_TWO = 2U;

constexpr static std::string_view MREAL_OBJECT = "OBJECT MReal\n\t() mreal";

constexpr static std::string_view DIRECTORY_OUTPUT = "output";

constexpr static std::string_view PATH_TO_INPUT_DIRECTORY = "./input/";

constexpr static std::uint8_t MPOINT_ARRAY_SIZE = 4U;

}