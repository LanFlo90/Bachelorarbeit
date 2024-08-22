#include "segmentEquation.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <memory>
#include <thread>



btlf::Interval::Interval(const Kernel::FT &start_interval, const Kernel::FT &end_interval, bool start, bool end):
    m_start_interval(start_interval),
    m_end_interval(end_interval),
    m_start_interval_including(start),
    m_end_interval_including(end)
{

}



btlf::Interval::Interval():
    m_start_interval(0.0),
    m_end_interval(1.0),
    m_start_interval_including(true),
    m_end_interval_including(true)
{

}



btlf::Interval::Interval(const Kernel::FT &start_interval, const Kernel::FT &end_interval, SegmentType segment_type)
{   
  
    /*
        Case 1: unvalid values ( Default ist ]0, 0[ )
    */
    if( (start_interval > 1.0) ||  (end_interval < 0.0) || (start_interval > end_interval) )
    {
        set_members_interval(0.0, 0.0, false, false);
    }
    /*
        Case 2: start_interval and end_interval are between ] 0, 1 [
    */
    else if ( (start_interval >= 0.0 && start_interval < 1.0) && (end_interval > 0.0 && end_interval < 1.0) )
    {
        set_members_interval(start_interval, end_interval, true, false);
    }

    // New
    else if ( (start_interval > 0.0 && start_interval < 1.0) && (end_interval == 1.0) )
    {
        set_members_interval(start_interval, end_interval, false, true);
    }
    // New ende

    /*
        Case 3: start_interval is less than 0 and end_interval is between 0 and 1
    */
    else if( (start_interval < 0.0) && (end_interval > 0.0 && end_interval <= 1.0) )
    {
        set_members_interval(0.0, end_interval, true, true);
    }
    /*
        Case 4: start_interval is between 0 and 1 and end_interval is greater than 1.
    */
    else if ( (start_interval > 0.0 && start_interval < 1.0) && (end_interval > 1.0) )
    {   
        set_members_interval(start_interval, 1.0, false, true);
    }
    /*
        Case 5: start_interval is less than 0 and end_interval is greater than 1.
    */
    else if ( (start_interval < 0.0) && (end_interval > 1.0) )
    {
        set_members_interval(0.0, 1.0, true, true);
    }
    else
    {   
        std::cout << "\n\nERROR!!!!!\n\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }

    /*
        Segment types checks:
 
        In both cases, there is no straight line because both points for setting up the segment_equation are identical.
    */
    if ( (segment_type == SegmentType::SEGMENT_TO_POINT) && (m_end_interval == 1) )
    {
        m_end_interval_including = false;
    }
    // else if ( (segment_type == SegmentType::POINT_TO_SEGMENT) && (m_end_interval == 1) )
    // {
    //     m_end_interval_including = true;
    // }

}



std::string btlf::Interval::to_string() const
{
    std::ostringstream interval;

    std::string start_sign = m_start_interval_including ? "\t[ " : "\t] ";
    std::string end_sign = m_end_interval_including ? " ] " : " [ ";

    interval
            << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY)
            << "\twith t"
            << start_sign
            << m_start_interval
            << ", "
            << m_end_interval
            << end_sign;

    return interval.str();

}



const Kernel::FT &btlf::Interval::get_m_start_interval() const
{
    return m_start_interval;
}



const Kernel::FT &btlf::Interval::get_m_end_interval() const
{
    return m_end_interval;
}



bool btlf::Interval::get_m_start_including() const
{
    return m_start_interval_including;
}



bool btlf::Interval::get_m_end_including() const
{
    return m_end_interval_including;
}



void btlf::Interval::set_members_interval(const Kernel::FT &start_interval, const Kernel::FT &end_interval, bool start_interval_including, bool end_interval_including)
{
    m_start_interval = start_interval;
    m_end_interval = end_interval;
    m_start_interval_including = start_interval_including;
    m_end_interval_including = end_interval_including;
}



/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::LinearEquation::LinearEquation(const MovingPoint &point_1, const MovingPoint &point_2, std::string name) :
    m_gradient(calculate_m_gradient(point_1, point_2)),
    m_y_axis(calculate_m_y_axis(point_1, m_gradient)),
    m_name(name)
{

}



btlf::LinearEquation::LinearEquation(const LinearEquation &segement_equation, const MovingPoint &perpendicular_point, const std::string name) :
    m_gradient(calculate_m_gradient(segement_equation)),
    m_y_axis(calculate_m_y_axis(perpendicular_point, m_gradient)),
    m_name(name)
{

}



std::string btlf::LinearEquation::to_string() const
{
    std::ostringstream equation;

    std::string sign_t_value = m_y_axis.get_m_linear_prefactor() >= 0 ? " + " : " ";
    std::string sign_t_constant = m_y_axis.get_m_constant_value() >= 0 ? " + " : " ";

    if(m_gradient != 0.0 && m_y_axis.get_m_linear_prefactor() != 0.0 && m_y_axis.get_m_constant_value() != 0.0)
    {
        equation
                << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY)
                << m_name 
                << ",t(x) = ( " 
                << m_gradient 
                << "x" 
                << sign_t_value
                <<  m_y_axis.get_m_linear_prefactor() 
                << "t"
                << sign_t_constant
                << m_y_axis.get_m_constant_value() 
                << " )";
    }
    else if(m_gradient != 0.0 && m_y_axis.get_m_linear_prefactor() != 0.0 && m_y_axis.get_m_constant_value().isZero())
    {
        equation
                << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY) 
                << m_name 
                << ",t(x) = ( " 
                << m_gradient 
                << "x" 
                << sign_t_value 
                <<  m_y_axis.get_m_linear_prefactor() 
                << "t )";

    }
    else if(m_gradient.isZero() && m_y_axis.get_m_linear_prefactor() != 0.0 && m_y_axis.get_m_constant_value().isZero())
    {
        equation 
                // << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY)
                << m_name 
                << ",t(x) = ( " 
                << sign_t_value 
                <<  m_y_axis.get_m_linear_prefactor() 
                << "t )";  
    }
  
    return equation.str();
}



const Kernel::FT &btlf::LinearEquation::get_m_gradient() const
{

    return m_gradient;
}



const btlf::LinearInterpolation &btlf::LinearEquation::get_m_y_axis() const
{
    return m_y_axis;
}



const std::string &btlf::LinearEquation::get_m_name() const
{
    return m_name;
}



Kernel::FT btlf::LinearEquation::calculate_m_gradient(const MovingPoint &point_1, const MovingPoint &point_2) const
{
    LinearInterpolation numerator;
    LinearInterpolation denominator;

    Kernel::FT gradient = 0.0;

    numerator.set_m_linear_prefactor(point_2.get_m_y_component().get_m_linear_prefactor() - point_1.get_m_y_component().get_m_linear_prefactor());
    numerator.set_m_constant_value(point_2.get_m_y_component().get_m_constant_value() - point_1.get_m_y_component().get_m_constant_value());

    denominator.set_m_linear_prefactor(point_2.get_m_x_component().get_m_linear_prefactor() - point_1.get_m_x_component().get_m_linear_prefactor());
    denominator.set_m_constant_value(point_2.get_m_x_component().get_m_constant_value() - point_1.get_m_x_component().get_m_constant_value());

    if( (numerator.get_m_linear_prefactor().isZero() && numerator.get_m_constant_value().isZero()) )
    {
        gradient = 0.0;
    }
    else if( (abs(numerator.get_m_linear_prefactor()) == abs(numerator.get_m_constant_value())
        && abs(denominator.get_m_linear_prefactor()) == abs(denominator.get_m_constant_value()))

        || (numerator.get_m_constant_value().isZero() && denominator.get_m_constant_value().isZero()))
    {
        gradient = numerator.get_m_linear_prefactor() / denominator.get_m_linear_prefactor();
    }
    

    return gradient;
}



Kernel::FT btlf::LinearEquation::calculate_m_gradient(const LinearEquation &segmentEquation) const
{   
    // Hier müssste die Exception geschmissen werden!
    if(segmentEquation.get_m_gradient().isZero())
    {
        return 0.0;
    }

    return (-1)/segmentEquation.get_m_gradient();
}



btlf::LinearInterpolation btlf::LinearEquation::calculate_m_y_axis(const MovingPoint &point, const Kernel::FT gradient) const
{
    Kernel::FT y_axis_t_value = 
        (point.get_m_y_component().get_m_linear_prefactor() - m_gradient * point.get_m_x_component().get_m_linear_prefactor());
    
    Kernel::FT y_axis_constant_value = 
        (point.get_m_y_component().get_m_constant_value() - gradient * point.get_m_x_component().get_m_constant_value());


    auto y_axis_info = LinearInterpolation{0.0, 0.0};
    y_axis_info.set_m_linear_prefactor(y_axis_t_value);
    y_axis_info.set_m_constant_value(y_axis_constant_value);

    return y_axis_info;
}



/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::SegmentEquation::SegmentEquation(const MovingPoint &point_1, const MovingPoint &point_2, const MovingPoint &moving_point_to_segment, const SegmentType segment_type, const std::string name) :
    LinearEquation(point_1, point_2, name),
    m_segment_type(segment_type),
    m_segment_point1(point_1),
    m_segment_point2(point_2),
    m_moving_point_to_segment(moving_point_to_segment),
    m_perpendicular(std::make_shared<Perpendicular>(*this, moving_point_to_segment, "L" + m_name)),
    m_interval(determine_m_interval())
{
    
}



btlf::SegmentEquation::SegmentEquation(const SegmentEquation &other):
    LinearEquation(other.m_segment_point1, other.m_segment_point2, other.m_name),
    m_segment_type(other.m_segment_type),
    m_segment_point1(other.m_segment_point1),
    m_segment_point2(other.m_segment_point2),
    m_moving_point_to_segment(other.m_moving_point_to_segment),
    m_perpendicular(std::make_shared<Perpendicular>(other, other.m_moving_point_to_segment, "L" + other.m_name)),
    m_interval(other.m_interval)
{

}



const btlf::Perpendicular &btlf::SegmentEquation::get_m_perpendicular() const
{
    return *m_perpendicular;
}



const btlf::MovingPoint &btlf::SegmentEquation::get_m_moving_point_to_segment() const
{
    return m_moving_point_to_segment;
}



const btlf::Interval &btlf::SegmentEquation::get_m_interval() const
{
    return m_interval;
}



btlf::SegmentType btlf::SegmentEquation::get_m_segment_type() const 
{
    return m_segment_type;
}



/*
    Given: segment_equation_point_1, segment_equation_point_1, perpendicular_point.

    Only the x coordinate (m_x_component) is important for the calculation.

    
    interval_value_1 = (perpendicular_point_constant_value - segment_equation_point_1_constant_value)
                                     /
                        (segment_equation_point_1_t_value - perpendicular_point_t_value);


    interval_value_2 = (segment_equation_point_2_constant_value - perpendicular_point_constant_value)
                                    /
                        (perpendicular_point_t_value - segment_equation_point_2_t_value);

*/
btlf::Interval btlf::SegmentEquation::determine_m_interval()
{   

    // std::cout << "\n\n\n" << this->m_segment_point1.to_string() << "\n";
    // std::cout << this->m_segment_point2.to_string() << "\n";



    Kernel::FT segment_equation_point_1_t_value = this
                                                        ->m_segment_point1
                                                        .get_m_x_component()
                                                        .get_m_linear_prefactor();

    Kernel::FT segment_equation_point_1_constant_value = this
                                                        ->m_segment_point1
                                                        .get_m_x_component()
                                                        .get_m_constant_value();

    Kernel::FT segment_equation_point_2_t_value = this
                                                        ->m_segment_point2
                                                        .get_m_x_component()
                                                        .get_m_linear_prefactor();

    Kernel::FT segment_equation_point_2_constant_value = this
                                                        ->m_segment_point2
                                                        .get_m_x_component()
                                                        .get_m_constant_value();

    Kernel::FT perpendicular_point_t_value = m_perpendicular
                                                        ->get_perpendicular_point_x_component()
                                                        .get_m_linear_prefactor();

    Kernel::FT perpendicular_point_constant_value = m_perpendicular
                                                        ->get_perpendicular_point_x_component()
                                                        .get_m_constant_value();



    Kernel::FT interval_value_1 = (perpendicular_point_constant_value - segment_equation_point_1_constant_value)
                                                /
                                    (segment_equation_point_1_t_value - perpendicular_point_t_value);


    Kernel::FT interval_value_2 = (segment_equation_point_2_constant_value - perpendicular_point_constant_value)
                                                /
                                    (perpendicular_point_t_value - segment_equation_point_2_t_value);



    // Kernel::FT start_interval = (interval_value_1 < interval_value_2) ? interval_value_1 : interval_value_2;
    // Kernel::FT end_interval = (interval_value_2 >  interval_value_1) ? interval_value_2 : interval_value_1;

    Kernel::FT upper_limit = -100.0;
    Kernel::FT lower_limit = -100.0;

    Kernel::FT denominator_value1 = segment_equation_point_1_t_value - perpendicular_point_t_value;
    Kernel::FT denominator_value2 = perpendicular_point_t_value - segment_equation_point_2_t_value;



    if(denominator_value1 > 0.0 && denominator_value2 > 0.0 && interval_value_1 < interval_value_2)
    {
        lower_limit = 0.0;
        upper_limit = interval_value_1;
    }

    else if(denominator_value1 > 0.0 && denominator_value2 > 0.0 && interval_value_1 > interval_value_2)
    {
        lower_limit = 0.0;
        upper_limit = interval_value_2;
    }

    else if(denominator_value1 > 0.0 && denominator_value2 < 0.0 && interval_value_1 < interval_value_2)
    {
        lower_limit = -100;
        upper_limit = -100;
    }

    else if(denominator_value1 > 0.0 && denominator_value2 < 0.0 && interval_value_1 > interval_value_2)
    {
        lower_limit = interval_value_2;
        upper_limit = interval_value_1;
    }

    else if(denominator_value1 < 0.0 && denominator_value2 > 0.0 && interval_value_1 < interval_value_2)
    {
        lower_limit = interval_value_1;
        upper_limit = interval_value_2;
    }

    else if(denominator_value1 < 0.0 && denominator_value2 > 0.0 && interval_value_1 > interval_value_2)
    {
        lower_limit = -100;
        upper_limit = -100;
    }

    else if(denominator_value1 < 0.0 && denominator_value2 < 0.0 && interval_value_1 < interval_value_2)
    {
        lower_limit = interval_value_2;
        upper_limit = 1.0;
    }

    else if(denominator_value1 < 0.0 && denominator_value2 < 0.0 && interval_value_1 > interval_value_2)
    {
        lower_limit = interval_value_1;
        upper_limit = 1.0;
    }




    // std::cout << "\nInterval:\n ";
    // std::cout << this->to_string() << "\n";
    // std::cout << lower_limit << ", " << upper_limit << "\n\n\n";



    // std::cout << "\n\n\n" << this->to_string() << "\n" << "Lower: " << lower_limit << ", Upper: " << upper_limit << "\n";
    // std::cout << "Interval_value1: " << interval_value_1 << "\n";
    // std::cout << "denominator_value1: "  << denominator_value1 << "\n";
    // std::cout << "Interval_value2: " << interval_value_2 << "\n";
    // std::cout << "denominator_value2: " << denominator_value2 << "\n";
    // std::cout << "msegment_point1: " << m_segment_point1.to_string() << "\n";
    // std::cout << "msegment_point2: " << m_segment_point2.to_string() << "\n";
    // std::cout << "lot: " << m_perpendicular->get_perpendicular_point_x_component().to_string() << "\n\n\n\n";


    return btlf::Interval{lower_limit, upper_limit, m_segment_type};
}



/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::Perpendicular::Perpendicular(const SegmentEquation &segment_equation, const MovingPoint &moving_point_to_segment, const std::string name) :
    LinearEquation(segment_equation, moving_point_to_segment, name),
    m_perpendicular_point_x_component(calculate_m_perpendicular_point_x_component(segment_equation))
{
    
}



const btlf::LinearInterpolation &btlf::Perpendicular::get_perpendicular_point_x_component() const
{
    return m_perpendicular_point_x_component;
}



btlf::LinearInterpolation btlf::Perpendicular::calculate_m_perpendicular_point_x_component(const SegmentEquation &segment_equation)
{
    LinearInterpolation m_x_perpendicular_point;

    if(segment_equation.get_m_gradient() == 0){
        m_perpendicular_point_x_component.set_m_linear_prefactor(segment_equation
                                                                .get_m_moving_point_to_segment()
                                                                .get_m_x_component()
                                                                .get_m_linear_prefactor());

        m_perpendicular_point_x_component.set_m_constant_value(segment_equation
                                                                .get_m_moving_point_to_segment()
                                                                .get_m_x_component()
                                                                .get_m_constant_value());
    }
    else
    {
        Kernel::FT gradient_segment = segment_equation.get_m_gradient();
        Kernel::FT gradient_perpendicular = this->get_m_gradient();
        
        Kernel::FT t_value_segment = segment_equation.get_m_y_axis().get_m_linear_prefactor();
        Kernel::FT t_value_perpendicular = this->get_m_y_axis().get_m_linear_prefactor();

        Kernel::FT constant_value_segment = segment_equation.get_m_y_axis().get_m_constant_value();
        Kernel::FT constant_value_perpendicular = this->get_m_y_axis().get_m_constant_value();


        m_x_perpendicular_point.set_m_linear_prefactor(
            (t_value_perpendicular - t_value_segment) / (gradient_segment - gradient_perpendicular)
        );

        m_x_perpendicular_point.set_m_constant_value(
            (constant_value_perpendicular - constant_value_segment) / (gradient_segment - gradient_perpendicular)
        );

    }

    return m_x_perpendicular_point;
}







