#include "mRegion.hpp"



btlf::Mregion::Mregion(const std::string &start_date, const std::string &end_date, const bool start_date_including, const bool end_date_including):
    m_start_date(start_date),
    m_end_date(end_date),
    m_start_date_including(start_date_including),
    m_end_date_including(end_date_including),
    m_mregion_faces(std::vector<MregionFace>())
{

}


// void btlf::Mregion::add_moving_point(const MovingPoint &moving_point)
// {
//     m_moving_points.push_back(moving_point);
// }



// std::vector<btlf::MovingPoint> btlf::Mregion::get_m_moving_points() const
// {
//     return m_moving_points;
// }



std::vector<btlf::MregionFace> btlf::Mregion::get_m_mregion_faces() const
{
    return m_mregion_faces;
}



void btlf::Mregion::add_mregion_face(const MregionFace &mregion_face)
{
    m_mregion_faces.push_back(mregion_face);
}




bool btlf::Mregion::is_point_inside(const MovingPoint &moving_point, Kernel::FT &t)
{   
    auto is_inside_main = bool{false};
    auto is_inside_hole = bool{false};

    auto point_to_check_x_coordinate = moving_point.get_m_x_component().calucate_value(t);
    auto point_to_check_y_coordinate = moving_point.get_m_y_component().calucate_value(t);

    auto point_to_check = Point_2(point_to_check_x_coordinate, point_to_check_y_coordinate);


    std::vector<Polygon_2> all_main_polygons;

    // Löcher Polygone
    std::vector<Polygon_2> all_hole_polygons;


    for(auto &face : this->get_m_mregion_faces())
    {   
        std::vector<Point_2> polygon_points;
        auto moving_points_face = face.get_m_moving_points();

        for(auto moving_point : moving_points_face)
        {
            auto x_corrdinate = moving_point.get_m_x_component().calucate_value(t);
            auto y_corrdinate = moving_point.get_m_y_component().calucate_value(t);

            auto point = Point_2(x_corrdinate, y_corrdinate);

            polygon_points.push_back(point);
        }

        auto polygon = Polygon_2(polygon_points.begin(), polygon_points.end());

        all_main_polygons.push_back(polygon);


        for(auto hole : face.get_m_moving_holes())
        {   
            std::vector<Point_2> polygon_hole_points;

            for(auto hole_point : hole.get_m_moving_points())
            {
                auto x_corrdinate = hole_point.get_m_x_component().calucate_value(t);
                auto y_corrdinate = hole_point.get_m_y_component().calucate_value(t);

                auto point_hole = Point_2(x_corrdinate, y_corrdinate);

                polygon_hole_points.push_back(point_hole);
            }

            auto polygon_hole = Polygon_2(polygon_hole_points.begin(), polygon_hole_points.end());

            all_hole_polygons.push_back(polygon_hole);

        }


    }


    for(auto polygon : all_main_polygons)
    {   
        if(polygon.is_simple())
            if(polygon.bounded_side(point_to_check) == CGAL::ON_BOUNDED_SIDE)
            {
                is_inside_main = true;
                break;
            }   
    }


    for(auto polygon_hole : all_hole_polygons)
    {   
        if(polygon_hole.is_simple())
            if(polygon_hole.bounded_side(point_to_check) == CGAL::ON_BOUNDED_SIDE)
            {   
                // std::cout << "test test test\n\n\n";

                is_inside_hole = true;
                break;
            }   
    }


    return (is_inside_main && !is_inside_hole);
    
}



std::string btlf::Mregion::get_m_start_date() const
{
    return m_start_date;
}


std::string btlf::Mregion::get_m_end_date() const
{
    return m_end_date;
}



void btlf::Mregion::set_m_start_date(const std::string &start_date)
{
    m_start_date = start_date;
}



void btlf::Mregion::set_m_end_date(const std::string &end_date)
{
    m_end_date = end_date;
}



void btlf::Mregion::set_m_start_date_including(const bool start_date_including)
{
    m_start_date_including = start_date_including;
}



void btlf::Mregion::set_m_end_date_including(const bool end_date_including)
{
    m_end_date_including = end_date_including;
}













void btlf::MregionFace::add_moving_point(const MovingPoint &moving_point)
{
    m_moving_points.push_back(moving_point);
}


std::vector<btlf::MovingPoint> btlf::MregionFace::get_m_moving_points() const
{
    return m_moving_points;
}


std::vector<btlf::MovingHole> btlf::MregionFace::get_m_moving_holes() const
{
    return m_moving_holes;
}



void btlf::MregionFace::add_moving_hole(const MovingHole &moving_hole)
{
    m_moving_holes.push_back(moving_hole);
}













void btlf::MovingHole::add_moving_point(const MovingPoint &moving_point)
{
    m_moving_points.push_back(moving_point);
}



std::vector<btlf::MovingPoint> btlf::MovingHole::get_m_moving_points() const
{
    return m_moving_points;
}