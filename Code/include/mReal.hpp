#pragma once

#include "mRegion.hpp"
#include "distanceEquation.hpp"
#include "polygonCheck.hpp"
#include "timeInterpolation.hpp"
#include "taylor.hpp"

namespace btlf
{

enum class MrealVariant
{
    CHANGED_MODEL = 1,
    ZERO_PARTS = 2,
    APPROXIMATION = 3
};



class MrealUnit
{
public:
    MrealUnit() = delete;

    /*
        Konstruktor für die 0-Unit (Fall: Innerhalb der Mregion)
    */
    MrealUnit(const Kernel::FT &start, const Kernel::FT &end, const std::string &start_date, const std::string &end_date);


    MrealUnit(const MrealUnit &other);

    MrealUnit(std::shared_ptr<btlf::DistanceEquation> &fixed_equation, const Kernel::FT &start_floating_point);

    ~MrealUnit() = default;

    void set_m_end_floating_point(const Kernel::FT &end_floating_point);
    void set_m_start_floating_point(const Kernel::FT &start_floating_point);

    void set_m_distance_equation(const std::shared_ptr<btlf::DistanceEquation> &distance_equation);
    void set_m_next_distance_equation(const std::shared_ptr<btlf::DistanceEquation> &next_distance_equation);

    void set_m_end_date_including(const bool is_inlcuding);
    void set_m_start_date_including(const bool is_inlcuding);

    void set_m_start_date_str(const std::string &start_str);
    void set_m_end_date_str(const std::string &end_str);

    void set_m_pull_root(const bool pull_root);


    // Model extension
    void set_m_inverse_result(const bool inverse_result);
    bool get_m_inverse_result() const;


    std::string get_m_start_date_str() const;
    std::string get_m_end_date_str() const;

    Kernel::FT get_end_floating_point() const;
    Kernel::FT get_start_floating_point() const;
    
    std::shared_ptr<btlf::DistanceEquation> get_next_distance_equation() const;
    std::shared_ptr<btlf::DistanceEquation> get_distance_equation() const;
    
    bool get_m_start_date_including() const;
    bool get_m_end_date_including() const;
    bool get_m_pull_root() const;

    std::string to_string() const;

    // Die m_distance_equation umdrehen!
    void inverse();

    bool value_is_inside_interval(const Kernel::FT &value);

private:
    Kernel::FT m_start_floating_point;
    Kernel::FT m_end_floating_point;
    
    bool m_start_date_including;
    bool m_end_date_including;
    
    std::shared_ptr<btlf::DistanceEquation> m_distance_equation;
    std::shared_ptr<btlf::DistanceEquation> m_next_distance_equation;
    
    bool m_pull_root;

    // Model extension (Wird im Konstruktor beim Erstellen auf false gesetzt! Default Verhalten)
    bool m_inverse_result;


    std::string m_start_date_str;
    std::string m_end_date_str;
};




// Das sind erstmal meine ersten Gedanken dazu. Das ist noch nicht final!
class Mreal
{
public:
    Mreal() = default;
    Mreal(const std::string &start_date, const std::string &end_date);
    ~Mreal() = default;

    void add_mreal_unit(const btlf::MrealUnit &mreal_unit);

    std::vector<btlf::MrealUnit> get_mreal_units() const;

    void set_m_start_date(const std::string &start_date);
    void set_m_end_date(const std::string &end_date);

    std::string get_m_start_date() const;
    std::string get_m_end_date() const;


    
    /*
        Schließt momentan nur die linearen Distanzen ein.
    */
    bool do_correction_original(const MovingPoint &mp, Mregion &mregion);

    /*
        Neue Variante um negative Bereiche im Mreal zu setzen. (Mreal Korrektur!)
    */
    Mreal do_new_correction_original(const MovingPoint &mp, Mregion &mregion);


    /*
        taylor polynom approximation
    */
    Mreal do_taylor_approximation(const MovingPoint &mp, Mregion &mregion);


    Mreal do_zero_part_correction(const MovingPoint &mp, Mregion &mregion);


    void do_correction_date_limits();

private:
    std::string m_start_date;
    std::string m_end_date;

    std::vector<btlf::MrealUnit> m_mreal_units;

    Mreal summarize_zero_parts(Mreal &to_summarize);
    void set_correct_next_equations(Mreal &to_correct);
};

class EndIntervalInfo
{

public:
    EndIntervalInfo() = default;

    EndIntervalInfo(const Kernel::FT &end_interval_floating_point, const std::shared_ptr<DistanceEquation> &next_equation);

    ~EndIntervalInfo() = default;

    const Kernel::FT get_m_end_interval_floating_point() const;
    const std::shared_ptr<DistanceEquation> get_m_next_equation() const;

private:
    Kernel::FT m_end_interval_floating_point;
    std::shared_ptr<btlf::DistanceEquation> m_next_equation;
};


class HelpInfo
{
public:
    HelpInfo() = default;

    HelpInfo(const std::shared_ptr<DistanceEquation> &fixed_equation, const Kernel::FT &fixed_position, const std::shared_ptr<DistanceEquation> &loop_equation, const Kernel::FT &next_point, const bool first_intersection, EndIntervalInfo &info);

    ~HelpInfo() = default;


    std::shared_ptr<DistanceEquation> get_m_fixed_equation() const;
    Kernel::FT get_m_fixed_position() const;
    std::shared_ptr<DistanceEquation> get_m_loop_equation() const;
    Kernel::FT get_m_next_point() const;
    bool get_m_first_intersection() const;

    EndIntervalInfo get_m_info() const;

    void set_m_info(EndIntervalInfo &info);

    void set_m_next_point(const Kernel::FT &next_point);
    void set_m_loop_equation(const std::shared_ptr<DistanceEquation> &loop_equation);
    void set_m_first_intersection(const bool bool_value);

private:
    std::shared_ptr<DistanceEquation> m_fixed_equation;
    Kernel::FT m_fixed_position;
    std::shared_ptr<DistanceEquation> m_loop_equation;
    Kernel::FT m_next_point;
    bool m_first_intersection;
    EndIntervalInfo m_info;
};






}