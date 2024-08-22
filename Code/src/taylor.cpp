#include "taylor.hpp"




btlf::Derivative::Derivative(const std::shared_ptr<DistanceEquation> quadratic_equation) :
    m_a(quadratic_equation->get_m_quadratic_prefactor()),
    m_b(quadratic_equation->get_m_linear_prefactor()),
    m_c(quadratic_equation->get_m_constant_value())
{

}



Kernel::FT btlf::Derivative::calculate_first_derivative_value(const Kernel::FT &t_value)
{
    auto result = Kernel::FT{
        (m_a * t_value + (m_b/2.0)) / sqrt(m_a * t_value * t_value + m_b * t_value + m_c)
    };

    return result;
}



Kernel::FT btlf::Derivative::calculate_second_derivative_value(const Kernel::FT &t_value)
{
    auto result = Kernel::FT{
        (m_a / sqrt(m_a * t_value * t_value + m_b * t_value + m_c))
        -
        (((m_a * t_value + (m_b/2.0)) * (m_a * t_value + (m_b/2.0)))
        /
        sqrt((m_a * t_value * t_value + m_b * t_value + m_c)*(m_a * t_value * t_value + m_b * t_value + m_c)*(m_a * t_value * t_value + m_b * t_value + m_c)))
    };

    return result;
}




btlf::TaylorPolynomGrade2::TaylorPolynomGrade2(const std::shared_ptr<btlf::DistanceEquation> &quadratic_equation, const Kernel::FT x0)
{

    auto derivative = btlf::Derivative{quadratic_equation};

    auto derivative1 = Kernel::FT{derivative.calculate_first_derivative_value(x0)};
    auto derivative2 = Kernel::FT{derivative.calculate_second_derivative_value(x0)};
    auto eqaution_x0_value = Kernel::FT{quadratic_equation->calculate_value(x0)};


    m_quadratic_prefactor = 0.5 * derivative2;
    m_linear_prefactor = derivative1 - derivative2 * x0;
    m_constant_value = eqaution_x0_value - derivative1 * x0 + 0.5 * derivative2 * x0 * x0; 

}


Kernel::FT btlf::TaylorPolynomGrade2::get_m_quadratic_prefactor() const
{
    return m_quadratic_prefactor;
}


Kernel::FT btlf::TaylorPolynomGrade2::get_m_linear_prefactor() const
{
    return m_linear_prefactor;
}


Kernel::FT btlf::TaylorPolynomGrade2::get_m_constant_value() const
{
    return m_constant_value;
}



std::string btlf::TaylorPolynomGrade2::to_string() const
{
    std::ostringstream taylor;

    taylor
            << "T_2(t)= "
            << m_quadratic_prefactor.doubleValue()
            << "t^2 "
            << "+ "
            << m_linear_prefactor.doubleValue()
            << "t "
            << "+ "
            << m_constant_value.doubleValue()
            << "\n";    


    return taylor.str();
}



void btlf::TaylorPolynomGrade2::inverse()
{
    m_quadratic_prefactor = -1.0 * m_quadratic_prefactor;
    m_linear_prefactor = -1.0 * m_linear_prefactor;
    m_constant_value = -1.0 * m_constant_value;

}