#pragma once

#include "mReal.hpp"


btlf::MrealUnit modify_start_next_linear_equation(std::vector<btlf::MrealUnit> &mreal_units_original, const std::vector<btlf::MrealUnit> &mreal_units_viewer, const std::size_t i)
{   
    std::shared_ptr<btlf::DistanceEquation> distance_equation_curr, distance_equation_next;

    if(i == 0)
    {
        distance_equation_curr = mreal_units_original[i].get_distance_equation(); 
    }
    else
    {
        distance_equation_curr = mreal_units_viewer[i-1].get_next_distance_equation();
    }

    // MrealUnit kopieren und nextEquation neu setzen
    auto mreal_unit_copy = btlf::MrealUnit(mreal_units_original[i]);

    auto new_end_interval_floating_point = mreal_units_original[i].get_end_floating_point() - mreal_units_original[i].get_start_floating_point();

    mreal_unit_copy.set_m_end_floating_point(new_end_interval_floating_point);

    // Start wird immer auf 0.0 gesetzt
    mreal_unit_copy.set_m_start_floating_point(0.0);


    auto end_floating_point_curr = mreal_units_original[i].get_end_floating_point();


    distance_equation_next = mreal_units_original[i].get_next_distance_equation();

    if(distance_equation_next != nullptr)
    {
        // Neue Distanzgleichung erstellen
        auto changed_distance_equation = std::make_shared<btlf::LinearDistanceEquation>(distance_equation_next);

        auto new_constant_value = distance_equation_next->calculate_value(end_floating_point_curr, false);

        // Setze den neuen Konstanten Wert
        changed_distance_equation->set_m_constant_value(new_constant_value);

        
        auto linear_prefactor = changed_distance_equation->get_m_linear_prefactor();

        auto end_floating_point_next_unit = mreal_units_original[i+1].get_end_floating_point();
        auto start_floating_point_next_unit = mreal_units_original[i+1].get_start_floating_point();

        // std::cout << "\n\n\n" << end_floating_point_next_unit << ", " << start_floating_point_next_unit << "\n\n\n";
        
        if(changed_distance_equation->calculate_value(end_floating_point_next_unit - start_floating_point_next_unit) != distance_equation_next->calculate_value(end_floating_point_next_unit))
        {
            changed_distance_equation->set_m_linear_prefactor(-1.0 * linear_prefactor);
        }



        mreal_unit_copy.set_m_next_distance_equation(changed_distance_equation);

    }


    if(i != 0)
    {
        mreal_unit_copy.set_m_distance_equation(mreal_units_viewer[i-1].get_next_distance_equation());
    }



    return mreal_unit_copy;
}


btlf::MrealUnit modify_start_next_quadratic_equation(std::vector<btlf::MrealUnit> &mreal_units_original, const std::vector<btlf::MrealUnit> &mreal_units_viewer, const std::size_t i)
{   
    std::shared_ptr<btlf::DistanceEquation> distance_equation_curr;

    if(i == 0){
        distance_equation_curr = mreal_units_original[i].get_distance_equation();
    }
    else
    {
        distance_equation_curr = mreal_units_viewer[i-1].get_next_distance_equation();
    }

    auto distance_equation_next = mreal_units_original[i].get_next_distance_equation();

    // MrealUnit kopieren und nextEquation neu setzen
    auto mreal_unit_copy = btlf::MrealUnit(mreal_units_original[i]);

    if(distance_equation_next != nullptr)
    {

        auto changed_distance_equation = std::make_shared<btlf::QuadraticDistanceEquation>(distance_equation_next);


        // ScheitelpunktForm berechnen
        auto quadratic_prefactor = changed_distance_equation->get_m_quadratic_prefactor();
        auto linear_prefactor = changed_distance_equation->get_m_linear_prefactor();
        auto constant_value = changed_distance_equation->get_m_constant_value();


        auto apex_x_coordinate = -(linear_prefactor/(2.0 * quadratic_prefactor));
        auto apex_y_coordinate = pow(mreal_units_original[i].get_next_distance_equation()->calculate_value(apex_x_coordinate), 2.0);

        // std::cout << "\n\n" << mreal_units_original[i].get_next_distance_equation()->to_string() << "\n\n";
        // std::cout << "\n\n" << apex_x_coordinate << "\n\n";
        // std::cout << "\n\n" << apex_y_coordinate << "\n\n";

        auto end_floating_point = mreal_units_original[i].get_end_floating_point();

        auto binomial_result = btlf::BinomialFormula(1.0, -apex_x_coordinate + end_floating_point);

        auto new_linear_prefactor = binomial_result.get_m_linear_prefactor() * quadratic_prefactor;
        auto new_constant_value = binomial_result.get_m_constant_value() * quadratic_prefactor + apex_y_coordinate;

        changed_distance_equation->set_m_linear_prefactor(new_linear_prefactor);
        changed_distance_equation->set_m_constant_value(new_constant_value);

        mreal_unit_copy.set_m_next_distance_equation(changed_distance_equation);
    }
            

    if(i != 0)
    {
        mreal_unit_copy.set_m_distance_equation(mreal_units_viewer[i-1].get_next_distance_equation());
    }
    
    auto new_end_interval_floating_point = mreal_units_original[i].get_end_floating_point() - mreal_units_original[i].get_start_floating_point();

    mreal_unit_copy.set_m_end_floating_point(new_end_interval_floating_point);
    mreal_unit_copy.set_m_start_floating_point(0.0);
    
    return mreal_unit_copy;
}


btlf::Mreal modify_mreal_for_viewer(const btlf::Mreal &mreal_original)
{
    auto start_date = mreal_original.get_m_start_date();
    auto end_date = mreal_original.get_m_end_date();

    // Erstelle einen neuen Mreal mit den gleichen Datumswerten
    auto mreal_viewer = btlf::Mreal(start_date, end_date);

    // Alle MrealUnits holen
    auto mreal_units_original = mreal_original.get_mreal_units();
    // auto mreal_units_viewer = mreal_viewer.get_mreal_units();


    for(std::size_t i = 0; i < mreal_units_original.size(); ++i)
    {
        auto unit_curr = mreal_units_original[i];

        if(unit_curr.get_next_distance_equation() == nullptr && unit_curr.get_distance_equation()->get_m_distance_equation_type() == btlf::DistanceEquationType::LINEAR)
        {
            auto mreal_unit_copy = modify_start_next_linear_equation(mreal_units_original, mreal_viewer.get_mreal_units(), i);
            mreal_viewer.add_mreal_unit(mreal_unit_copy);
        }
        else if(unit_curr.get_next_distance_equation() == nullptr && unit_curr.get_distance_equation()->get_m_distance_equation_type() == btlf::DistanceEquationType::QUADRATIC)
        {
            auto mreal_unit_copy = modify_start_next_quadratic_equation(mreal_units_original, mreal_viewer.get_mreal_units(), i);
            mreal_viewer.add_mreal_unit(mreal_unit_copy);  
        }
        else if(unit_curr.get_next_distance_equation()->get_m_distance_equation_type() == btlf::DistanceEquationType::LINEAR && i == 0)
        {
            auto mreal_unit_copy = modify_start_next_linear_equation(mreal_units_original, mreal_viewer.get_mreal_units(), i);
            mreal_viewer.add_mreal_unit(mreal_unit_copy);
        }
        else if(unit_curr.get_next_distance_equation()->get_m_distance_equation_type() == btlf::DistanceEquationType::QUADRATIC && i == 0)
        {
            auto mreal_unit_copy = modify_start_next_quadratic_equation(mreal_units_original, mreal_viewer.get_mreal_units(), i);
            mreal_viewer.add_mreal_unit(mreal_unit_copy);
        }
        else if(unit_curr.get_next_distance_equation()->get_m_distance_equation_type() == btlf::DistanceEquationType::QUADRATIC && i != 0)
        {
            auto mreal_unit_copy = modify_start_next_quadratic_equation(mreal_units_original, mreal_viewer.get_mreal_units(), i);
            mreal_viewer.add_mreal_unit(mreal_unit_copy);
        }
        else if(unit_curr.get_next_distance_equation()->get_m_distance_equation_type() == btlf::DistanceEquationType::LINEAR && i != 0)
        {
            auto mreal_unit_copy = modify_start_next_linear_equation(mreal_units_original, mreal_viewer.get_mreal_units(), i);
            mreal_viewer.add_mreal_unit(mreal_unit_copy);
        }

    }



    return mreal_viewer;
}





