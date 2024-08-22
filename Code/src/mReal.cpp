#include "mReal.hpp"



btlf::HelpInfo::HelpInfo(const std::shared_ptr<DistanceEquation> &fixed_equation, const Kernel::FT &fixed_position, const std::shared_ptr<DistanceEquation> &loop_equation, const Kernel::FT &next_point, const bool first_intersection, EndIntervalInfo &info) :
    m_fixed_equation(fixed_equation),
    m_fixed_position(fixed_position),
    m_loop_equation(loop_equation),
    m_next_point(next_point),
    m_first_intersection(first_intersection),
    m_info(info)
{

}


std::shared_ptr<btlf::DistanceEquation> btlf::HelpInfo::get_m_fixed_equation() const
{
    return m_fixed_equation;
}


Kernel::FT btlf::HelpInfo::get_m_fixed_position() const
{
    return m_fixed_position;
}

std::shared_ptr<btlf::DistanceEquation> btlf::HelpInfo::get_m_loop_equation() const
{
    return m_loop_equation;
}

Kernel::FT btlf::HelpInfo::get_m_next_point() const
{
    return m_next_point;
}


bool btlf::HelpInfo::get_m_first_intersection() const
{
    return m_first_intersection;
}



void btlf::HelpInfo::set_m_loop_equation(const std::shared_ptr<DistanceEquation> &loop_equation)
{
    m_loop_equation = loop_equation;
}


void btlf::HelpInfo::set_m_first_intersection(const bool bool_value)
{
    m_first_intersection = bool_value;
}

void btlf::HelpInfo::set_m_next_point(const Kernel::FT &next_point)
{
    m_next_point = next_point;
}




btlf::EndIntervalInfo btlf::HelpInfo::get_m_info() const
{
    return m_info;
}

   
void btlf::HelpInfo::set_m_info(EndIntervalInfo &info)
{
    m_info = info;
}






btlf::EndIntervalInfo::EndIntervalInfo(const Kernel::FT &end_interval_floating_point, const std::shared_ptr<DistanceEquation> &next_equation) :
    m_end_interval_floating_point(end_interval_floating_point),
    m_next_equation(next_equation)
{

}



 const Kernel::FT btlf::EndIntervalInfo::get_m_end_interval_floating_point() const
 {
    return m_end_interval_floating_point;
 }


const std::shared_ptr<btlf::DistanceEquation> btlf::EndIntervalInfo::get_m_next_equation() const
{
    return m_next_equation;
}






btlf::MrealUnit::MrealUnit(std::shared_ptr<btlf::DistanceEquation> &fixed_equation, const Kernel::FT &start_floating_point) :
    m_start_floating_point(start_floating_point),
    m_end_floating_point(-1.0),
    m_start_date_including(nullptr),
    m_end_date_including(nullptr),
    m_distance_equation(fixed_equation),
    m_next_distance_equation(nullptr),
    m_pull_root(fixed_equation->get_m_distance_equation_type() == DistanceEquationType::QUADRATIC ? true : false),
    m_inverse_result(false),
    m_start_date_str(""),
    m_end_date_str("")
{

}



btlf::MrealUnit::MrealUnit(const MrealUnit &other):
    m_start_floating_point(other.m_start_floating_point),
    m_end_floating_point(other.m_end_floating_point),
    m_start_date_including(other.m_start_date_including),
    m_end_date_including(other.m_end_date_including),
    m_distance_equation(other.m_distance_equation),
    m_next_distance_equation(other.m_next_distance_equation),
    m_pull_root(other.m_pull_root),
    m_inverse_result(other.m_inverse_result),
    m_start_date_str(other.m_start_date_str),
    m_end_date_str(other.m_end_date_str)
{

}




/*
    Konstruktor für die Zero Unit
*/
btlf::MrealUnit::MrealUnit(const Kernel::FT &start, const Kernel::FT &end, const std::string &start_date, const std::string &end_date):
    m_start_floating_point(start),
    m_end_floating_point(end),
    m_start_date_including(true),
    m_end_date_including(true),
    m_distance_equation(std::make_shared<btlf::QuadraticDistanceEquation>()),
    m_pull_root(true),
    m_inverse_result(false),
    m_start_date_str(start_date),
    m_end_date_str(end_date)
{

}







void btlf::MrealUnit::set_m_start_date_str(const std::string &start_date_str)
{
    m_start_date_str = start_date_str;
}

void btlf::MrealUnit::set_m_end_date_str(const std::string &end_date_str)
{
    m_end_date_str = end_date_str;
}

std::string btlf::MrealUnit::get_m_start_date_str() const
{
    return m_start_date_str;
}

std::string btlf::MrealUnit::get_m_end_date_str() const
{
    return m_end_date_str;
}







void btlf::MrealUnit::set_m_end_date_including(const bool is_inlcuding)
{
    m_end_date_including = is_inlcuding;
}


void btlf::MrealUnit::set_m_start_date_including(const bool is_inlcuding)
{
    m_start_date_including = is_inlcuding;
}



void btlf::MrealUnit::set_m_start_floating_point(const Kernel::FT &start_floating_point)
{
    m_start_floating_point = start_floating_point;
}



void btlf::MrealUnit::set_m_end_floating_point(const Kernel::FT &end_floating_point)
{
    m_end_floating_point = end_floating_point;
}


void btlf::MrealUnit::set_m_next_distance_equation(const std::shared_ptr<btlf::DistanceEquation> &next_distance_equation)
{
    m_next_distance_equation = next_distance_equation;
}



void btlf::MrealUnit::set_m_distance_equation(const std::shared_ptr<btlf::DistanceEquation> &distance_equation)
{
    m_distance_equation = distance_equation;
}




Kernel::FT btlf::MrealUnit::get_end_floating_point() const
{
    return m_end_floating_point;
}


Kernel::FT btlf::MrealUnit::get_start_floating_point() const
{
    return m_start_floating_point;
}


std::shared_ptr<btlf::DistanceEquation> btlf::MrealUnit::get_next_distance_equation() const
{
    return m_next_distance_equation;
}



std::shared_ptr<btlf::DistanceEquation> btlf::MrealUnit::get_distance_equation() const
{
    return m_distance_equation;
}



bool btlf::MrealUnit::get_m_start_date_including() const
{
    return m_start_date_including;
}


bool btlf::MrealUnit::get_m_end_date_including() const
{
    return m_end_date_including;
}


bool btlf::MrealUnit::get_m_pull_root() const
{
    return m_pull_root;
}



std::string btlf::MrealUnit::to_string() const
{
    std::ostringstream mreal_unit;

    std::string next_equation_str = (this->get_next_distance_equation() == nullptr) ? " nulllptr " : this->get_next_distance_equation()->to_string();
    std::string start_date_str = (this->m_start_date_str.empty()) ? "INVALID\n" : this->m_start_date_str;
    std::string end_date_str = (this->m_end_date_str.empty()) ? "INVALID\n" : this->m_end_date_str;


    mreal_unit
                << this->m_distance_equation->to_string()
                << "\nstart: "
                << start_date_str
                << "start t: "
                << this->m_start_floating_point
                << "\n"
                << "end: "
                << end_date_str
                << "end t: "
                << this->m_end_floating_point
                << "\n"
                << "next equation: "
                << next_equation_str;


    return mreal_unit.str();

}



void btlf::MrealUnit::inverse()
{   
    // std::cout << this->get_distance_equation()->to_string() << "\n\n";
    std::static_pointer_cast<btlf::LinearDistanceEquation>(this->get_distance_equation())->inverse();
    // std::cout << this->get_distance_equation()->to_string() << "\n\n";
}



/*
    This function checks if a value within the unit interval.
*/
bool btlf::MrealUnit::value_is_inside_interval(const Kernel::FT &value)
{
    bool inside_check = (value > m_start_floating_point) && (value < m_end_floating_point);

    return inside_check;
}


/*
    Model extension
*/

void btlf::MrealUnit::set_m_inverse_result(const bool inverse_result)
{
    m_inverse_result = inverse_result;
}



bool btlf::MrealUnit::get_m_inverse_result() const
{
    return m_inverse_result;
}


void btlf::MrealUnit::set_m_pull_root(const bool pull_root)
{
    m_pull_root = pull_root;
}



/*
    ########################################################################################################
    ########################################################################################################
*/


btlf::Mreal::Mreal(const std::string &start_date, const std::string &end_date):
    m_start_date(start_date),
    m_end_date(end_date),
    m_mreal_units(std::vector<btlf::MrealUnit>())
{

}



void btlf::Mreal::add_mreal_unit(const btlf::MrealUnit &mreal_unit)
{
    m_mreal_units.push_back(mreal_unit);
}



std::vector<btlf::MrealUnit> btlf::Mreal::get_mreal_units() const
{
    return m_mreal_units;
}



void btlf::Mreal::set_m_start_date(const std::string &start_date)
{
    m_start_date = start_date;
}



void btlf::Mreal::set_m_end_date(const std::string &end_date)
{
    m_end_date = end_date;
}



std::string btlf::Mreal::get_m_start_date() const
{
    return m_start_date;
}



std::string btlf::Mreal::get_m_end_date() const
{
    return m_end_date;
}




bool btlf::Mreal::do_correction_original(const MovingPoint &mp, Mregion &mregion)
{   

    // this->m_mreal_units[3].inverse();


    auto was_corrected = bool{false};
    auto current_mp_start_position = Kernel::FT{0};
    auto is_inside = bool{false};
    auto pull_sqrt = bool{false};

    for(auto &unit : this->m_mreal_units)
    {
        pull_sqrt = unit.get_m_pull_root();
        current_mp_start_position = unit.get_start_floating_point();

        if(!pull_sqrt)
        {
            is_inside = mregion.is_point_inside(mp, current_mp_start_position);
            auto equation_value = unit.get_distance_equation()->calculate_value(current_mp_start_position, false); 

            // std::cout << "\n\n" << current_mp_start_position << "\n" << unit.get_distance_equation()->to_string() << "\n" << equation_value << ", " << std::boolalpha << is_inside << "\n\n";

            if( (is_inside && equation_value >= 0.0) || (!is_inside && equation_value < 0.0))
            {
                unit.inverse();
                was_corrected = true;
            }


        }

        /*
            Schnittpunkte müssen noch beachtet werden!
        */

        else if(pull_sqrt)
        {
            is_inside = mregion.is_point_inside(mp, current_mp_start_position);
            auto equation_value = unit.get_distance_equation()->calculate_value(current_mp_start_position, false);

            if(is_inside)
            {   
                // Model extension
                unit.set_m_inverse_result(true);

                was_corrected = true;
            }

        }

    }


    return was_corrected;
}

/*
    Start new correction
*/


btlf::Mreal btlf::Mreal::do_new_correction_original(const MovingPoint &mp, Mregion &mregion)
{   
    auto corrected_mreal = btlf::Mreal{};
    auto test_start_equation_value = Kernel::FT{999999999.99};

    for(auto &unit : m_mreal_units)
    {   
        auto zero_point_vaild = bool{false};
        auto pull_sqrt = bool{unit.get_m_pull_root()};
        auto is_start_equation_value_inside_mregion = bool{false};
        auto current_unit_start_position = unit.get_start_floating_point();
        auto current_unit_end_position = unit.get_end_floating_point();
        auto current_distance_equation = unit.get_distance_equation();

        // Test gibt eine gültige Nullstelle die im gültigen Intervall liegt?

        auto zeros = current_distance_equation->calculate_zeros();

        if(!zeros.empty())
        {   
            auto zero_point = zeros[0].m_point;

            if(unit.value_is_inside_interval(zero_point))
            {   
                zero_point_vaild = true;
                test_start_equation_value = (current_unit_start_position + zero_point) / 2.0;
            }
            else
            {
                zero_point_vaild = false;
                test_start_equation_value = (current_unit_start_position + current_unit_end_position) / 2.0;
            }
        }

        else
        {   
            zero_point_vaild = false;
            test_start_equation_value = (current_unit_start_position + current_unit_end_position) / 2.0;
        }


        is_start_equation_value_inside_mregion = mregion.is_point_inside(mp, test_start_equation_value);
        auto eqValue = current_distance_equation->calculate_value(test_start_equation_value, false);

        /*
            Linear equation case 1: The movingpoint is located outside the mregion.
        */
        if( (!pull_sqrt && !is_start_equation_value_inside_mregion && (eqValue < 0.0)) 
            || (!pull_sqrt && is_start_equation_value_inside_mregion && (eqValue > 0.0)) )
        {
            unit.inverse();
        }

        /*
            Linear equation case 2: The movingpoint is located inside the mregion.
        */

        /*
            Quadtratic equation case 1: The movingpoint is located outside the mregion, and there is no intersection point. else if(pull_sqrt && !zero_point_vaild && !is_start_equation_value_inside_mregion)
        */


        /*
            Quadtratic equation case : The movingpoint is located inside the mregion, and there is no intersection point.
        */
        else if(pull_sqrt && !zero_point_vaild && is_start_equation_value_inside_mregion)
        {   
            std::cout << "Quadtratic equation case 2\n";
            std::cout << unit.to_string() << "\n\n\n";

            unit.set_m_inverse_result(true);
        }

        corrected_mreal.add_mreal_unit(unit);
    }

    return corrected_mreal;
}



/*
    End new correction
*/


void btlf::Mreal::do_correction_date_limits()
{   

    for(std::size_t i = 0; auto &unit : m_mreal_units)
    {   
        if(i == 0)
        {
            unit.set_m_start_date_including(true);
        }

        if(i == m_mreal_units.size()-1)
        {
            unit.set_m_end_date_including(true);
        }


        auto current_end = bool{unit.get_m_end_date_including()};
        auto next_start = bool{false};

        if(i < m_mreal_units.size()-1)
        {
            next_start = bool{m_mreal_units[i+1].get_m_start_date_including()};
        }
    
        if(current_end && next_start)
        {
            m_mreal_units[i+1].set_m_start_date_including(false);
        }

        ++i;
    }


}



btlf::Mreal btlf::Mreal::do_zero_part_correction(const MovingPoint &mp, Mregion &mregion)
{
    std::cout << "(Start<) Inside do zero part correction \n";

    auto zero_mreal = btlf::Mreal();

    /*
        A zero equation is being created.
    */
    std::shared_ptr<btlf::DistanceEquation> zero_eq = std::make_shared<btlf::QuadraticDistanceEquation>();


    for(auto &unit : this->m_mreal_units)
    {   
        auto unit_start_position = unit.get_start_floating_point();
        auto unit_end_postion = unit.get_end_floating_point();

        auto is_start_inside_mregion = mregion.is_point_inside(mp, unit_start_position);
        auto is_after_zero_point_inside_mregion = bool{false};
        auto zero_point = Kernel::FT{btlf::INVALID_ZERO_POINT};

        /*
            The zeros are being calculated.
        */
        auto zeros = unit.get_distance_equation()->calculate_zeros();

        if(!zeros.empty())
        {   
            zero_point = zeros[0].m_point;

            if(unit.value_is_inside_interval(zero_point))
            {
                auto after_zero_point = (zero_point + unit_end_postion) / 2.0;
                is_after_zero_point_inside_mregion = mregion.is_point_inside(mp, after_zero_point);
            }

        }

        /*
            Case 1: Transition form outside the mregion to inside the mregion.
        */
        if(!zeros.empty() && !is_start_inside_mregion && is_after_zero_point_inside_mregion)
        {   
            auto positive_equation = unit.get_distance_equation();

            auto positive_unit = btlf::MrealUnit{positive_equation, unit_start_position};

            auto start_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), unit_start_position.doubleValue());
            auto zero_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), zero_point.doubleValue());
            auto end_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), unit_end_postion.doubleValue());

            positive_unit.set_m_start_date_str(start_date);
            positive_unit.set_m_end_floating_point(zero_point);
            positive_unit.set_m_end_date_str(zero_date);

            // Set next equation
            positive_unit.set_m_next_distance_equation(zero_eq);


            zero_mreal.m_mreal_units.push_back(positive_unit);


            auto zero_unit = btlf::MrealUnit{zero_point, unit_end_postion, zero_date, end_date};


            zero_mreal.m_mreal_units.push_back(zero_unit);


        }
        

        /*
            Case 2: The point stays inside the mregion the whole time.
        */
        else if(!unit.value_is_inside_interval(zero_point) && mregion.is_point_inside(mp, unit_start_position))
        {
            auto zero_unit = btlf::MrealUnit{unit};
            zero_unit.set_m_distance_equation(zero_eq);

            zero_mreal.m_mreal_units.push_back(zero_unit);
        }


        /*
            Case 3: The point stays outside the mregion the whole time.
        */
        else if(!unit.value_is_inside_interval(zero_point) && !mregion.is_point_inside(mp, unit_start_position))
        {
            zero_mreal.m_mreal_units.push_back(unit);
        }


        /*
            Case 4: Transiton from inside the mregion to outside the mregion.
        */
        else if(!zeros.empty() && is_start_inside_mregion && !is_after_zero_point_inside_mregion)
        {
            auto start_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), unit_start_position.doubleValue());
            auto zero_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), zero_point.doubleValue());
            auto end_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), unit_end_postion.doubleValue());

            auto positive_equation = unit.get_distance_equation();


            auto zero_unit = btlf::MrealUnit{unit_start_position, zero_point, start_date, zero_date};
            zero_unit.set_m_next_distance_equation(positive_equation);
            
            zero_mreal.m_mreal_units.push_back(zero_unit);


            auto positive_unit = btlf::MrealUnit{positive_equation, zero_point};

            positive_unit.set_m_start_date_str(zero_date);
            positive_unit.set_m_end_floating_point(unit_end_postion);
            positive_unit.set_m_end_date_str(end_date);


            zero_mreal.m_mreal_units.push_back(positive_unit);

        }


    }

    auto summarized_zero_parts = summarize_zero_parts(zero_mreal);

    set_correct_next_equations(summarized_zero_parts);

    return summarized_zero_parts;
}



btlf::Mreal btlf::Mreal::summarize_zero_parts(Mreal &to_summarize)
{
    auto summarized = btlf::Mreal{};

    std::size_t unit_counter = 0;

    for(std::size_t i = 0; auto &unit : to_summarize.m_mreal_units)
    {   
        auto unit_equation_type = unit.get_distance_equation()->get_m_distance_equation_type();

        if( i == 0 )
        {
            summarized.add_mreal_unit(unit);
            ++unit_counter;
        }

        else if( (i > 0) && (unit_equation_type == btlf::DistanceEquationType::ZERO) )
        {
            auto previous_equation_type = to_summarize.m_mreal_units[i-1].get_distance_equation()->get_m_distance_equation_type();

            if(previous_equation_type != btlf::DistanceEquationType::ZERO)
            {
                summarized.add_mreal_unit(unit);
                ++unit_counter;
            }
            else if(previous_equation_type == btlf::DistanceEquationType::ZERO)
            {
                auto end_t = unit.get_end_floating_point();
                auto end_date = unit.get_m_end_date_str();

                summarized.m_mreal_units[unit_counter-1].set_m_end_date_str(end_date);
                summarized.m_mreal_units[unit_counter-1].set_m_end_floating_point(end_t);
            }
            
        }

        else if( (i > 0) && (unit_equation_type != btlf::DistanceEquationType::ZERO) )
        {
            summarized.add_mreal_unit(unit);
            ++unit_counter;
        }


        ++i;
    }



    return summarized;
}


/*
    Erstmal nur zum probiern, eventuell schreib ich eine eigene Funktion um den Zero mreal zu korrigieren!
*/
void btlf::Mreal::set_correct_next_equations(Mreal &to_correct)
{   
    std::size_t length = to_correct.m_mreal_units.size();

    for(std::size_t i = 0; i < to_correct.m_mreal_units.size(); ++i)
    {
        if(i == to_correct.m_mreal_units.size() - 1)
        {
            to_correct.m_mreal_units[i].set_m_next_distance_equation(nullptr);
        }
        else
        {   auto next_equation = to_correct.m_mreal_units[i+1].get_distance_equation();

            to_correct.m_mreal_units[i].set_m_next_distance_equation(next_equation);
        }
    }

}






/*
    Erste Prototype, die Methode ist bestimmt noch nicht fertig.
*/
btlf::Mreal btlf::Mreal::do_taylor_approximation(const MovingPoint &mp, Mregion &mregion)
{
    auto mreal_taylor = btlf::Mreal{};

    auto current_mp_start_position = Kernel::FT{0};
    auto current_mp_end_position = Kernel::FT{0};
    auto is_inside = bool{false};
    auto pull_sqrt = bool{false};
    auto has_zeros = bool{false};
    auto zero = Kernel::FT{btlf::INVALID_ZERO_POINT};


    for(auto &unit : this->m_mreal_units)
    {
        pull_sqrt = unit.get_m_pull_root();
        current_mp_start_position = unit.get_start_floating_point();
        current_mp_end_position = unit.get_end_floating_point();

        /*
            The zeros are being calculated.
        */
        auto zeros = unit.get_distance_equation()->calculate_zeros();

        if(!zeros.empty())
        {
            has_zeros = true;
            zero = zeros[0].m_point;
        }

        is_inside = mregion.is_point_inside(mp, current_mp_start_position);

        if(!pull_sqrt)
        {
            auto equation_value = unit.get_distance_equation()->calculate_value(current_mp_start_position, false); 

            // std::cout << "\n\n" << current_mp_start_position << "\n" << unit.get_distance_equation()->to_string() << "\n" << equation_value << ", " << std::boolalpha << is_inside << "\n\n";

            if( (is_inside && equation_value >= 0.0) || (!is_inside && equation_value < 0.0))
            {
                unit.inverse();
            }

            mreal_taylor.add_mreal_unit(unit);

        }

        else
        {

            auto step = Kernel::FT{0.001};

            auto x0 = Kernel::FT{current_mp_start_position};

            /*
                Case 1: It has to build inverse taylor polynoms.
            */
            if(is_inside && !has_zeros)
            {
                
                // auto taylor_end = Kernel::FT{x0 + step};

                while(x0 < (current_mp_end_position - step))
                {

                    auto start_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), x0.doubleValue());
                    auto end_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), (x0 + step).doubleValue());


                    auto taylor_polynom = btlf::TaylorPolynomGrade2{unit.get_distance_equation(), x0};

                    taylor_polynom.inverse();

                    auto quadratic_prefactor = taylor_polynom.get_m_quadratic_prefactor();
                    auto linear_prefactor = taylor_polynom.get_m_linear_prefactor();
                    auto const_value = taylor_polynom.get_m_constant_value();

                    std::shared_ptr<btlf::DistanceEquation> new_equation = std::make_shared<btlf::QuadraticDistanceEquation>(quadratic_prefactor, linear_prefactor, const_value);

                    auto new_unit = btlf::MrealUnit{new_equation, x0};
                    new_unit.set_m_start_floating_point(x0);
                    new_unit.set_m_end_floating_point(x0 + step);
                    new_unit.set_m_start_date_str(start_date);
                    new_unit.set_m_end_date_str(end_date);
                    new_unit.set_m_pull_root(false);
                    new_unit.set_m_end_date_including(true);

                    mreal_taylor.add_mreal_unit(new_unit);

                    x0 += step;
                }

          
                auto start_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), x0.doubleValue());
                auto end_date = linear_interpolation_timestamps(mregion.get_m_start_date(), mregion.get_m_end_date(), current_mp_end_position.doubleValue());

                auto taylor_polynom = btlf::TaylorPolynomGrade2{unit.get_distance_equation(), x0};

                taylor_polynom.inverse();

                auto quadratic_prefactor = taylor_polynom.get_m_quadratic_prefactor();
                auto linear_prefactor = taylor_polynom.get_m_linear_prefactor();
                auto const_value = taylor_polynom.get_m_constant_value();

                std::shared_ptr<btlf::DistanceEquation> new_equation = std::make_shared<btlf::QuadraticDistanceEquation>(quadratic_prefactor, linear_prefactor, const_value);

                auto new_unit = btlf::MrealUnit{new_equation, x0};
                new_unit.set_m_start_floating_point(x0);
                new_unit.set_m_end_floating_point(current_mp_end_position);
                new_unit.set_m_start_date_str(start_date);
                new_unit.set_m_end_date_str(end_date);
                new_unit.set_m_pull_root(false);
                new_unit.set_m_end_date_including(true);


                mreal_taylor.add_mreal_unit(new_unit); 

            }
            else 
            {
                mreal_taylor.add_mreal_unit(unit);
            }



        }


        has_zeros = false;
    }



    return mreal_taylor;
}

        







