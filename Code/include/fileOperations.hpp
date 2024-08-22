#pragma once

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <array>

#include "mRegion.hpp"
#include "mReal.hpp"
#include "movingPoint.hpp"
#include "buildMreal.hpp"
#include "constants.hpp"
#include "RList.hpp"


namespace btlf
{


Mregion read_moving_region(const std::string_view argument)
{
    Mregion mregion;

    std::string path_to_file = std::string(btlf::PATH_TO_INPUT_DIRECTORY) + std::string(argument);

    std::cout << path_to_file << "\n\n";

    RList rlist = RList::fromFile(path_to_file);


    auto date_rlist = rlist.items[4].items[0].items[0];

    std::cout << date_rlist.ToString() << "\n\n";
   
   /*
        The time interval is set.
   */
    mregion.set_m_start_date(date_rlist.items[0].getString());
    mregion.set_m_end_date(date_rlist.items[1].getString());
    mregion.set_m_start_date_including(date_rlist.items[2].getBool());
    mregion.set_m_end_date_including(date_rlist.items[3].getBool());


    auto face_date_rlist = rlist.items[4].items[0].items[1];
    std::size_t size_faces = rlist.items[4].items[0].items[1].size();

    std::cout << "\n\n";
    std::cout << "Size: " << size_faces << "\n\n\n";

    auto numeration = long{0};

    

    for(auto face_data : face_date_rlist.items)
    {   

        auto face_in_process = btlf::MregionFace{};

        for(std::size_t i = 0; auto face : face_data.items)
        {   
            /*
                Das Face aufbauen!
            */
           
            if(i == 0)
            {
                for(auto row : face.items)
                {
                    std::cout << "Face: " << row.ToString() << "\n";
                   
                    auto start_x = Kernel::FT{row.items[0].getNr()};
                    auto start_y = Kernel::FT{row.items[1].getNr()};
                    auto end_x = Kernel::FT{row.items[2].getNr()};
                    auto end_y = Kernel::FT{row.items[3].getNr()};

                    auto moving_point = btlf::MovingPoint{start_x, start_y, end_x, end_y, std::to_string(numeration)};
                    numeration++;

                    face_in_process.add_moving_point(moving_point);
                }
            }

            /*
                Löcher bauen!
            */
            if(i > 0)
            {
                auto hole = btlf::MovingHole{};

                for(auto row : face.items)
                {
                    std::cout << "Loch: " << row.ToString() << "\n";
                    
                    auto start_x = Kernel::FT{row.items[0].getNr()};
                    auto start_y = Kernel::FT{row.items[1].getNr()};
                    auto end_x = Kernel::FT{row.items[2].getNr()};
                    auto end_y = Kernel::FT{row.items[3].getNr()};

                    auto hole_point = btlf::MovingPoint{start_x, start_y, end_x, end_y, std::to_string(numeration)};
                    numeration++;

                    hole.add_moving_point(hole_point);
                }

                face_in_process.add_moving_hole(hole);
            }

            ++i;

            std::cout << "\n\n";
        }

        mregion.add_mregion_face(face_in_process);

        std::cout << "\n\n";
    }

    // std::cout << face_date_rlist.items[1].items[0].ToString() << "\n\n";

    return mregion;
}






btlf::MovingPoint read_moving_point(const std::string argument)
{   
    std::string path_to_file = std::string{btlf::PATH_TO_INPUT_DIRECTORY} + argument;

    std::cout << path_to_file << "\n\n";

    RList rlist = RList::fromFile(path_to_file);
    RList rlist_info = rlist.items[4].items[0].items[1];

    std::array<double, btlf::MPOINT_ARRAY_SIZE> double_values;


    for(std::size_t i = 0; auto rlist : rlist_info.items)
    {
        double_values[i] = rlist.getNr();
        i++;
    }


    return MovingPoint(double_values[0], double_values[1], double_values[2], double_values[3], "M_Point");

}





bool check_arguments(int arg_counter)
{
    bool arguments_are_available = false;

    // std::cout << "Counter: " << arg_counter << "\n";

    if( (arg_counter == 1) || (arg_counter == 2) || (arg_counter >= 4) ){

        std::cerr << "Error: no arguments were given to the program, or the amount of them was wrong!\n";
        std::cerr << "Usage: ./build/main <movingpoint> <movingregion>\n\n";
    }
    else
    {
        arguments_are_available = true;
    }


    return arguments_are_available;
}



void mreal_to_file(const btlf::Mreal &mreal, btlf::MrealVariant variant)
{   
    RList mreal_list;
    RList mreal_units;

    mreal_list.appendsym(std::string{btlf::MREAL_OBJECT});

    for(auto &unit : mreal.get_mreal_units())
    {
        RList unit_info;
        RList date_info;
        RList equation_info;

        auto start_date = unit.get_m_start_date_str();
        auto end_date = unit.get_m_end_date_str();
        auto start_date_including = unit.get_m_start_date_including();
        auto end_date_including = unit.get_m_end_date_including();

        /*
            Deleting the newline at the end of the string.
        */
        start_date.erase(std::remove(start_date.begin(), start_date.end(), '\n'), start_date.end());
        end_date.erase(std::remove(end_date.begin(), end_date.end(), '\n'), end_date.end());

        date_info.append(start_date);
        date_info.append(end_date);
        date_info.append(start_date_including);
        date_info.append(end_date_including);


        unit_info.append(date_info);


        auto equation = unit.get_distance_equation();
        auto quadratic_prefactor = equation->get_m_quadratic_prefactor().doubleValue();
        auto linear_prefactor = equation->get_m_linear_prefactor().doubleValue();
        auto constant_value = equation->get_m_constant_value().doubleValue();
        auto pull_sqrt = unit.get_m_pull_root();

        // Model extension
        auto inverse_result = unit.get_m_inverse_result();

        equation_info.append(quadratic_prefactor);
        equation_info.append(linear_prefactor);
        equation_info.append(constant_value);
        equation_info.append(pull_sqrt);

        // Model extension
        if(variant == btlf::MrealVariant::CHANGED_MODEL)
        {
            equation_info.append(inverse_result);
        }
    

        unit_info.append(equation_info);


        mreal_units.append(unit_info);
    }

    mreal_list.append(mreal_units);


    std::cout << mreal_list.ToString() << "\n\n";


    if(!std::filesystem::exists(btlf::DIRECTORY_OUTPUT))
    {
        std::filesystem::create_directory(btlf::DIRECTORY_OUTPUT);
    }


    std::string data_name = "invalid";


    if(variant == btlf::MrealVariant::CHANGED_MODEL)
    {
        data_name = "./output/mreal_changed_model";
    } 
    else if (variant == btlf::MrealVariant::ZERO_PARTS)
    {
        data_name = "./output/mreal_zero_parts";
    }
    else if (variant == btlf::MrealVariant::APPROXIMATION)
    {
        data_name = "./output/mreal_approximation";
    }


    mreal_list.toFile(data_name);
}


}




