#pragma once


#include "distanceEquation.hpp"


namespace btlf
{

class Derivative
{
public:
    Derivative() = delete;
    Derivative(const std::shared_ptr<DistanceEquation> quadratic_equation);
    ~Derivative() = default;


    Kernel::FT calculate_first_derivative_value(const Kernel::FT &value);
    Kernel::FT calculate_second_derivative_value(const Kernel::FT &value);


private:
    Kernel::FT m_a;
    Kernel::FT m_b;
    Kernel::FT m_c;
};








class TaylorPolynomGrade2
{
public:
    TaylorPolynomGrade2() = delete;
    TaylorPolynomGrade2(const std::shared_ptr<btlf::DistanceEquation> &quadratic_equation, const Kernel::FT x0);
    ~TaylorPolynomGrade2() = default;


    Kernel::FT get_m_quadratic_prefactor() const;
    Kernel::FT get_m_linear_prefactor() const;
    Kernel::FT get_m_constant_value() const;


    void inverse();

    std::string to_string() const;


private:
    Kernel::FT m_x0;

    Kernel::FT m_quadratic_prefactor;
    Kernel::FT m_linear_prefactor;
    Kernel::FT m_constant_value;

    
};















}