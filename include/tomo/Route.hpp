#include <set>
#include "tomo/Vector.hpp"
#include "tbd/property.h"

namespace tomo
{
  typedef double Time;

  struct Movement
  {
    Movement(Time _time, Point3f _position = Point3f(), float _speed = 0.0, float _filament = 0.0) :
      position_(_position), speed_(_speed), extrudedLength_(_extrudedLength), time_(_time)
    {
    }

    friend bool operator<(const Movement& lhs, const Movement& rhs)
    {
      return lhs.time() < rhs.time();
    }
    
    TBD_PROPERTY_REF(Point3f,position);
    TBD_PROPERTY(float,speed);
    TBD_PROPERTY(float,extrudedLength);
    TBD_PROPERTY(Time,time);
  };

  struct Event
  {
    
    friend bool operator<(const Movement& lhs, const Movement& rhs)
    {
      return lhs.time() < rhs.time();
    }

  };

  struct Route
  {
      Movement movement(Time _time, Interpolation _interpolation = MI_NONE);
      std::vector<Movement*> movements(Time _begin, Time _end);





    private:
      std::set<Movement> movements_;
      std::set<Event> events_;
  };
}

