#pragma once

#include "mReal.hpp"
#include <limits>
#include <cmath>
#include <chrono>
#include "timeInterpolation.hpp"

using ClockType = std::chrono::steady_clock;
using ClockRes = std::chrono::milliseconds;


namespace btlf
{



bool is_value_inside_equation_interval(const Kernel::FT &value, const std::shared_ptr<btlf::DistanceEquation> &equation){

    bool is_valid = false;

    auto equation_interval = equation->get_m_interval();
    
    auto equation_interval_start_including = equation_interval.get_m_start_including();
    auto equation_interval_end_including = equation_interval.get_m_end_including();

    auto equation_interval_start = equation_interval.get_m_start_interval();
    auto equation_interval_end = equation_interval.get_m_end_interval();

    // std::cout << std::boolalpha << "start: " << equation_interval_start_including << '\n';
    // std::cout << std::boolalpha << "end: " << equation_interval_end_including << '\n';


    if( !equation_interval_start_including && !equation_interval_end_including)
    {
        is_valid = (value > equation_interval_start) && (value < equation_interval_end);
    }
    else if( equation_interval_start_including && !equation_interval_end_including)
    {
        is_valid = (value >= equation_interval_start) && (value < equation_interval_end);
    }
    else if( !equation_interval_start_including && equation_interval_end_including)
    {
        is_valid = (value > equation_interval_start) && (value <= equation_interval_end);
    }
    else if( equation_interval_start_including && equation_interval_end_including)
    {
        is_valid = (value >= equation_interval_start) && (value <= equation_interval_end);
    }

    
   // std::cout << equation->to_string() << " : value: " << value << "  : " << is_valid << "\n\n";


    return is_valid;
}


bool is_zero_inside_of_distance_equation_interval(const btlf::DistanceEquation &distance_equation)
{
    bool zero = distance_equation
                                        .get_m_interval()
                                        .get_m_start_interval()
                                        .isZero();

    bool is_including = distance_equation
                                        .get_m_interval()
                                        .get_m_start_including();

                                        
    return (zero && is_including);
}



std::shared_ptr<btlf::DistanceEquation> find_initial_distance_equation(const std::vector<std::shared_ptr<btlf::DistanceEquation>> &distance_equation_list)
{   

    std::shared_ptr<btlf::DistanceEquation> initial_distance_equation = nullptr;

    auto value = Kernel::FT(std::numeric_limits<double>::max());

    for(auto &distance_equation : distance_equation_list)
    {
        if( is_zero_inside_of_distance_equation_interval(*distance_equation) )
        {
            auto distance_equation_value = distance_equation->calculate_value(0.0, true);

            if(distance_equation_value < value)
            {
                value = distance_equation_value;
                initial_distance_equation = distance_equation;
            }
            else if (distance_equation_value == value)
            {   
                auto inital_end = initial_distance_equation->get_m_interval().get_m_end_interval();
                auto current_end = distance_equation->get_m_interval().get_m_end_interval();

                auto cmp_value = std::min(inital_end, current_end) / 10000.0;

                auto loop_distance_equation_value = distance_equation->calculate_value(cmp_value);
                auto current_distance_equation_value = initial_distance_equation->calculate_value(cmp_value);

                if(loop_distance_equation_value < current_distance_equation_value)
                {
                    value = distance_equation_value;
                    initial_distance_equation = distance_equation;   
                }


            }

        }
    }


    return initial_distance_equation;
}



const std::vector<std::shared_ptr<btlf::DistanceEquation>> build_current_list(const std::vector<std::shared_ptr<btlf::DistanceEquation>> &distance_equation_list, std::shared_ptr<btlf::DistanceEquation> &input_eq)
{
    std::vector<std::shared_ptr<btlf::DistanceEquation>> equation_list_without_input_eq;

    for(auto equation : distance_equation_list)
    {
        if(equation->get_m_id() != input_eq->get_m_id()){
            equation_list_without_input_eq.push_back(equation);
        }         
    }

    return equation_list_without_input_eq;
}



bool check_validity(const Kernel::FT &current_t_position, const btlf::HelpInfo &help_info)
{
    bool valid = false;

    auto t_position = current_t_position;
    auto t_fix = help_info.get_m_fixed_position();
    auto t_next = help_info.get_m_next_point();
    auto first = help_info.get_m_first_intersection();
    auto fix_eq_start = help_info.get_m_fixed_equation()->get_m_interval().get_m_start_interval();
    auto fix_eq_end = help_info.get_m_fixed_equation()->get_m_interval().get_m_end_interval();
    auto eq_start = help_info.get_m_loop_equation()->get_m_interval().get_m_start_interval();
    auto eq_end = help_info.get_m_loop_equation()->get_m_interval().get_m_end_interval();


    if( (t_position < 0.0) || (t_position >= 1) )
    {
        valid = false;
    }
    else
    {
        if( (t_position > t_fix) && ( first || ( t_position < t_next) ) )
        {

            if( (eq_end <= t_position) || (fix_eq_end < t_position) || (eq_start > t_position) )
            {
                valid = false;
            }

            if( (eq_end >= t_position) && (eq_start <= t_position) && (fix_eq_end >= t_position) && (fix_eq_start < t_position) )
            {
                valid = true;
            }

        }
        else
        {
            valid = false;
        }
    }


    return valid;
}


Kernel::FT calculate_compare_point(const Kernel::FT &current_intersec_point, const btlf::HelpInfo &eq_help_info)
{
    auto eq_end = eq_help_info.get_m_loop_equation()->get_m_interval().get_m_end_interval();
    auto fix_eq_end = eq_help_info.get_m_fixed_equation()->get_m_interval().get_m_end_interval();

    auto cmp_value = Kernel::FT{0.0};
    auto min_end = std::min(eq_end, fix_eq_end);
    auto max_end = std::max(eq_end, fix_eq_end);

    if(!(min_end == current_intersec_point))
    {
        cmp_value = ( (min_end - current_intersec_point ) / 100.0 ) + current_intersec_point;
    }
    else
    {
        cmp_value = ( (max_end - current_intersec_point ) / 100.0 ) + current_intersec_point;
    }

    return cmp_value;
}






btlf::EndIntervalInfo determine_end_interval_info(const Kernel::FT &intersec_point, btlf::HelpInfo &eq_help_info)
{   

    auto eq_loop = eq_help_info.get_m_loop_equation();
    auto eq_fix = eq_help_info.get_m_fixed_equation();

    auto cmp_point = calculate_compare_point(intersec_point, eq_help_info);


    // std::cout << "\n\n determine_end_interval_info: \n"
    //         << eq_loop->to_string()
    //         << "\n"
    //         << eq_fix->to_string()
    //         << "\n"
    //         << cmp_point << "\n\n\n";


   // std::cout << "cmp_point: " << cmp_point << "\n\n";


    if( is_value_inside_equation_interval(cmp_point, eq_fix) && is_value_inside_equation_interval(cmp_point, eq_loop) )
    {   
        //std::cout << "\n\nCase 1 \n\n";


        auto eq_loop_value = eq_loop->calculate_value(cmp_point);
        auto eq_fix_value = eq_fix->calculate_value(cmp_point);

        if(eq_loop_value < eq_fix_value)
        {
            auto end_interval_info = btlf::EndIntervalInfo(intersec_point, eq_loop);
            eq_help_info.set_m_info(end_interval_info);
            eq_help_info.set_m_first_intersection(false);
            eq_help_info.set_m_next_point(intersec_point);

            return end_interval_info;
        }
    }
    else if( is_value_inside_equation_interval(cmp_point, eq_loop) && !is_value_inside_equation_interval(cmp_point, eq_fix) )
    {   
        //std::cout << "\n\nCase 2 \n\n";


        auto end_interval_info = btlf::EndIntervalInfo(intersec_point, eq_loop);
        eq_help_info.set_m_info(end_interval_info);

        eq_help_info.set_m_first_intersection(false);
        eq_help_info.set_m_next_point(intersec_point);

        return end_interval_info;
    }

    //std::cout << "\n\nNo Case!!! \n\n";
    

    return btlf::EndIntervalInfo(eq_help_info.get_m_info().get_m_end_interval_floating_point(), eq_help_info.get_m_info().get_m_next_equation());
}




btlf::MrealUnit find_next_mreal_unit(std::vector<std::shared_ptr<btlf::DistanceEquation>> &current_list, std::shared_ptr<btlf::DistanceEquation> &current_equation, Kernel::FT &curr_point, btlf::Mreal &mreal)
{  
    /*
        Fetching date information of the mreal.
    */
    auto start_date_mreal = mreal.get_m_start_date();
    auto end_date_mreal = mreal.get_m_end_date();
    

    auto mreal_unit = btlf::MrealUnit(current_equation, curr_point);
    auto end_interval_info = btlf::EndIntervalInfo{};
    auto help_info = btlf::HelpInfo(current_equation, curr_point, nullptr, -1.0, true, end_interval_info);

    // int counter = 0;

    // const auto start_outside = ClockType::now();

    for(auto loop_equation : current_list)
    {   

    //    const auto start = ClockType::now();
        auto intersec_points = current_equation->intersect(*loop_equation);
    //    const auto end = ClockType::now();

        // counter++;

    //    const auto elapsed_time = std::chrono::duration_cast<ClockRes>(end - start).count();

    //    std::cout << "\n\nZeit Schnittpunkt Berechnung: " << elapsed_time << "ms\n\n";

        help_info.set_m_loop_equation(loop_equation);


        // std::cout << current_equation->to_string() << "\n";
        // std::cout << loop_equation->to_string() << "\n";


        for(auto &intersec_point : intersec_points)
        {   

            auto is_valid = check_validity(intersec_point, help_info);

            // std::cout << intersec_point << std::boolalpha << " : " << is_valid << "\n\n";

            // hier mocktest ??
        
            // std::cout << current_equation->to_string() << "\n";
            // std::cout << loop_equation->to_string() << "\n";

            // const auto b_start = ClockType::now();
            // bool mock = (loop_equation->calculate_value(intersec_point) == current_equation->calculate_value(intersec_point));
            // const auto b_end = ClockType::now();

            // const auto elapsed_time = std::chrono::duration_cast<ClockRes>(b_end - b_start).count();

            // std::cout << "\n\nZeit mock: " << elapsed_time << "ms\n\n";


            if(is_valid)
            {   

                auto info = determine_end_interval_info(intersec_point, help_info);

                mreal_unit.set_m_next_distance_equation(info.get_m_next_equation());
                mreal_unit.set_m_end_floating_point(info.get_m_end_interval_floating_point());
            }
        }

    }

    // const auto end_outside = ClockType::now();

    // const auto elapsed_time = std::chrono::duration_cast<ClockRes>(end_outside - start_outside).count();

    // std::cout << "\n\n# Gleichungen: " << current_list.size() << "  Zeit: " << elapsed_time << "ms\n\n";
   
    // std::cout << "\n\nCounter: " << counter << "\n\n";


    auto start_point = mreal_unit.get_start_floating_point();
    auto end_point = mreal_unit.get_end_floating_point();

    mreal_unit.set_m_start_date_including(is_value_inside_equation_interval(start_point, current_equation));
    mreal_unit.set_m_end_date_including(is_value_inside_equation_interval(end_point, current_equation));

    mreal_unit.set_m_start_date_str(linear_interpolation_timestamps(start_date_mreal, end_date_mreal, start_point.doubleValue()));
    mreal_unit.set_m_end_date_str(linear_interpolation_timestamps(start_date_mreal, end_date_mreal, end_point.doubleValue()));


    if(help_info.get_m_first_intersection())
    {
        mreal_unit.set_m_end_floating_point(1.0);
        mreal_unit.set_m_end_date_including(is_value_inside_equation_interval(1.0, current_equation));
        mreal_unit.set_m_end_date_str(linear_interpolation_timestamps(start_date_mreal, end_date_mreal, 1.0));
        mreal_unit.set_m_next_distance_equation(nullptr);
    }


    return mreal_unit;
}




}