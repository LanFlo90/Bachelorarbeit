#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <cmath>
#include <array>


#include "RList.hpp"


#include "movingPoint.hpp"
#include "mRegion.hpp"
#include "segmentEquation.hpp"
#include "distanceEquation.hpp"
#include "mReal.hpp"
#include "buildMreal.hpp"
#include "viewerMreal.hpp"
#include "fileOperations.hpp"

#include "taylor.hpp"

/*

(mpoint 
    (
        (
            ("2021-01-01" "2021-01-02" TRUE TRUE)
            (54.0 151.0 267.0 -32.0))))


(mregion 
    (
        (
            ("2021-01-01" "2021-01-02" TRUE TRUE)
            (
                (
                    (
                        (100.0 0.0 140.0 -86.0)
                        (100.0 0.0 180.0 10.0)
                        (50.0 86.0 180.0 10.0)
                        (50.0 86.0 100.0 0.0)
                        (0.0 0.0 100.0 0.0)
                        (0.0 0.0 140.0 -86.0)))))))


*/

using ClockType = std::chrono::steady_clock;
using ClockRes = std::chrono::milliseconds;


int main(int argc, char **argv)
{   
    std::cout << "\n\nStart program\n\n"; 

    const auto start = ClockType::now();


    btlf::MovingPoint input_point;
    btlf::Mregion input_mregion;

    auto arguments = bool{btlf::check_arguments(argc)};    

    if(arguments)
    {
        input_point = btlf::read_moving_point(std::string{argv[1]});
        input_mregion = btlf::read_moving_region(argv[2]);
    }
    else
    {
        return 1;
    }

    // std::cout << input_point.to_string() << "\n\n\n";

    // Konkav 712.0 918.0 978.0 373.0 Beispiel 1 (erstellt)
    // auto input_point = btlf::MovingPoint(712.0, 918.0, 978.0, 373.0, "M");

    // Konkav Beispiel 2 (erstellt)
    // auto input_point = btlf::MovingPoint(402.0, 808.0, 914.0, 412.0, "M");

    // Konkav Beispiel 3 (erstellt)
    // auto input_point = btlf::MovingPoint(327.0, 491.0, 940.0, 491.0, "M");

    // Konkav Beipspiel 4 (erstellt)
    // auto input_point = btlf::MovingPoint{333.0, 615.0, 872.0, 615.0, "M"};

    // Konkav Beispiel senkrecht
    // auto input_point = btlf::MovingPoint(601.0, 900.0, 601.0, 368.0, "M");

    // Testpunkt mp1
    // auto input_point = btlf::MovingPoint(54.0, 151.0, 267.0, -32.0, "M");

    // std::cout << point_m.to_string() << "\n\n";


    // Testpunkt durchquert die MovinngRegion
    // auto input_point = btlf::MovingPoint(4.0, 107.0, 186.0, -67.0, "M");

    // Testpunkt mp2
    // auto input_point = btlf::MovingPoint(290.0, -48.0, 117.0, 144.0, "M");


    std::cout << "\n\n" << input_point.to_string() << "\n\n";


// ( OBJECT mregion 
//    ( ) mregion 
//    ( 
//       ( 
//          ( "2024-01-01-00:00:00.000" "2024-01-02-00:00:00.000" TRUE TRUE ) 
//          ( 
//             ( 
//                ( 
//                   ( 308.300395256917 377.470355731225 311.358558952647 384.274086456592 ) k
//                   ( 308.300395256917 377.470355731225 503.003483885079 604.461872549173 ) k
//                   ( 610.671936758893 549.407114624506 503.003483885079 604.461872549173 ) k
//                   ( 610.671936758893 549.407114624506 617.174928525676 657.470043275165 ) k
//                   ( 877.470355731225 355.731225296443 617.174928525676 657.470043275165 ) k
//                   ( 877.470355731225 355.731225296443 876.099454764174 364.905716383634 ) k
//                   ( 877.470355731225 355.731225296443 804.7423018638 689.071068131044 ) k
//                   ( 600.790513833992 871.541501976285 804.7423018638 689.071068131044 ) k
//                   ( 600.790513833992 871.541501976285 605.961661641332 874.599665672015 ) k
//                   ( 600.790513833992 871.541501976285 398.006530331672 740.040463059882 ) 
//                   ( 308.300395256917 377.470355731225 398.006530331672 740.040463059882 ) ) ) ) ) ) )


    // auto mregion = btlf::Mregion("2003-11-20-00:00:00.000", "2003-11-21-00:00:00.000", true, true);


    // auto mregion = btlf::Mregion("2024-01-01-00:00:00.000", "2024-01-02-00:00:00.000", true, true);

    // auto mregion_face_1 = btlf::MregionFace();

    //+ auto point_a1 = btlf::MovingPoint(100.0, 0.0, 140.0, -86.0, "A1");
    //+ auto point_a2 = btlf::MovingPoint(100.0, 0.0, 180.0, 10.0, "A2");
    //+ auto point_b1 = btlf::MovingPoint(50.0, 86.0, 180.0, 10.0, "B1");
    //+ auto point_b2 = btlf::MovingPoint(50.0, 86.0, 100.0, 0.0, "B2");
    //+ auto point_c1 = btlf::MovingPoint(0.0, 0.0, 100.0, 0.0, "C1");
    //+ auto point_c2 = btlf::MovingPoint(0.0, 0.0, 140.0, -86.0, "C2");


    // auto point_a1 = btlf::MovingPoint(308.300395256917, 377.470355731225, 311.358558952647, 384.274086456592, "A1");
    // auto point_a2 = btlf::MovingPoint(308.300395256917, 377.470355731225, 503.003483885079, 604.461872549173, "A2");
    // auto point_b1 = btlf::MovingPoint(610.671936758893, 549.407114624506, 503.003483885079, 604.461872549173, "B1");
    // auto point_b2 = btlf::MovingPoint(610.671936758893, 549.407114624506, 617.174928525676, 657.470043275165, "B2");
    // auto point_c1 = btlf::MovingPoint(877.470355731225, 355.731225296443, 617.174928525676, 657.470043275165, "C1");
    // auto point_c2 = btlf::MovingPoint(877.470355731225, 355.731225296443, 876.099454764174, 364.905716383634, "C2");
    // auto point_c3 = btlf::MovingPoint(877.470355731225, 355.731225296443, 804.7423018638, 689.071068131044, "C3");
    // auto point_d1 = btlf::MovingPoint(600.790513833992, 871.541501976285, 804.7423018638, 689.071068131044, "D1");
    // auto point_d2 = btlf::MovingPoint(600.790513833992, 871.541501976285, 605.961661641332, 874.599665672015, "D2");
    // auto point_d3 = btlf::MovingPoint(600.790513833992, 871.541501976285, 398.006530331672, 740.040463059882, "D3");
    // auto point_a3 = btlf::MovingPoint(308.300395256917, 377.470355731225, 398.006530331672, 740.040463059882, "A3");




    // mregion_face_1.add_moving_point(point_a1);
    // mregion_face_1.add_moving_point(point_a2);
    // mregion_face_1.add_moving_point(point_b1);
    // mregion_face_1.add_moving_point(point_b2);
    // mregion_face_1.add_moving_point(point_c1);
    // mregion_face_1.add_moving_point(point_c2);
    // mregion_face_1.add_moving_point(point_c3);
    // mregion_face_1.add_moving_point(point_d1);
    // mregion_face_1.add_moving_point(point_d2);
    // mregion_face_1.add_moving_point(point_d3);
    // mregion_face_1.add_moving_point(point_a3);


    // mregion.add_mregion_face(mregion_face_1);


    // std::cout << "\n\nStart inside test\n\n";

    // for(std::size_t i = 0; i < 100; i++){
        
    //     auto static k = Kernel::FT(0.0);

    //     auto is_inside = mregion.is_point_inside(input_point, k);
    //     std::cout << std::boolalpha << "is_inside (" << k.doubleValue() << "): " << is_inside << "\n";

    //     k = k + 0.01;
    // }

    // std::cout << "\n\nEnd inside test\n\n\n";



    std::cout << "Alle Moving-Punkte:\n\n";

    for(auto &mregion_face : input_mregion.get_m_mregion_faces())
    {
        for(auto &moving_point : mregion_face.get_m_moving_points())
        {
            std::cout << "Äußerer Rand: " << moving_point.to_string() << "\n";
        }

        for(auto &hole : mregion_face.get_m_moving_holes())
        {
            for(auto &hole_point : hole.get_m_moving_points())
            {
                std::cout << "Loch: " << hole_point.to_string() << "\n";
            }
        }
        
        std::cout << "\n\n";
    }


    std::cout << "\n\n";


    auto distance_equations = std::vector<std::shared_ptr<btlf::DistanceEquation>>();

    auto segment_equations = std::vector<std::shared_ptr<btlf::SegmentEquation>>();


    for(auto &mregion_face : input_mregion.get_m_mregion_faces())
    {   
        auto moving_points_face = mregion_face.get_m_moving_points();

        auto moving_holes = mregion_face.get_m_moving_holes();

        for(std::size_t i = 0; i < moving_points_face.size(); i++)
        {   
            auto mp_current = moving_points_face[i];
            auto mp_next_index = (i == moving_points_face.size()-1) ? 0 : i+1;
            auto mp_next = moving_points_face[mp_next_index];
              
            auto segment_name = moving_points_face[i].get_m_name() + "_" + moving_points_face[mp_next_index].get_m_name();

            auto segment_type = (i % 2 == 0) ? btlf::SegmentType::POINT_TO_SEGMENT : btlf::SegmentType::SEGMENT_TO_POINT;

            if(!mp_current.has_equal_x_component(mp_next))
            { 
                // auto next_index = (i == moving_points_face.size()-1) ? 0 : i+1;

                auto pair_movingpoint = moving_points_face[i].order_size(moving_points_face[mp_next_index], true);

                auto segment_ptr = std::make_shared<btlf::SegmentEquation>(pair_movingpoint.first, pair_movingpoint.second, input_point, segment_type, segment_name);

                segment_equations.push_back(segment_ptr);
            }
            
            else
            {
                auto pair_movingpoint = moving_points_face[i].order_size(moving_points_face[mp_next_index], false);
                
                auto distance_eq = std::make_shared<btlf::LinearDistanceEquation>(pair_movingpoint.first, pair_movingpoint.second, input_point, segment_type, "D_" + segment_name);


                if(distance_eq->get_m_interval().get_m_start_interval().isZero() && distance_eq->get_m_interval().get_m_end_interval().isZero())
                {
                        continue;
                }

                distance_equations.push_back(distance_eq);
            }

        }

        for(auto &moving_hole : moving_holes)
        {   
            auto moving_points_hole = moving_hole.get_m_moving_points();

            for(std::size_t i = 0; i < moving_points_hole.size(); i++)
            {   
                auto mp_current = moving_points_hole[i];
                auto mp_next_index = (i == moving_points_hole.size()-1) ? 0 : i+1;
                auto mp_next = moving_points_hole[mp_next_index];

                auto segment_name = moving_points_hole[i].get_m_name() + "_" + moving_points_hole[mp_next_index].get_m_name();

                auto segment_type = (i % 2 == 0) ? btlf::SegmentType::POINT_TO_SEGMENT : btlf::SegmentType::SEGMENT_TO_POINT;

                if(!mp_current.has_equal_x_component(mp_next))
                {

                    auto pair_movingpoint = moving_points_hole[i].order_size(moving_points_hole[mp_next_index], true);

                    auto segment_ptr = std::make_shared<btlf::SegmentEquation>(pair_movingpoint.first, pair_movingpoint.second, input_point, segment_type, segment_name);

                    segment_equations.push_back(segment_ptr);
                }

                else
                {   
                    auto pair_movingpoint = moving_points_hole[i].order_size(moving_points_hole[mp_next_index], false);
                
                    auto distance_eq = std::make_shared<btlf::LinearDistanceEquation>(pair_movingpoint.first, pair_movingpoint.second, input_point, segment_type, "D_" + segment_name);
                    

                    if(distance_eq->get_m_interval().get_m_start_interval().isZero() && distance_eq->get_m_interval().get_m_end_interval().isZero())
                    {
                        continue;
                    }

                    distance_equations.push_back(distance_eq);
                }

            }

        }

    }




    std::cout << "Alle Segment-Gleichungen:\n\n";

    for(auto &segment : segment_equations)
    {   
        std::cout << segment->to_string() << "\t" << segment->get_m_interval().to_string() << '\n';
    }



    std::cout << "\n\n";


    



    for(auto &mregion_face : input_mregion.get_m_mregion_faces())
    {
        auto moving_points_face = mregion_face.get_m_moving_points();

        for(auto &moving_point : moving_points_face)
        {
            auto distance_quadratic_equation_name = "D_" + moving_point.get_m_name() + "_" + input_point.get_m_name();
            auto distance_quadratic_equation = std::make_shared<btlf::QuadraticDistanceEquation>(moving_point, input_point, distance_quadratic_equation_name);
            distance_equations.push_back(distance_quadratic_equation);
        }

        auto all_holes_of_face = mregion_face.get_m_moving_holes();

        for(auto &hole : all_holes_of_face)
        {
            auto hole_moving_points = hole.get_m_moving_points();

            for(auto &hole_point : hole_moving_points)
            {
                auto distance_quadratic_equation_name = "D_" + hole_point.get_m_name() + "_" + input_point.get_m_name();
                auto distance_quadratic_equation = std::make_shared<btlf::QuadraticDistanceEquation>(hole_point, input_point, distance_quadratic_equation_name);
                distance_equations.push_back(distance_quadratic_equation);
            }
        }


    }


    for(auto &segment_equation : segment_equations)
    {
        auto distance_linear_equation_name = "D_" + segment_equation->get_m_name() + "_" + input_point.get_m_name();
        auto distance_linear_equation = std::make_shared<btlf::LinearDistanceEquation>(*segment_equation, input_point, distance_linear_equation_name);
        
        auto distance_equation_interval = distance_linear_equation->get_m_interval();

        if(distance_equation_interval.get_m_start_interval().isZero() && distance_equation_interval.get_m_end_interval().isZero())
        {
            continue;
        }

        distance_equations.push_back(distance_linear_equation);
    }


     /*
         Output all distance equations
     */

    auto anzahlSegment = 0;

    std::cout << "Alle Distanzgleichungen: \n\n";

    for(auto &distance_equation : distance_equations)
    {
        std::cout << distance_equation->to_string() << distance_equation->get_m_interval().to_string() << "\n\n";
        anzahlSegment++;
    }


    std::cout << "\n\n\n";

    // std::cout << "Taylor start\n";
    // std::cout << distance_equations[0]->to_string() << "\n\n";

    // auto taylor = btlf::TaylorPolynomGrade2{distance_equations[0], 0.001};

    // std::cout << taylor.to_string();

    // taylor.inverse();

    // std::cout << taylor.to_string();

    // std::cout << "Taylor end\n";

    std::cout << "\n\n\n";


    // for(auto &distance_equation : distance_equations)
    // {
    //     auto points = distance_equation->calculate_zeros();

    //     std::cout << distance_equation->to_string() << "\n";

    //     for(auto &point : points)
    //     {
    //         std::cout << "Nullstelle: " << point.m_point << "\n";
    //     }

    //     std::cout << "\n\n\n";
    // }



    // std::cout << "\n\n\n";

    // std::cout << distance_equations[11]->to_string() << "\n\n";

    // std::static_pointer_cast<btlf::LinearDistanceEquation>(distance_equations[11])->inverse();

    // std::cout << distance_equations[11]->to_string() << "\n\n";

    // std::cout << "\n\n\n";


    // auto intersection_points = distance_equations[9]->intersect(*distance_equations[8]);

    // for(auto result : intersection_points)
    // {
    //     std::cout << result << "\n";
    // }



    // std::cout << "Paarweise Berechnung der Schnittpunkte (Ungültige Schnittpunkte werden mit den Wert -100 gekennzeichnet): \n";

    // for(std::size_t i = 0; i < distance_equations.size(); i++)
    // {
    //     auto current_equation = distance_equations[i];

    //     for(std::size_t j = 0; j < distance_equations.size(); j++)
    //     {   
    //         if(current_equation->get_m_id() != distance_equations[j]->get_m_id())
    //         {   
    //             auto res = current_equation->intersect(*distance_equations[j]);

    //             for(int k = 0; auto &r : res)
    //             {
    //                 std::cout 
    //                             << std::setprecision(btlf::LONG_DOUBLE_DISPLAY_ACCURACY)
    //                             << "t" 
    //                             << k
    //                             << " = " 
    //                             <<  r 
    //                             << "\n";
    //                 k++;
    //             }

    //             std::cout << "\n\n";
    //         }

    //         std::cout << "\n\n";
    //     }

    // }
    
    std::cout << "\n\n\n";

    std::cout << "Start algo\n\n";

    int it = 0;

    
  

    auto mreal = btlf::Mreal(input_mregion.get_m_start_date(), input_mregion.get_m_end_date());

    auto curr_eq = find_initial_distance_equation(distance_equations);
    auto curr_point = Kernel::FT(0.0);

    while(curr_eq != nullptr)
    {
        auto curr_list = build_current_list(distance_equations, curr_eq);
        auto mreal_unit = find_next_mreal_unit(curr_list, curr_eq, curr_point, mreal);

        std::cout << mreal_unit.to_string() << "\n\n";

        mreal.add_mreal_unit(mreal_unit);

        curr_point = mreal_unit.get_end_floating_point();

       // std::cout << "\n\n Curr_point: " << curr_point << "\n\n";

        curr_eq = mreal_unit.get_next_distance_equation();

        it++;
    }

    std::cout << "End algo\n\n";
    



    auto mreal_changel_model = mreal.do_new_correction_original(input_point, input_mregion);
    mreal_changel_model.do_correction_date_limits();
    
    const auto end = ClockType::now();
    

    //auto zero_parts = mreal.do_zero_part_correction(input_point, input_mregion);
    //zero_parts.do_correction_date_limits();

    // std::cout << "\n\n\nTest Start \n";

    // for(auto &unit : zero_parts.get_mreal_units())
    // {
    //     std::cout << unit.to_string() << "\n\n";
    // }
    // std::cout << "\n\n\nTest End \n";




    // std::cout << "Taylor test start\n";

    //auto m_taylor = mreal.do_taylor_approximation(input_point, input_mregion);
    // m_taylor.do_correction_date_limits();

    // for(auto &unit : m_taylor.get_mreal_units())
    // {
    //     std::cout << unit.to_string() << "\n\n";
    // }

    // std::cout << "Taylor test end\n";




    std::cout << "\n\n\n";

    /*
        Print all original mreal_units
    */

    // std::cout << "Mreal original:\n\n";

    // for(auto &mreal_unit : mreal_changel_model.get_mreal_units())
    // {
    //     std::cout << mreal_unit.to_string() << "\n\n";
    // }




    std::cout << "\n\n\n\n\n";



    const auto elapsed_time = std::chrono::duration_cast<ClockRes>(end - start).count();







    mreal_to_file(mreal_changel_model, btlf::MrealVariant::CHANGED_MODEL);
   // mreal_to_file(zero_parts, btlf::MrealVariant::ZERO_PARTS);
   // mreal_to_file(m_taylor, btlf::MrealVariant::APPROXIMATION);

    
    std::cout << "\n\n\nZeit: " << elapsed_time << "ms\n\n";

    std::cout << "\n\nAnzahl Segmente: " << anzahlSegment << "\n\n";

    std::cout << "\n\nAnzahl Iterationen: " << it << "\n\n";

    std::cout << "\n\nAnzahl Berechnungen: " << (anzahlSegment-1) * it << "\n\n";


    std::cout << "End progam" << "\n";

    return 0;
}
