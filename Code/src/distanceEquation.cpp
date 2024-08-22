#include "distanceEquation.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <limits>





btlf::HesseNormalForm::HesseNormalForm(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point)
{
    auto hesse_normal_form_prefactors = caluculate_hesse_normal_form(segment_equation, to_segment_point);

    m_linear_prefactor = hesse_normal_form_prefactors.first;
    m_constant_value = hesse_normal_form_prefactors.second;
}



const Kernel::FT &btlf::HesseNormalForm::get_m_linear_prefactor() const
{
    return m_linear_prefactor;
}



const Kernel::FT &btlf::HesseNormalForm::get_m_constant_value() const
{
    return m_constant_value;
}



btlf::HesseNormalFormPair btlf::HesseNormalForm::caluculate_hesse_normal_form(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point)
{   
    const Kernel::FT y_prefactor_value = 1.0;
    const Kernel::FT x_prefactor_value = - segment_equation.get_m_gradient();

    btlf::LinearInterpolation new_y_axis;
    new_y_axis.set_m_linear_prefactor(-1.0 * segment_equation.get_m_y_axis().get_m_linear_prefactor());
    new_y_axis.set_m_constant_value(-1.0 * segment_equation.get_m_y_axis().get_m_constant_value());

    // Test output
    // std::cout << to_segment_point.to_string() << '\n';    
    // std::cout << y_prefactor_value << ", " << x_prefactor_value << ", " << new_y_axis.to_string() << '\n';

    Kernel::FT local_t_value = 
                                x_prefactor_value * to_segment_point.get_m_x_component().get_m_linear_prefactor()
                                            +
                                y_prefactor_value * to_segment_point.get_m_y_component().get_m_linear_prefactor()
                                            +
                                new_y_axis.get_m_linear_prefactor();
           

    Kernel::FT local_constant =
                                x_prefactor_value * to_segment_point.get_m_x_component().get_m_constant_value()
                                            +
                                y_prefactor_value * to_segment_point.get_m_y_component().get_m_constant_value()
                                            +
                                new_y_axis.get_m_constant_value();


    Kernel::FT divider =  sqrt(pow(x_prefactor_value, 2.0) + pow(y_prefactor_value, 2.0));


    // Test output
    // std::cout << std::setprecision(10) << "new_t_value: " << local_t_value / divider << '\n';
    // std::cout << std::setprecision(10) << "new_constant: " << local_constant / divider << '\n';

    return btlf::HesseNormalFormPair(local_t_value / divider, local_constant / divider);
}



/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::BinomialFormula::BinomialFormula(const Kernel::FT &linear_prefactor, const Kernel::FT &constant_value)
{
    auto binomial_formula_result = applying_binomial_formula(linear_prefactor, constant_value);

    m_quadratic_prefactor = std::get<btlf::TUPLE_VALUE_ZERO>(binomial_formula_result);

    m_linear_prefactor = std::get<btlf::TUPLE_VALUE_ONE>(binomial_formula_result);

    m_constant_value = std::get<btlf::TUPLE_VALUE_TWO>(binomial_formula_result);
}



btlf::BinomialFormula::BinomialFormula(const Kernel::FT &quadratic_prefactor, const Kernel::FT &linear_prefactor, const Kernel::FT &constant_value):
    m_quadratic_prefactor(quadratic_prefactor),
    m_linear_prefactor(linear_prefactor),
    m_constant_value(constant_value)
{

}



const Kernel::FT &btlf::BinomialFormula::get_m_quadratic_prefactor() const
{
    return m_quadratic_prefactor;
}



const Kernel::FT &btlf::BinomialFormula::get_m_linear_prefactor() const
{
    return m_linear_prefactor;
}



const Kernel::FT &btlf::BinomialFormula::get_m_constant_value() const
{
    return m_constant_value;
}



btlf::BinomialTuple btlf::BinomialFormula::applying_binomial_formula(const Kernel::FT &linear_prefactor, const Kernel::FT &constant_value)
{
    Kernel::FT result_quadratic_prefactor, result_linear_prefactor, result_constant_value;

    /*
        If necessary, convert form (-a*t + b) to form (a*t - b).
        Where a and b are Kernel::FT values and t is the variable.
    */
    Kernel::FT local_linear_prefactor = linear_prefactor < 0.0 ? -linear_prefactor : linear_prefactor;
    Kernel::FT local_constant_value = linear_prefactor < 0.0 ? -constant_value : constant_value;

    /*
        First & Second binomial formula calculation.
    */
    result_quadratic_prefactor = pow(local_linear_prefactor, 2.0);
    result_linear_prefactor = 2.0 * local_linear_prefactor * local_constant_value;
    result_constant_value = pow(local_constant_value, 2.0);

    /*
        Test output
    */
    // std::cout << result_t_quadratic << "t² " << result_t_linear << "t " << result_t_constant << "\n\n";

    return btlf::BinomialTuple{result_quadratic_prefactor, result_linear_prefactor, result_constant_value};
}



/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/

/*
    Initialization of the static variable m_static_next_id.
*/
std::uint32_t btlf::DistanceEquation::m_static_next_id = static_cast<std::uint32_t>(0.0L);


btlf::DistanceEquation::DistanceEquation(Kernel::FT &quadratic_prefactor, Kernel::FT &linear_prefactor, Kernel::FT &const_value) :
    m_id(m_static_next_id),
    m_quadratic_prefactor(quadratic_prefactor),
    m_linear_prefactor(linear_prefactor),
    m_constant_value(const_value),
    m_name("Taylor"),
    m_distance_equation_type(DistanceEquationType::TAYLOR)
{
    m_static_next_id++;
}


/*
    Case: No segment
*/
btlf::DistanceEquation::DistanceEquation(const MovingPoint &mp_start, const MovingPoint &mp_end, const MovingPoint &mp, SegmentType type, std::string name):
    m_id(m_static_next_id++),
    m_quadratic_prefactor(0.0),
    m_linear_prefactor(mp_start.get_m_x_component().get_m_linear_prefactor() - mp.get_m_x_component().get_m_linear_prefactor()),
    m_constant_value(mp_start.get_m_x_component().get_m_constant_value() - mp.get_m_x_component().get_m_constant_value()),
    m_name(name),
    m_distance_equation_type(DistanceEquationType::LINEAR)
{

    Kernel::FT mp_start_y_t_prefactor = mp_start
                                                        .get_m_y_component()
                                                        .get_m_linear_prefactor();

    Kernel::FT mp_start_y_constant = mp_start
                                                        .get_m_y_component()
                                                        .get_m_constant_value();

    Kernel::FT mp_end_y_t_prefactor = mp_end
                                                        .get_m_y_component()
                                                        .get_m_linear_prefactor();

    Kernel::FT mp_end_y_constant = mp_end
                                                        .get_m_y_component()
                                                        .get_m_constant_value();


    Kernel::FT mp_y_t_prefactor = mp
                                                        .get_m_y_component()
                                                        .get_m_linear_prefactor();


    Kernel::FT mp_y_constant = mp
                                                        .get_m_y_component()
                                                        .get_m_constant_value();



    Kernel::FT interval_value_1 = (mp_y_constant - mp_start_y_constant)
                                                /
                                    (mp_start_y_t_prefactor - mp_y_t_prefactor);


    Kernel::FT interval_value_2 = (mp_end_y_constant - mp_y_constant)
                                                /
                                    (mp_y_t_prefactor - mp_end_y_t_prefactor);



    // Kernel::FT start_interval = (interval_value_1 < interval_value_2) ? interval_value_1 : interval_value_2;
    // Kernel::FT end_interval = (interval_value_2 >  interval_value_1) ? interval_value_2 : interval_value_1;

    Kernel::FT upper_limit = -100.0;
    Kernel::FT lower_limit = -100.0;

    Kernel::FT denominator_value1 = mp_start_y_t_prefactor - mp_y_t_prefactor;
    Kernel::FT denominator_value2 = mp_y_t_prefactor - mp_end_y_t_prefactor;



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


    // std::cout << "\n\nLower: " << lower_limit << ", Upper: " << upper_limit << "\n";
    // std::cout << "Interval_value1: " << interval_value_1 << "\n";
    // std::cout << "denominator_value1: "  << denominator_value1 << "\n";
    // std::cout << "Interval_value2: " << interval_value_2 << "\n";
    // std::cout << "denominator_value2: " << denominator_value2 << "\n";
    // std::cout << "msegment_point1: " << mp_start.to_string() << "\n";
    // std::cout << "msegment_point2: " << mp_end.to_string() << "\n";
    // std::cout << "mp: " << mp.to_string() << "\n\n\n";

    // std::this_thread::sleep_for(std::chrono::seconds(10));

    m_interval = btlf::Interval{lower_limit, upper_limit, type};

}








btlf::DistanceEquation::DistanceEquation() :
    m_id(m_static_next_id),
    m_quadratic_prefactor(0.0),
    m_linear_prefactor(0.0),
    m_constant_value(0.0),
    m_name("Zero"),
    m_distance_equation_type(DistanceEquationType::ZERO)
{   
    auto interval = btlf::Interval{};
    m_static_next_id++;
}



btlf::DistanceEquation::DistanceEquation(const MovingPoint &segment_point, const MovingPoint &to_segment_point, const std::string name):
    m_id(m_static_next_id),
    m_name(name),
    m_interval(btlf::Interval{}),
    m_distance_equation_type(DistanceEquationType::QUADRATIC)
{
    auto member_parts = caluculate_member_variables(segment_point, to_segment_point);

    m_quadratic_prefactor = member_parts.get_m_quadratic_prefactor();
    m_linear_prefactor = member_parts.get_m_linear_prefactor();
    m_constant_value = member_parts.get_m_constant_value();

    m_static_next_id++;
}



btlf::DistanceEquation::DistanceEquation(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point, const std::string name):
    m_id(m_static_next_id),
    m_quadratic_prefactor(0.0),
    m_name(name),
    m_interval(segment_equation.get_m_interval()),
    m_distance_equation_type(DistanceEquationType::LINEAR)
{
    auto member_parts = HesseNormalForm{segment_equation, to_segment_point};

    m_linear_prefactor = member_parts.get_m_linear_prefactor();
    m_constant_value = member_parts.get_m_constant_value();

    m_static_next_id++;
}


btlf::DistanceEquation::DistanceEquation(const std::shared_ptr<DistanceEquation> &other):
    m_id(m_static_next_id),
    m_quadratic_prefactor(other->m_quadratic_prefactor),
    m_linear_prefactor(other->m_linear_prefactor),
    m_constant_value(other->m_constant_value),
    m_name(other->m_name),
    m_interval(other->m_interval),
    m_distance_equation_type(other->m_distance_equation_type)
{
    m_static_next_id++;
}




void btlf::DistanceEquation::set_m_linear_prefactor(const Kernel::FT &linear_prefactor)
{
    m_linear_prefactor = linear_prefactor;
}



void btlf::DistanceEquation::set_m_constant_value(const Kernel::FT &constant_value)
{
    m_constant_value = constant_value;
}


void btlf::DistanceEquation::set_m_quadratic_prefactor(const Kernel::FT &prefactor)
{
    m_quadratic_prefactor = prefactor;
}




const Kernel::FT &btlf::DistanceEquation::get_m_quadratic_prefactor() const
{
    return m_quadratic_prefactor;
}



const Kernel::FT &btlf::DistanceEquation::get_m_linear_prefactor() const
{
    return m_linear_prefactor;
}



const Kernel::FT &btlf::DistanceEquation::get_m_constant_value() const
{
    return m_constant_value;
}



const std::string &btlf::DistanceEquation::get_m_name() const
{
    return m_name;
}



const btlf::Interval &btlf::DistanceEquation::get_m_interval() const
{
    return m_interval;
}



std::uint32_t btlf::DistanceEquation::get_m_id() const
{
    return m_id;
}



btlf::DistanceEquationType btlf::DistanceEquation::get_m_distance_equation_type() const
{
    return m_distance_equation_type;
}



const btlf::MovingPoint &btlf::DistanceEquation::get_m_to_segment() const
{
    return m_to_segment_point;
}




btlf::IntersectionResults btlf::DistanceEquation::intersect(DistanceEquation &other_distance_equation) const
{
    /*
        Test output
    */
    // std::cout << "\n\n" << this->to_string() << '\n';
    // std::cout << other_distance_equation.to_string() << "\n\n";


    if( (this->m_distance_equation_type == DistanceEquationType::LINEAR) && 
        (other_distance_equation.m_distance_equation_type == DistanceEquationType::LINEAR) )
    {   
        // std::cout << "linear mit linear\n\n";

        return intersect_linear_with_linear(other_distance_equation);
    }

    else
    {   

        return intersect_quadratic_with_linear(other_distance_equation);
    }


}






Kernel::FT btlf::DistanceEquation::calculate_possible_linear_intersection_point(DistanceEquation &linear_eqaution, btlf::LinearCalculationCase calculation_case) const
{
    auto intersection_point = btlf::INVALID_INTERSECTION_VALUE;

    bool condition1_is_valid = false;
    bool condition2_is_valid = false;


    auto this_linear_prefactor = this->get_m_linear_prefactor();
    auto this_constant_value = this->get_m_constant_value();
    auto linear_equation_linear_prefactor = linear_eqaution.get_m_linear_prefactor();
    auto linear_equation_constant_value = linear_eqaution.get_m_constant_value();


    if( (calculation_case == btlf::LinearCalculationCase::CASE_1) && !(this_linear_prefactor - linear_equation_linear_prefactor).isZero() )
    {
        intersection_point = (linear_equation_constant_value - this_constant_value) / (this_linear_prefactor - linear_equation_linear_prefactor);

        // std::cout << "Case1: " << intersection_point << "\n\n";

        condition1_is_valid = check_intersection_point_condition(*this, intersection_point, true);
        condition2_is_valid = check_intersection_point_condition(linear_eqaution, intersection_point, true);
    }

    else if( (calculation_case == btlf::LinearCalculationCase::CASE_2) && !(this_linear_prefactor + linear_equation_linear_prefactor).isZero() )
    {
        intersection_point = (-1.0 * linear_equation_constant_value - this_constant_value) / (this_linear_prefactor + linear_equation_linear_prefactor);

        // std::cout << "Case2: " << intersection_point << "\n\n";

        condition1_is_valid = check_intersection_point_condition(*this, intersection_point, true);
        condition2_is_valid = check_intersection_point_condition(linear_eqaution, intersection_point, false);
    }

    else if( (calculation_case == btlf::LinearCalculationCase::CASE_3) && !(-1.0 * this_linear_prefactor - linear_equation_constant_value).isZero() )
    {
        intersection_point = (linear_equation_constant_value + this_constant_value) / (-1.0 * this_linear_prefactor - linear_equation_linear_prefactor);

        // std::cout << "Case3: " << intersection_point << "\n\n";

        condition1_is_valid = check_intersection_point_condition(*this, intersection_point, false);
        condition2_is_valid = check_intersection_point_condition(linear_eqaution, intersection_point, true);
    }

    else if( (calculation_case == btlf::LinearCalculationCase::CASE_4) && !(linear_equation_linear_prefactor - this_linear_prefactor).isZero() )
    {
        intersection_point = (this_constant_value - linear_equation_constant_value) / (linear_equation_linear_prefactor - this_linear_prefactor);

        // std::cout << "Case4: " << intersection_point << "\n\n";

        condition1_is_valid = check_intersection_point_condition(*this, intersection_point, false);
        condition2_is_valid = check_intersection_point_condition(linear_eqaution, intersection_point, false);
    }


    intersection_point = (condition1_is_valid && condition2_is_valid) ? intersection_point : btlf::INVALID_INTERSECTION_VALUE;

    return intersection_point;
}





/*
    Helper function to check the validity of a solution.

    mode true:  Case f(t) >= 0.0
    mode false: Case f(t) < 0.0
*/
bool btlf::DistanceEquation::check_intersection_point_condition(const DistanceEquation &linear_equation, const Kernel::FT &intersection_point, const bool mode) const
{
    auto linear_prefactor = linear_equation.get_m_linear_prefactor();
    auto constant_value = linear_equation.get_m_constant_value();

    auto is_valid = false;

    Kernel::FT compare_value = 0.0;


    if(linear_prefactor != 0.0)
    {
        compare_value = (-1.0 * constant_value) / linear_prefactor;
    }
    else
    {
        std::cout << "Linear prefactor is Zero!\n\n";
    }

    /*
        Test output
    */
    // std::cout << "Compare value: " << compare_value << "\n\n";

    if( linear_prefactor > 0.0 && mode)
    {
        is_valid = intersection_point >= compare_value;
    }
    else if( linear_prefactor < 0.0 && mode)
    {
        is_valid = intersection_point <= compare_value;
    }
    else if( linear_prefactor > 0.0 && !mode)
    {
        is_valid = intersection_point < compare_value;
    }
    else if( linear_prefactor < 0.0 && !mode)
    {
        is_valid = intersection_point > compare_value;
    }


    return is_valid;
}



btlf::IntersectionResults btlf::DistanceEquation::intersect_linear_with_linear(DistanceEquation &equation) const
 {    
    auto intersection_points = IntersectionResults();

    intersection_points[0] = calculate_possible_linear_intersection_point(equation, btlf::LinearCalculationCase::CASE_1);
    intersection_points[1] = calculate_possible_linear_intersection_point(equation, btlf::LinearCalculationCase::CASE_2);
    intersection_points[2] = calculate_possible_linear_intersection_point(equation, btlf::LinearCalculationCase::CASE_3);
    intersection_points[3] = calculate_possible_linear_intersection_point(equation, btlf::LinearCalculationCase::CASE_4);

    return intersection_points;
}













btlf::IntersectionPair btlf::DistanceEquation::calculate_quadratic_linear_case1(const DistanceEquation &other_equation) const
{
    auto result = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);

    /*
        Determine the correct values for the calculation.
    */
    auto quadratic_constant_c2 = 
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_constant_value : this->m_constant_value;

    auto linear_constant_c1 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? this->m_constant_value : other_equation.m_constant_value;

    auto linear_prefactor_a1 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? this->m_linear_prefactor : other_equation.m_linear_prefactor;

    auto quadratic_linear_prefactor_b2 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_linear_prefactor : this->m_linear_prefactor;


    /*
        Do the calculation.
    */

    result = (quadratic_constant_c2 - pow(linear_constant_c1, 2.0)) / (2.0 * linear_prefactor_a1 * linear_constant_c1 - quadratic_linear_prefactor_b2);

    return IntersectionPair{result, btlf::INVALID_INTERSECTION_VALUE};
}



btlf::IntersectionPair btlf::DistanceEquation::calculate_quadratic_linear_case2(const DistanceEquation &other_equation) const
{
    auto result_first = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);
    auto result_second = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);


    auto quadratic_constant_c2 = 
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_constant_value : this->m_constant_value;

    auto linear_constant_c1 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? this->m_constant_value : other_equation.m_constant_value;

    auto linear_prefactor_a1 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? this->m_linear_prefactor : other_equation.m_linear_prefactor;

    auto quadratic_prefactor_a2 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_quadratic_prefactor : this->m_quadratic_prefactor;



    result_first = sqrt( (quadratic_constant_c2 - pow(linear_constant_c1, 2.0)) / (pow(linear_prefactor_a1, 2.0) - quadratic_prefactor_a2) );

    result_second = -1.0 * result_first;

    return IntersectionPair{result_first, result_second};
}



btlf::IntersectionPair btlf::DistanceEquation::calculate_quadratic_linear_case3(const DistanceEquation &other_equation) const
{
    auto result_first = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);
    auto result_second = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);

    auto linear_prefactor_a1 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? this->m_linear_prefactor : other_equation.m_linear_prefactor;

    auto quadratic_prefactor_a2 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_quadratic_prefactor : this->m_quadratic_prefactor;

    auto quadratic_constant_c2 = 
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_constant_value : this->m_constant_value;

    auto linear_constant_c1 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? this->m_constant_value : other_equation.m_constant_value;
    
    auto quadratic_linear_prefactor_b2 =
        (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) ? other_equation.m_linear_prefactor : this->m_linear_prefactor;


    const auto discriminatory =
        4.0 * (pow(linear_prefactor_a1, 2.0) * quadratic_constant_c2 - linear_prefactor_a1 * quadratic_linear_prefactor_b2 * linear_constant_c1 + quadratic_prefactor_a2 * pow(linear_constant_c1, 2.0) - quadratic_prefactor_a2 * quadratic_constant_c2)
        + pow(quadratic_linear_prefactor_b2, 2.0);



    if(discriminatory.isZero())
    {
        result_first = (quadratic_linear_prefactor_b2 - 2.0 * linear_prefactor_a1 * linear_constant_c1) / (2.0 * pow(linear_prefactor_a1, 2.0) - 2.0 * quadratic_prefactor_a2);
        result_second = result_first;
    }

    else if(discriminatory > 0.0)
    {
        result_first = 
                        (quadratic_linear_prefactor_b2 - 2.0 * linear_prefactor_a1 * linear_constant_c1 + sqrt(discriminatory))
                        /
                        (2.0 * pow(linear_prefactor_a1, 2.0) - 2.0 * quadratic_prefactor_a2);

        result_second = 
                        (quadratic_linear_prefactor_b2 - 2.0 * linear_prefactor_a1 * linear_constant_c1 - sqrt(discriminatory))
                        /
                        (2.0 * pow(linear_prefactor_a1, 2.0) - 2.0 * quadratic_prefactor_a2);
    }

    return IntersectionPair{result_first, result_second};
}










btlf::IntersectionPair btlf::DistanceEquation::calculate_possible_quadratic_intersection_points(const DistanceEquation &other_equation, EquationPrefactorTuple &prefactor_tuple, QuadraticCalculationCase calculation_case) const
{
    auto summarized_quadratic_prefactor = std::get<btlf::TUPLE_VALUE_ZERO>(prefactor_tuple);
    auto summarized_linear_prefactor = std::get<btlf::TUPLE_VALUE_ONE>(prefactor_tuple);
    auto summarized_constant_value = std::get<btlf::TUPLE_VALUE_TWO>(prefactor_tuple);

    auto summarized_quadratic_prefactor_is_zero = summarized_quadratic_prefactor.isZero();
    auto summarized_linear_prefactor_is_zero = summarized_linear_prefactor.isZero();

    IntersectionPair intersection_points;

    auto condition_point_first = bool(false);
    auto condition_point_second = bool(false);
    auto mock_test_first = bool(true);
    auto mock_test_second = bool(true);

    auto condition_case = bool(false);


    if( (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) || (other_equation.m_distance_equation_type == btlf::DistanceEquationType::LINEAR) )
    {
        if(summarized_quadratic_prefactor_is_zero && !summarized_linear_prefactor_is_zero)
        {
            intersection_points = calculate_quadratic_linear_case1(other_equation);
        }
        
        else if(!summarized_quadratic_prefactor_is_zero && summarized_linear_prefactor_is_zero)
        {
            intersection_points = calculate_quadratic_linear_case2(other_equation);
        }

        else if(!summarized_quadratic_prefactor_is_zero && !summarized_linear_prefactor_is_zero)
        {
            intersection_points = calculate_quadratic_linear_case3(other_equation);
        }



        condition_case = (calculation_case == btlf::QuadraticCalculationCase::CASE_1) ? true : false;

        if(this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR)
        {
            condition_point_first = check_intersection_point_condition(*this, intersection_points.first, condition_case);
            condition_point_second = check_intersection_point_condition(*this, intersection_points.second, condition_case);
        }
        
        else if(other_equation.m_distance_equation_type == btlf::DistanceEquationType::LINEAR)
        {
            condition_point_first = check_intersection_point_condition(other_equation, intersection_points.first, condition_case);
            condition_point_second = check_intersection_point_condition(other_equation, intersection_points.second, condition_case);
        }



        // mock_test_first = mock_solution_test(intersection_points.first, other_equation);
        // mock_test_second = mock_solution_test(intersection_points.second, other_equation);

        // std::cout << "other: " << other_equation.to_string() << "\n";
        // std::cout << "this: " << this->to_string() << "\n";

        // const auto start = ClockType::now();

        auto other_1 = other_equation.calculate_value(intersection_points.first).doubleValue();
        auto this_1 = this->calculate_value(intersection_points.first).doubleValue();

        // std::cout << other_1 << "\n";
        // std::cout << this_1 << "\n";

        auto other_2 = other_equation.calculate_value(intersection_points.second).doubleValue();
        auto this_2 = this->calculate_value(intersection_points.second).doubleValue();

        mock_test_first = (other_1 - this_1) < std::abs(0.00001);
        mock_test_second = (other_2 - this_2) < std::abs(0.00001);
        
        // const auto end = ClockType::now();

        // const auto elapsed_time = std::chrono::duration_cast<ClockRes>(end - start).count();

        // std::cout << "\n\nZeit Intersection: " << elapsed_time << "ms\n\n";


        // if (CGAL::compare(this->calculate_value(intersection_points.first), other_equation.calculate_value(intersection_points.first)) == CGAL::EQUAL)
        // {
        //     mock_test_first = true;
        // }

        // if (CGAL::compare(this->calculate_value(intersection_points.second), other_equation.calculate_value(intersection_points.second)) == CGAL::EQUAL)
        // {
        //     mock_test_second = true;
        // }


        // mock_test_first = (this->calculate_value(intersection_points.first)) == (other_equation.calculate_value(intersection_points.first));
        // mock_test_second = (this->calculate_value(intersection_points.second)) == (other_equation.calculate_value(intersection_points.second));


        intersection_points.first = (mock_test_first && condition_point_first) ? intersection_points.first : btlf::INVALID_INTERSECTION_VALUE;
        intersection_points.second = (mock_test_second && condition_point_second) ? intersection_points.second : btlf::INVALID_INTERSECTION_VALUE;

    }


    return intersection_points;
}



btlf::IntersectionPair btlf::DistanceEquation::calculate_quadratic_quadratic_case1(const EquationPrefactorTuple &prefactor_tuple) const
{
    auto result = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);

    auto summarized_constant_values = std::get<btlf::TUPLE_VALUE_TWO>(prefactor_tuple);
    auto summarized_linear_prefactor = std::get<btlf::TUPLE_VALUE_ONE>(prefactor_tuple);

    result = summarized_constant_values / summarized_linear_prefactor;

    return IntersectionPair{result, btlf::INVALID_INTERSECTION_VALUE};
}



btlf::IntersectionPair btlf::DistanceEquation::calculate_quadratic_quadratic_case2(const EquationPrefactorTuple &prefactor_tuple) const
{
    auto result_first = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);
    auto result_second = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);

    auto summarized_constant_values = std::get<btlf::TUPLE_VALUE_TWO>(prefactor_tuple);
    auto summaried_quadratic_prefactor = std::get<btlf::TUPLE_VALUE_ZERO>(prefactor_tuple);

    result_first = sqrt( summarized_constant_values / summaried_quadratic_prefactor );
    result_second = -1.0 * result_first;

    return IntersectionPair{result_first, result_second};
}



btlf::IntersectionPair btlf::DistanceEquation::calculate_quadratic_quadratic_case3(const EquationPrefactorTuple &prefactor_tuple) const
{
    const auto summarized_linear_prefactor = std::get<btlf::TUPLE_VALUE_ONE>(prefactor_tuple);
    const auto summaried_quadratic_prefactor = std::get<btlf::TUPLE_VALUE_ZERO>(prefactor_tuple);

    const auto discriminatory = calculate_discriminatory(prefactor_tuple);

    auto result_first = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);
    auto result_second = Kernel::FT(btlf::INVALID_INTERSECTION_VALUE);
    
    if(discriminatory.isZero())
    {
        result_first = (-1.0 * summarized_linear_prefactor) / (2.0 * summaried_quadratic_prefactor);
        result_second = result_first;
    }

    else if(discriminatory > 0.0)
    {
        result_first = (-1.0 * summarized_linear_prefactor + sqrt(discriminatory)) / (2.0 * summaried_quadratic_prefactor);
        result_second = (-1.0 * summarized_linear_prefactor - sqrt(discriminatory)) / (2.0 * summaried_quadratic_prefactor);
    }

    return IntersectionPair{result_first, result_second};
}




btlf::IntersectionPair btlf::DistanceEquation::calculate_true_quadratic_intersection_points(const EquationPrefactorTuple &prefactor_tuple) const
{
    auto summarized_quadratic_prefactor = std::get<btlf::TUPLE_VALUE_ZERO>(prefactor_tuple);
    auto summarized_linear_prefactor = std::get<btlf::TUPLE_VALUE_ONE>(prefactor_tuple);
    auto summarized_constant_value = std::get<btlf::TUPLE_VALUE_TWO>(prefactor_tuple);

    auto summarized_quadratic_prefactor_is_zero = summarized_quadratic_prefactor.isZero();
    auto summarized_linear_prefactor_is_zero = summarized_linear_prefactor.isZero();

    IntersectionPair intersection_points;

    if(summarized_quadratic_prefactor_is_zero && !summarized_linear_prefactor_is_zero)
    {
        intersection_points = calculate_quadratic_quadratic_case1(prefactor_tuple);
    }

    else if(!summarized_quadratic_prefactor_is_zero && summarized_linear_prefactor_is_zero)
    {
        intersection_points = calculate_quadratic_quadratic_case2(prefactor_tuple);
    }

    else if(!summarized_quadratic_prefactor_is_zero && !summarized_linear_prefactor_is_zero)
    {
        intersection_points = calculate_quadratic_quadratic_case3(prefactor_tuple);
    }

    return intersection_points;
}





btlf::IntersectionResults btlf::DistanceEquation::intersect_quadratic_with_linear(const DistanceEquation &other_equation) const
{  
    auto equation_prefactor_tuple = calculate_equation_prefactors_for_calculation(other_equation);
    
    if( (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) || (other_equation.m_distance_equation_type == btlf::DistanceEquationType::LINEAR) )
    {   

        //std::cout << "linear mit quad\n\n";

        auto intersection_part1 = calculate_possible_quadratic_intersection_points(other_equation, equation_prefactor_tuple, btlf::QuadraticCalculationCase::CASE_1);

        auto intersection_part2 = calculate_possible_quadratic_intersection_points(other_equation, equation_prefactor_tuple, btlf::QuadraticCalculationCase::CASE_2);

        return IntersectionResults{intersection_part1.first, intersection_part1.second, intersection_part2.first, intersection_part2.second};

    }

    else if( (this->m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC) && (other_equation.m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC) )
    {   

       // std::cout << "quad mit quad\n\n";

        auto intersection = calculate_true_quadratic_intersection_points(equation_prefactor_tuple);

        return IntersectionResults{intersection.first, intersection.second, btlf::INVALID_INTERSECTION_VALUE, btlf::INVALID_INTERSECTION_VALUE};
    }


    return IntersectionResults{btlf::INVALID_INTERSECTION_VALUE, btlf::INVALID_INTERSECTION_VALUE, btlf::INVALID_INTERSECTION_VALUE, btlf::INVALID_INTERSECTION_VALUE};
}















btlf::EquationPrefactorTuple btlf::DistanceEquation::calculate_equation_prefactors_for_calculation(const DistanceEquation &other_equation) const
{   
    auto summarized_quadratic_prefactor = Kernel::FT(0.0);
    auto summarized_linear_prefactor = Kernel::FT(0.0);
    auto summarized_constant_value = Kernel::FT(0.0);

    if( (this->m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC) &&
        (other_equation.m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC) )
    {
        summarized_quadratic_prefactor = this->m_quadratic_prefactor - other_equation.m_quadratic_prefactor;
        summarized_linear_prefactor = this->m_linear_prefactor - other_equation.m_linear_prefactor;
        summarized_constant_value = this->m_constant_value - other_equation.m_constant_value;
    }
   
    else if( (this->m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC) && 
             (other_equation.m_distance_equation_type == btlf::DistanceEquationType::LINEAR) )
    {   
        summarized_quadratic_prefactor = pow(other_equation.m_linear_prefactor, 2.0) - this->m_quadratic_prefactor;
        summarized_linear_prefactor = (2.0 * other_equation.m_linear_prefactor * other_equation.m_constant_value) - this->m_linear_prefactor;
        summarized_constant_value = pow(other_equation.m_constant_value, 2.0) - this->m_constant_value;
    }

    else if( (this->m_distance_equation_type == btlf::DistanceEquationType::LINEAR) &&
             (other_equation.m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC) )
    {
        summarized_quadratic_prefactor = pow(this->m_linear_prefactor, 2.0) - other_equation.m_quadratic_prefactor;
        summarized_linear_prefactor = (2.0 * this->m_linear_prefactor * this->m_constant_value) - other_equation.m_linear_prefactor;
        summarized_constant_value = pow(this->m_constant_value, 2.0) - other_equation.m_constant_value;
    }


    return btlf::EquationPrefactorTuple{summarized_quadratic_prefactor, summarized_linear_prefactor, summarized_constant_value};
}



Kernel::FT btlf::DistanceEquation::calculate_discriminatory(const EquationPrefactorTuple &prefactor_tuple) const
{   
    auto summarized_linear_prefactor = std::get<btlf::TUPLE_VALUE_ONE>(prefactor_tuple);
    auto summarized_quadratic_prefactor = std::get<btlf::TUPLE_VALUE_ZERO>(prefactor_tuple);
    auto summarized_constant_value = std::get<btlf::TUPLE_VALUE_TWO>(prefactor_tuple);

    Kernel::FT discriminatory = (pow(summarized_linear_prefactor, 2.0)) - 4.0 * summarized_quadratic_prefactor * summarized_constant_value;

    if( discriminatory.isZero() )
    {   
        discriminatory = 0.0;
    }

    return discriminatory;
}



bool btlf::DistanceEquation::mock_solution_test(const Kernel::FT &test_value, const DistanceEquation &other_equation) const
{   
    auto compare_value_this = this->calculate_value(test_value);
    auto compare_value_other_equation = other_equation.calculate_value(test_value);

    /*
        Test output
    */
    // std::cout << "t: " << test_value << "\n";
    // std::cout << compare_value_this << "\n";
    // std::cout << compare_value_other_equation << "\n";


    return (compare_value_this == compare_value_other_equation);
}




btlf::ZeroPoint btlf::DistanceEquation::caluculate_linear_zeros()
{
    auto zero_point = Kernel::FT{btlf::INVALID_ZERO_POINT};

    /*
        In diesen Fall gibt es keine Nullstelle.
    */
    if(this->m_linear_prefactor.isZero())
    {
        return ZeroPoint{zero_point, btlf::ZeroPointType::INVALID};
    }
    else
    {
        zero_point = (-1.0 * m_constant_value) / m_linear_prefactor;

        return ZeroPoint{zero_point, btlf::ZeroPointType::SINGLE};
    }

}




std::vector<btlf::ZeroPoint> btlf::DistanceEquation::calculate_zeros()
{
    if(m_distance_equation_type == btlf::DistanceEquationType::LINEAR)
    {
        auto zero_point = caluculate_linear_zeros();

        std::vector<btlf::ZeroPoint> result;

        result.push_back(zero_point);

        return result;
    }

    else
    // if(m_distance_equation_type == btlf::DistanceEquationType::QUADRATIC)
    {
        auto zero_points = calculate_quadratic_zeros();

        return zero_points;
    }

}



std::vector<btlf::ZeroPoint> btlf::DistanceEquation::calculate_quadratic_zeros()
{   
    std::vector<btlf::ZeroPoint> zero_points;

    auto zero_point_1 = Kernel::FT{btlf::INVALID_ZERO_POINT};
    auto zero_point_2 = Kernel::FT{btlf::INVALID_ZERO_POINT};

    auto discriminant = Kernel::FT{ pow(m_linear_prefactor, 2.0) - 4.0 * m_quadratic_prefactor * m_constant_value };


    if(discriminant < 0.0)
    {   
        return zero_points;
    }

    else if(discriminant.isZero())
    {

        zero_point_1 = (-1.0 * m_linear_prefactor) / ( 2.0 * m_quadratic_prefactor);

        auto result_point = btlf::ZeroPoint{zero_point_1, btlf::ZeroPointType::DOUBLE};

        zero_points.push_back(result_point);


        return zero_points;
    }

    else if(discriminant > 0.0)
    {

        zero_point_1 = ((-1.0 * m_linear_prefactor) + sqrt(discriminant)) / ( 2.0 * m_quadratic_prefactor);
        zero_point_2 = ((-1.0 * m_linear_prefactor) - sqrt(discriminant)) / ( 2.0 * m_quadratic_prefactor);

        auto result_point1 = btlf::ZeroPoint{zero_point_1, btlf::ZeroPointType::SINGLE};
        auto result_point2 = btlf::ZeroPoint{zero_point_2, btlf::ZeroPointType::SINGLE};

        zero_points.push_back(result_point1);
        zero_points.push_back(result_point2);


        return zero_points;
    }


    return zero_points;

}















btlf::BinomialFormula btlf::DistanceEquation::caluculate_member_variables(const MovingPoint &segment_point, const MovingPoint &to_segment_point)
{   
    Kernel::FT summarized_x_linear_prefactor = to_segment_point.get_m_x_component().get_m_linear_prefactor()
                                                -
                                        segment_point.get_m_x_component().get_m_linear_prefactor();


    Kernel::FT summarized_x_constant_value = to_segment_point.get_m_x_component().get_m_constant_value()
                                                    -
                                                segment_point.get_m_x_component().get_m_constant_value();


    Kernel::FT summarized_y_linear_prefactor = to_segment_point.get_m_y_component().get_m_linear_prefactor()
                                                -
                                        segment_point.get_m_y_component().get_m_linear_prefactor();


    Kernel::FT summarized_y_constant_value = to_segment_point.get_m_y_component().get_m_constant_value()
                                                    -
                                                segment_point.get_m_y_component().get_m_constant_value();


    auto binomial_x = btlf::BinomialFormula{summarized_x_linear_prefactor, summarized_x_constant_value};
    auto binomial_y = btlf::BinomialFormula{summarized_y_linear_prefactor, summarized_y_constant_value};


    return BinomialFormula{
            binomial_x.get_m_quadratic_prefactor() + binomial_y.get_m_quadratic_prefactor(),
            binomial_x.get_m_linear_prefactor() + binomial_y.get_m_linear_prefactor(), 
            binomial_x.get_m_constant_value() + binomial_y.get_m_constant_value()
    };

}





/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::QuadraticDistanceEquation::QuadraticDistanceEquation(Kernel::FT &quadratic_prefactor, Kernel::FT &linear_prefactor, Kernel::FT &const_value) :
    DistanceEquation(quadratic_prefactor, linear_prefactor, const_value)
{

}






btlf::QuadraticDistanceEquation::QuadraticDistanceEquation():
    DistanceEquation()
{

}



btlf::QuadraticDistanceEquation::QuadraticDistanceEquation(const MovingPoint &segment_point, const MovingPoint &to_segment_point, const std::string name):
    DistanceEquation(segment_point, to_segment_point, name),
    m_segment_point(segment_point)
{

}



btlf::QuadraticDistanceEquation::QuadraticDistanceEquation(const std::shared_ptr<DistanceEquation> &other):
    DistanceEquation(other)
{

}



Kernel::FT btlf::QuadraticDistanceEquation::calculate_value(const Kernel::FT &value, bool abso) const
{       
        // unused parameter abso


        return sqrt
        ( 
            m_quadratic_prefactor * pow(value, 2.0) + m_linear_prefactor * value + m_constant_value
        );
}



std::string btlf::QuadraticDistanceEquation::to_string() const
{
    std::ostringstream equation;

    std::string sign_linear = m_linear_prefactor > 0.0 ? " + " : "";
    std::string sign_constant = m_constant_value > 0.0 ? " + " : "";

    equation    
                << std::setprecision(LONG_DOUBLE_DISPLAY_ACCURACY)
                << m_name
                << "(t) = sqrt( "
                << m_quadratic_prefactor
                << "t² "
                << sign_linear
                << m_linear_prefactor
                << "t "
                << sign_constant
                << m_constant_value
                << " )";

    return equation.str();
}





/*
    ############################################################################################################
    ############################################################################################################
    ############################################################################################################
*/



btlf::LinearDistanceEquation::LinearDistanceEquation(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point, std::string name):
    DistanceEquation(segment_equation, to_segment_point, name)
{

};


btlf::LinearDistanceEquation::LinearDistanceEquation(const std::shared_ptr<DistanceEquation> &other):
    DistanceEquation(other)
{

}



/*
    Case : No segment
*/
btlf::LinearDistanceEquation::LinearDistanceEquation(const MovingPoint &to_mp1, const MovingPoint &to_mp2, const MovingPoint &mp, btlf::SegmentType type, std::string name):
    DistanceEquation(to_mp1, to_mp2, mp, type, name)
{
    
}



Kernel::FT btlf::LinearDistanceEquation::calculate_value(const Kernel::FT &value, bool abso) const
{   

    if(abso)
    {
        return abs(m_linear_prefactor * value + m_constant_value);

    }
    else 
    {
        return m_linear_prefactor * value + m_constant_value;
    }

}



std::string btlf::LinearDistanceEquation::to_string() const
{   
    std::ostringstream equation;

    std::string sign_constant = m_constant_value > 0.0 ? " + " : "";

    equation
                << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY)
                << m_name
                << "(t) = "
                << m_linear_prefactor
                << "t "
                << sign_constant
                << m_constant_value;

    return equation.str();
}



void btlf::LinearDistanceEquation::inverse()
{
    m_constant_value = -1.0 * m_constant_value;
    m_linear_prefactor = -1.0 * m_linear_prefactor;
}









btlf::ZeroPoint::ZeroPoint(const Kernel::FT &point, ZeroPointType type) :
    m_point(point),
    m_type(type)
{

}
