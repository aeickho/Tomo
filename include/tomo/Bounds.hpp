#pragma once

#include "tomo/Vector.hpp"
#include "tomo/misc.hpp"

#include <vector>

namespace tomo 
{
  struct Bounds 
  {
    Bounds() { min_ = Point3f(INF,INF,INF); max_ = Point3f(-INF,-INF,-INF); }
    Bounds(Point3f _min, Point3f _max) { this->operator()(_min,_max); }

    void operator()(const Point3f& _min, const Point3f& _max);  

    void validate(); 
    std::vector<Point3f> corners() const;
    void extend(const Bounds& _bounds);

    Vec3f size() const;
    float radius() const { return size().length()/2; }
    
    Point3f center() { return 0.5*(max().vec() + min().vec()); }

    /** @brief Split bounding box in two halves 
     */
    void split(float splitPos, Axis axis, Bounds& boxLeft, Bounds& boxRight) const;

    /** @brief Return axis which largest extent
     */
    Axis dominantAxis() const;

    TBD_PROPERTY_MON(Point3f,min,validate);
    TBD_PROPERTY_MON(Point3f,max,validate);
  };

}
