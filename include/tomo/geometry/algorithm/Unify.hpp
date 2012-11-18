#include "tomo/geometry/prim/Ring.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace algorithm
    {
      struct Unify
      {
        typedef prim::Ring Ring;
        typedef std::vector<Ring> Rings;

        void operator()(const Rings& _input, Rings& _output)
        {
          typedef std::list<Ring> RingList;
          RingList _inputRings;

          BOOST_FOREACH( const Ring& _ring, _input)
          {
            Ring _newRing(_ring);
            boost::geometry::correct(_newRing);
            _inputRings.push_back(_newRing);
          }

          typedef RingList::iterator iterator;
          for (iterator i = _inputRings.begin(); i != _inputRings.end(); ++i)
          {
            iterator j = i;
            ++j;
            for (; j != _inputRings.end(); ++j)
            {
              Rings _newRings;
              boost::geometry::union_(*i,*j,_newRings);
              if (_newRings.size() == 1)
              {
                *i = _newRings.back();
                _inputRings.erase(j);
                j = i;
              }
            }
          }
          _output.insert(_output.end(),_inputRings.begin(),_inputRings.end());
        }
      };
    }
  }
}


