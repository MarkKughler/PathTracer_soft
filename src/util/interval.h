#pragma once

class cInterval
{

public:
    double min_t;
    double max_t;

    double clamp_t(double x) const 
    {
        if (x < min_t) x = min_t;
        if (x > max_t) x = max_t;
        return x;
    }

    cInterval() : min_t(+infinity), max_t(-infinity) {}
    cInterval(double _min, double _max) : min_t(_min), max_t(_max) {}
    cInterval(const cInterval& a, const cInterval& b)
    {
        min_t = a.min_t <= b.min_t ? a.min_t : b.min_t;
        max_t = a.max_t >= b.max_t ? a.max_t : b.max_t;
    }

    double Size() const { return max_t - min_t; }

    cInterval Expand(double delta) const
    {
        double padding = delta / 2;
        return cInterval(min_t - padding, max_t + padding);
    }

    bool Contains(double x) const { return min_t <= x && x <= max_t; }
    bool Surrounds(double x) const { return min_t < x && x < max_t; }

    static const cInterval empty, universe;

};

const cInterval cInterval::empty = cInterval(+infinity, -infinity);
const cInterval cInterval::universe = cInterval(-infinity, +infinity);

cInterval operator + (const cInterval& ival, double displacement)
{
    return cInterval(ival.min_t + displacement, ival.max_t + displacement);
}

cInterval operator + (double displacement, const cInterval& ival)
{
    return ival + displacement;
}