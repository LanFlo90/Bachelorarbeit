#pragma once


#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Polygon_2.h>
#include <iomanip>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
using Point_2 = Kernel::Point_2;
using Polygon_2 = CGAL::Polygon_2<Kernel>;



#include "movingPoint.hpp"


namespace btlf
{


class MovingHole
{
public:
    MovingHole() = default;
    ~MovingHole() = default;


    void add_moving_point(const MovingPoint &moving_point);

    std::vector<MovingPoint> get_m_moving_points() const;


private:
    std::vector<MovingPoint> m_moving_points;
};



class MregionFace
{
public:
    MregionFace() = default;
    ~MregionFace() = default;

    void add_moving_point(const MovingPoint &moving_point);

    std::vector<MovingPoint> get_m_moving_points() const;

    std::vector<MovingHole> get_m_moving_holes() const;

    void add_moving_hole(const MovingHole &moving_hole);

private:
    std::vector<MovingPoint> m_moving_points;

    std::vector<MovingHole> m_moving_holes;
};




// Es muss noch die Klasse MregionFace erstellt werden, und Mregion benötigt dann einen Vector von MregionFace
class Mregion
{
public:
    Mregion() = default;
    Mregion(const std::string &start_date, const std::string &end_date, const bool start_date_including, const bool end_date_including);

    ~Mregion() = default;

    // Getter für die Datums
    std::string get_m_start_date() const;
    std::string get_m_end_date() const;

    std::vector<MregionFace> get_m_mregion_faces() const;
    void add_mregion_face(const MregionFace &mregion_face);

    void set_m_start_date(const std::string &start_date);
    void set_m_end_date(const std::string &end_date);
    void set_m_start_date_including(const bool start_date_including);
    void set_m_end_date_including(const bool end_date_including);

    bool is_point_inside(const MovingPoint &moving_point, Kernel::FT &t);


private:
    std::string m_start_date;
    std::string m_end_date;
    bool m_start_date_including;
    bool m_end_date_including;


    std::vector<MregionFace> m_mregion_faces;
};


}