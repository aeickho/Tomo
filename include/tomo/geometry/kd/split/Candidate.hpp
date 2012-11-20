#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace split
      {
        /// A split candidate holds a possible split position and a pointer to a primitive
        template<typename NODE>
        struct Candidate
        {
          typedef typename NODE::primitive_type primitive_type;
          typedef typename NODE::scalar_type scalar_type;

          Candidate(scalar_type _pos = INF,
                    base::Axis _axis,
                    const primitive_type* _prim = nullptr) :
            pos_(_pos),
            axis_(_axis),
            prim_(_prim)
          {}

          void put(scalar_type _pos, base::Axis _axis, const primitive_type* _prim)
          {
            prim_=_prim;
            pos_=_pos;
          }

          TBD_PROPERTY(base::Axis,axis);
          TBD_PROPERTY(scalar_type,pos);
          TBD_PROPERTY_RO(const primitive_type*,prim);
        };
      }
    }
  }
}
