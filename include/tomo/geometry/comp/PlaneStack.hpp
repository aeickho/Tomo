#pragma once

#include <map>
#include <vector>

#include "tomo/geometry/prim/Primitive.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace comp
    {
      /** @brief Plane Stack is a class template to represent several planes ordered by position
       *  @details Internally, planes are stored in a map<scalar_type,plane_type>
       */
      template<class PLANE>
      struct PlaneStack : prim::Primitive3f
      {
        typedef PLANE plane_type;
        typedef std::pair<scalar_type,plane_type> key_value_type;
        typedef std::map<scalar_type,plane_type> map_type;
        typedef std::vector<plane_type*> ptrctnr_type;
        typedef std::vector<const plane_type*> const_ptrctnr_type;

        typedef typename map_type::iterator iterator;
        typedef typename map_type::const_iterator const_iterator;

        ptrctnr_type fetch(iterator _itBegin, iterator _itEnd)
        {
          ptrctnr_type _planes;
          _planes.reserve(std::distance(_itBegin,_itEnd));
          for (iterator it = _itBegin; it != _itEnd; ++it)
            _planes.push_back(&it->second);
          return _planes;
        }

        const_ptrctnr_type fetch(const_iterator _itBegin, const_iterator _itEnd) const
        {
          const_ptrctnr_type _planes;
          _planes.reserve(std::distance(_itBegin,_itEnd));
          for (const_iterator it = _itBegin; it != _itEnd; ++it)
            _planes.push_back(&it->second);
          return _planes;
        }

        ptrctnr_type fetch()
        {
          return fetch(planes_.begin(),planes_.end());
        }

        const_ptrctnr_type fetch() const
        {
          return fetch(planes_.begin(),planes_.end());
        }

        ptrctnr_type fetch(scalar_type _begin, scalar_type _end)
        {
          return fetch(get(_begin),get(_end));
        }

        const_ptrctnr_type fetch(scalar_type _begin, scalar_type _end) const
        {
          return fetch(get(_begin),get(_end));
        }

        const map_type& get() const
        {
          return planes_;
        }
        map_type& get()
        {
          return planes_;
        }

        /// Get slice by position
        iterator get(scalar_type _pos)
        {
          return planes_.lower_bound(_pos);
        }

        const_iterator get(scalar_type _pos) const
        {
          return planes_.lower_bound(_pos);
        }

        iterator end()
        {
          return planes_.end();
        }
        iterator begin()
        {
          return planes_.begin();
        }
        const_iterator end() const
        {
          return planes_.end();
        }
        const_iterator begin() const
        {
          return planes_.begin();
        }

      protected:
        map_type planes_;
      };
    }
  }
}
