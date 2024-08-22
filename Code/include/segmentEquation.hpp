#pragma once

#include "movingPoint.hpp"
#include <memory>



/*
    btlf : Bachelor thesis Langer Florian
*/
namespace btlf
{

enum class SegmentType
{
    POINT_TO_SEGMENT = 0,
    SEGMENT_TO_POINT = 1,
    NO_SEGMENT = 2
};



/*
    The class Interval represents an interval.
    For instance in the form [start, end] or ]start, end[;
*/
class Interval
{   
public:
    Interval();

    Interval(const Kernel::FT &start_interval, const Kernel::FT &end_interval, SegmentType segment_type);

    Interval(const Kernel::FT &start_interval, const Kernel::FT &end_interval, bool start, bool end);

    ~Interval() = default;


    std::string to_string() const;

    /*
        Getter for member variables.
    */
    const Kernel::FT &get_m_start_interval() const;
    const Kernel::FT &get_m_end_interval() const;
    bool get_m_start_including() const;
    bool get_m_end_including() const;

private:

    Kernel::FT m_start_interval;

    Kernel::FT m_end_interval;

    bool m_start_interval_including;

    bool m_end_interval_including;

    void set_members_interval(const Kernel::FT &start_interval, const Kernel::FT &end_interval, bool start_interval_including, bool end_interval_including);

};






/*
    Forward declaration of the class Perpendicular.
*/
class Perpendicular;






class LinearEquation
{
public:
    LinearEquation() = default;

    /*
        Constructor for a segment.
    */
    LinearEquation(const MovingPoint &point_1, const MovingPoint &point_2, std::string name = "f");

    /*
        Constructor for a perpendicular.
    */
    LinearEquation(const LinearEquation &segement_equation, const MovingPoint &perpendicular_point, const std::string name = "f");

    virtual ~LinearEquation() = default;
    

    std::string to_string() const;

    /*
        Getter for the member variables.
    */
    const Kernel::FT &get_m_gradient() const;
    const LinearInterpolation &get_m_y_axis() const;
    const std::string &get_m_name() const;

protected:
    Kernel::FT m_gradient;

    LinearInterpolation m_y_axis;

    std::string m_name;

private:
    /*
        The calculation for a segment.
    */
    Kernel::FT calculate_m_gradient(const MovingPoint &point_1, const MovingPoint &point_2) const;

    /*
        The calculation for a perpendicular.
    */
    Kernel::FT calculate_m_gradient(const LinearEquation &segmentEquation) const;
    
    /*
        General calculation for both cases.
    */
    LinearInterpolation calculate_m_y_axis(const MovingPoint &point, const Kernel::FT gradient) const;
};






class SegmentEquation : public LinearEquation
{
public:
    SegmentEquation() = default;
    
    SegmentEquation(const MovingPoint &point_1, const MovingPoint &point_2, const MovingPoint &moving_point_to_segment, const SegmentType type, std::string = "f");

    SegmentEquation(const SegmentEquation &other);

    virtual ~SegmentEquation() = default;


    /*
        Getter for member variables.
    */
    const Perpendicular &get_m_perpendicular() const;
    const MovingPoint &get_m_moving_point_to_segment() const;
    const Interval &get_m_interval() const;
    SegmentType get_m_segment_type() const;

private:
    SegmentType m_segment_type;

    MovingPoint m_segment_point1;

    MovingPoint m_segment_point2;

    MovingPoint m_moving_point_to_segment;

    std::shared_ptr<Perpendicular> m_perpendicular;

    Interval m_interval;


    Interval determine_m_interval();
};




class Perpendicular : public LinearEquation
{
public:
    Perpendicular() = default;

    Perpendicular(const SegmentEquation &segment_equation, const MovingPoint &moving_point_to_segment, const std::string name = "f");

    virtual ~Perpendicular() = default;


    /*
        Getter for member variables.
    */
    const LinearInterpolation &get_perpendicular_point_x_component() const;

private:
    LinearInterpolation m_perpendicular_point_x_component;

    LinearInterpolation calculate_m_perpendicular_point_x_component(const SegmentEquation &segment_equation);
};


}

