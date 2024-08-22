// #pragma once


// #include <vector>
// #include <iostream>
// #include <iomanip>

// #include "mRegion.hpp"

// #include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
// #include <CGAL/Polygon_2.h>


// using Kernel = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
// using Point_2 = Kernel::Point_2;
// using Polygon_2 = CGAL::Polygon_2<Kernel>;




// namespace btlf
// {


// void test_inside_polygon()
// {

//     std::vector<Point_2> points;

//     points.push_back(Point_2(2.0, 2.0));
//     points.push_back(Point_2(5.0, 5.0));
//     points.push_back(Point_2(8.0, 2.0));
//     points.push_back(Point_2(10.0, 8.5));
//     points.push_back(Point_2(3.0, 7.0));


//     auto polygon = Polygon_2(points.begin(), points.end());

//     auto point_p = Point_2(7.0, 6.0);

//     auto is_inside = polygon.bounded_side(point_p) == CGAL::ON_BOUNDED_SIDE;

//     std::cout << std::boolalpha << is_inside << "\n";

// }





// }




