#pragma once

#include <utility>
#include <string>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include "constants.hpp"


using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;


/*
    btlf : Bachelor thesis Langer Florian
*/
namespace btlf
{


/*
   The class LinearInterpolation represent the linear interpolation between 0 and 1.
   The shape of the linear interpolation is f(t) = m_t_value * t + m_constant_value.
   Here t is the variable to determine the value of the linear interpolation.
   The member m_t_value is the result of end_value - start_value.
   The member m_constant_value is the start_value.

   For instance: start_value = 100.0L, end_value = 140.0L
   => f(t) = (140.0L - 100.0L) * t + 100.0L
   => f(t) = 40.0L * t + 100.0L

*/
class LinearInterpolation
{
public:
    LinearInterpolation() = default;

    LinearInterpolation(const Kernel::FT &start_value, const Kernel::FT &end_value);

    LinearInterpolation(const LinearInterpolation &other);

    ~LinearInterpolation() = default;


    std::string to_string() const;

    /*  
        Method for determining a concrete value of linear interpolation.
        Valid values are in the interval 0 and 1.
    */
    Kernel::FT calucate_value(const Kernel::FT &value) const;

    /*
        Getter for the member variables.
    */
    const Kernel::FT &get_m_linear_prefactor() const;
    const Kernel::FT &get_m_constant_value() const;

    /*
        Setter for the member variables.
    */
    void set_m_linear_prefactor(const Kernel::FT &value);
    void set_m_constant_value(const Kernel::FT &value);


    LinearInterpolation &operator=(const LinearInterpolation &other) = default;



private:
    Kernel::FT m_linear_prefactor;

    Kernel::FT m_constant_value;
};





/*
    A MovingPoint object consists of two LinearInterpolation objects.
*/
class MovingPoint
{
public:
    MovingPoint() = default;

    MovingPoint(const Kernel::FT &start_x, const Kernel::FT &start_y,
                const Kernel::FT &end_x, const Kernel::FT &end_y,
                std::string name = "f");

    ~MovingPoint() = default;

    /*
        With a t in the interval between 0 and 1, the corresponding MovingPoint
        is returned as a string.
    */
    std::string value_of_t(const Kernel::FT t) const;

    std::string to_string() const;

    std::pair<const Kernel::FT, const Kernel::FT> pair_value_of_t(const Kernel::FT &t);

    /*
        Getter for member variables.
    */
    const LinearInterpolation &get_m_x_component() const;
    const LinearInterpolation &get_m_y_component() const;
    const std::string &get_m_name() const;


    std::pair<MovingPoint, MovingPoint> order_size(const MovingPoint &other, bool x_coordinate);


    MovingPoint &operator=(const MovingPoint &other) = default;


    bool has_equal_x_component(const MovingPoint &other) const;


private:

    /*
        m_x_component is the x coordinate of the MovingPoint object.
    */
    LinearInterpolation m_x_component;

    /*
        m_y_component is the y coordinate of the MovingPoint object.
    */
    LinearInterpolation m_y_component;

    std::string m_name;
};



}


