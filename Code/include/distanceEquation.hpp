#pragma once

#include "segmentEquation.hpp"
#include <tuple>
#include <utility>
#include <cstdint>
#include <array>


namespace btlf
{

/*
    Fördern die Usings die Lesbarkeit?
*/
using IntersectionPair = std::pair<Kernel::FT, Kernel::FT>;

using HesseNormalFormPair = std::pair<Kernel::FT, Kernel::FT>;

using IntersectionResults = const std::array<Kernel::FT, btlf::ARRAY_SIZE>;

using BinomialTuple = std::tuple<Kernel::FT, Kernel::FT, Kernel::FT>;

using EquationPrefactorTuple = std::tuple<Kernel::FT, Kernel::FT, Kernel::FT>;

using ClockType = std::chrono::steady_clock;
using ClockRes = std::chrono::milliseconds;


/*
    f1(t) = this
    f2(t) = linear_equation


    CASE_1 = f1(t) && f2(t) >= 0.0,

    CASE_2 = f1(t) >= 0.0 && f2(t) < 0.0

    CASE_3 = f1(t) < 0.0 && f2(t) >= 0.0

    CASE_4 = f1(t) < 0.0 && f2(t) < 0.0
*/
enum class LinearCalculationCase
{
    CASE_1 = 0,
    CASE_2 = 1,
    CASE_3 = 2,
    CASE_4 = 3
};



/*
    f1(t) = abs linear equation


    CASE_1 = f1(t) >= 0.0

    CASE_2 = f1(t) < 0.0
*/
enum class QuadraticCalculationCase
{
    CASE_1 = 0,
    CASE_2 = 1
};



enum class DistanceEquationType
{
    QUADRATIC = 0,
    LINEAR = 1,
    ZERO = 2,
    TAYLOR = 3,
    NO_SEGMENT = 4
};


/*
    The enum determine the kind of a specific zero point.
*/
enum class ZeroPointType
{
    SINGLE = 0,
    DOUBLE = 1,
    INVALID = 2
};


class ZeroPoint
{
public:
    ZeroPoint() = delete;
    ZeroPoint(const Kernel::FT &point, ZeroPointType type);
    ~ZeroPoint() = default;

    Kernel::FT m_point;
    ZeroPointType m_type;
    
};




class HesseNormalForm
{
public:
    HesseNormalForm() = delete;
    HesseNormalForm(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point);
    ~HesseNormalForm() = default;

    const Kernel::FT &get_m_linear_prefactor() const;
    const Kernel::FT &get_m_constant_value() const;

private:
    Kernel::FT m_linear_prefactor;
    Kernel::FT m_constant_value;

    HesseNormalFormPair caluculate_hesse_normal_form(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point);
};




class BinomialFormula
{
public:
    BinomialFormula() = delete;
    BinomialFormula(const Kernel::FT &linear_prefactor, const Kernel::FT &constant_value);
    BinomialFormula(const Kernel::FT &quadratic_prefactor, const Kernel::FT &linear_prefactor, const Kernel::FT &constant_value);
    ~BinomialFormula() = default;

    const Kernel::FT &get_m_quadratic_prefactor() const;
    const Kernel::FT &get_m_linear_prefactor() const;
    const Kernel::FT &get_m_constant_value() const;

private:
    Kernel::FT m_quadratic_prefactor;
    Kernel::FT m_linear_prefactor;
    Kernel::FT m_constant_value;

    BinomialTuple applying_binomial_formula(const Kernel::FT &linear_prefactor, const Kernel::FT &constant_value);
};





class DistanceEquation
{
public:
    DistanceEquation();

    /*
        Case : No segment
    */
    DistanceEquation(const MovingPoint &to_mp1, const MovingPoint &to_mp2, const MovingPoint &mp, btlf::SegmentType type, const std::string name);


    DistanceEquation(Kernel::FT &quadratic_prefactor, Kernel::FT &linear_prefactor, Kernel::FT &const_value);

    DistanceEquation(const std::shared_ptr<DistanceEquation> &other);


    DistanceEquation(const MovingPoint &segment_point, const MovingPoint &to_segment_point, const std::string name = "Distance");

    DistanceEquation(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point, const std::string name = "Distance");

    virtual ~DistanceEquation() = default;



    virtual Kernel::FT calculate_value(const Kernel::FT &t_value, bool abso = true) const = 0;
    virtual std::string to_string() const = 0;

    void set_m_quadratic_prefactor(const Kernel::FT &prefactor);
    void set_m_linear_prefactor(const Kernel::FT &linear_prefactor);
    void set_m_constant_value(const Kernel::FT &constant_value);

    const Kernel::FT &get_m_quadratic_prefactor() const;
    const Kernel::FT &get_m_linear_prefactor() const;
    const Kernel::FT &get_m_constant_value() const;
    const std::string &get_m_name() const;
    const Interval &get_m_interval() const;

    const MovingPoint &get_m_to_segment() const;


    DistanceEquationType get_m_distance_equation_type() const;
    
    std::uint32_t get_m_id() const;


    IntersectionResults intersect(DistanceEquation &other_distance_equation) const;

    /*
        Als erster Test ist der Rückgabetyp void, um die Nullenstellen zu berechen.
    */
    std::vector<btlf::ZeroPoint> calculate_zeros();




protected:
    std::uint32_t m_id;

    MovingPoint m_to_segment_point;
    
    Kernel::FT m_quadratic_prefactor;
    Kernel::FT m_linear_prefactor;
    Kernel::FT m_constant_value;

    std::string m_name;

    Interval m_interval;

    DistanceEquationType m_distance_equation_type;


private:
    static std::uint32_t m_static_next_id;

    IntersectionResults intersect_linear_with_linear(DistanceEquation &linear_equation) const;
    IntersectionResults intersect_quadratic_with_linear(const DistanceEquation &quadratic_equation) const;

    EquationPrefactorTuple calculate_equation_prefactors_for_calculation(const DistanceEquation &other_equation) const;
    bool mock_solution_test(const Kernel::FT &test_value, const DistanceEquation &other_equation) const;


    BinomialFormula caluculate_member_variables(const MovingPoint &segment_point, const MovingPoint &to_segment_point);


    bool check_intersection_point_condition(const DistanceEquation &linear_equation, const Kernel::FT &intersection_point, const bool mode) const;


    Kernel::FT calculate_possible_linear_intersection_point(DistanceEquation &linear_eqaution, LinearCalculationCase calculation_case) const;


    IntersectionPair calculate_possible_quadratic_intersection_points(const DistanceEquation &other_equation, EquationPrefactorTuple &prefactor_tuple, QuadraticCalculationCase calculation_case) const;
    IntersectionPair calculate_true_quadratic_intersection_points(const EquationPrefactorTuple &prefactor_tuple) const;

    Kernel::FT calculate_discriminatory(const EquationPrefactorTuple &prefactor_tuple) const;

    IntersectionPair calculate_quadratic_linear_case1(const DistanceEquation &other_equation) const;
    IntersectionPair calculate_quadratic_linear_case2(const DistanceEquation &other_equation) const;
    IntersectionPair calculate_quadratic_linear_case3(const DistanceEquation &other_equation) const;

    IntersectionPair calculate_quadratic_quadratic_case1(const EquationPrefactorTuple &prefactor_tuple) const;
    IntersectionPair calculate_quadratic_quadratic_case2(const EquationPrefactorTuple &prefactor_tuple) const;
    IntersectionPair calculate_quadratic_quadratic_case3(const EquationPrefactorTuple &prefactor_tuple) const;



    ZeroPoint caluculate_linear_zeros();
    std::vector<ZeroPoint> calculate_quadratic_zeros();
};



class QuadraticDistanceEquation : public DistanceEquation
{
public:
    QuadraticDistanceEquation();
    QuadraticDistanceEquation(Kernel::FT &quadratic_prefactor, Kernel::FT &linear_prefactor, Kernel::FT &const_value);
    QuadraticDistanceEquation(const MovingPoint &segment_point, const MovingPoint &to_segment_point, const std::string name = "Distance");
    QuadraticDistanceEquation(const std::shared_ptr<DistanceEquation> &other); 
    virtual ~QuadraticDistanceEquation() = default;

    Kernel::FT calculate_value(const Kernel::FT &t_value, bool abso = true) const final;
    std::string to_string() const final;

private:
    MovingPoint m_segment_point;

};



class LinearDistanceEquation : public DistanceEquation
{
public:
    LinearDistanceEquation() = default;
    LinearDistanceEquation(const SegmentEquation &segment_equation, const MovingPoint &to_segment_point, std::string name = "Distance");
    LinearDistanceEquation(const std::shared_ptr<DistanceEquation> &other);
    
    /*
        Case: Both x-component are equal.
    */
    LinearDistanceEquation(const MovingPoint &to_mp1, const MovingPoint &to_mp2, const MovingPoint &mp, btlf::SegmentType type, std::string name);


    virtual ~LinearDistanceEquation() = default;

    std::string to_string() const final;
    Kernel::FT calculate_value(const Kernel::FT &t_value, bool abso = true) const final;

    void inverse();

private:
    SegmentEquation m_segment_equation;

};




}

