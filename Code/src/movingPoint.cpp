#include <iostream>
#include <sstream>
#include <exception>
#include "movingPoint.hpp"
#include <iomanip>




btlf::LinearInterpolation::LinearInterpolation(const Kernel::FT &start_value, const Kernel::FT &end_value) :
     m_linear_prefactor(end_value - start_value), 
     m_constant_value(start_value)
{

}



btlf::LinearInterpolation::LinearInterpolation(const LinearInterpolation &other):
m_linear_prefactor(other.get_m_linear_prefactor()),
m_constant_value(other.get_m_constant_value())
{

}



std::string btlf::LinearInterpolation::to_string() const
{   

    std::ostringstream point;
    std::string sign = m_constant_value > 0 ? "+ " : "";

    if( (m_constant_value == 0.0) && (m_linear_prefactor == 0.0) )
    {
        return " 0 ";
    }
    else if ( (m_linear_prefactor != 0.0) && (m_constant_value == 0.0) )
    {   
        point
            << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY) 
            << m_linear_prefactor 
            << "t ";

        return  point.str();
    }  
    else
    {   
        point   
                << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY)
                << m_linear_prefactor
                << "t " 
                << sign 
                << m_constant_value
                << " ";

        return point.str();
    }
        
}



Kernel::FT btlf::LinearInterpolation::calucate_value(const Kernel::FT &value) const
{   
    // std::cout << "\n\n\n Value: " << value << "\n\n\n";

    if( (value >= 0.0) && (value <= 1.0) )
    {
        return m_linear_prefactor * value + m_constant_value;
    }
    else
    {   
        throw std::invalid_argument("the value must be between 0 and 1. (btlf::LinearInterpolation::calculate_value)");
    }
}



const Kernel::FT &btlf::LinearInterpolation::get_m_linear_prefactor() const
{
    return m_linear_prefactor;
}



const Kernel::FT &btlf::LinearInterpolation::get_m_constant_value() const
{
    return m_constant_value;
}



void btlf::LinearInterpolation::set_m_linear_prefactor(const Kernel::FT &value)
{
    m_linear_prefactor = value;
}



void btlf::LinearInterpolation::set_m_constant_value(const Kernel::FT &value)
{
    m_constant_value = value;
}



/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::MovingPoint::MovingPoint(const Kernel::FT &start_x, const Kernel::FT &start_y, const Kernel::FT &end_x, const Kernel::FT &end_y, std::string name) :
    m_x_component(start_x, end_x),
    m_y_component(start_y, end_y),
    m_name(name)
{

}



std::string btlf::MovingPoint::value_of_t(const Kernel::FT t) const
{
    std::ostringstream point;

    point   
            << m_name 
            << "(" 
            << t  
            << ") = ( " 
            << m_x_component.calucate_value(t) 
            << ", " 
            << m_y_component.calucate_value(t) 
            << ")";

    return point.str();
}



std::string btlf::MovingPoint::to_string() const
{
    std::ostringstream stream;

    stream 
            << m_name 
            << "(t) = ( " 
            << m_x_component.to_string() 
            << ", " 
            << m_y_component.to_string() 
            << ")";
    
    return stream.str();
}



const btlf::LinearInterpolation &btlf::MovingPoint::get_m_x_component() const
{
    return m_x_component;
}



const btlf::LinearInterpolation &btlf::MovingPoint::get_m_y_component() const
{
    return m_y_component;
}


const std::string &btlf::MovingPoint::get_m_name() const
{
    return m_name;
}



std::pair<const Kernel::FT, const Kernel::FT> btlf::MovingPoint::pair_value_of_t(const Kernel::FT &t)
{
    std::pair<Kernel::FT,Kernel::FT> point_corrdinates;

    const auto x_corrdinate = this->m_x_component.calucate_value(t);
    const auto y_corrdinate = this->m_y_component.calucate_value(t);

    point_corrdinates.first = x_corrdinate;
    point_corrdinates.second = y_corrdinate;

    return point_corrdinates;
}

/*
    Diese Methode ordnet die Movingpoints anhand der größe der X-Komponente.
    first = kleineres x
    second = größeres x
*/
std::pair<btlf::MovingPoint, btlf::MovingPoint> btlf::MovingPoint::order_size(const MovingPoint &other, bool x_coordinate)
{
    std::pair<MovingPoint, MovingPoint> right_order_from_small_to_big;

    auto mp1_value = Kernel::FT{0.0};
    auto mp2_value = Kernel::FT{0.0};

    if(x_coordinate)
    {
        mp1_value = m_x_component.calucate_value(0.5);
        mp2_value = other.m_x_component.calucate_value(0.5);      
    }

    else
    {
        mp1_value = m_y_component.calucate_value(0.5);
        mp2_value = other.m_y_component.calucate_value(0.5);  
    }


    if(mp1_value < mp2_value)
    {
        right_order_from_small_to_big.first = *this;
        right_order_from_small_to_big.second = other;
    }
    else
    {
        right_order_from_small_to_big.first = other;
        right_order_from_small_to_big.second = *this;  
    }

    return right_order_from_small_to_big;
}



bool btlf::MovingPoint::has_equal_x_component(const MovingPoint &other) const
{
    auto t_prefactor_cmp = (m_x_component.get_m_linear_prefactor() == other.get_m_x_component().get_m_linear_prefactor());
    auto constant_cmp = (m_x_component.get_m_constant_value() == other.get_m_x_component().get_m_constant_value());

    return (t_prefactor_cmp && constant_cmp);
}