#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace split
      {
        template<typename PRIMITIVE>
        struct Candidate
        {
          typedef PRIMITIVE primitive_type;
          typedef typename primitive_type::scalar_type scalar_type;

          Candidate(scalar_type _pos = INF,
                         const primitive_type* _primitive = nullptr) :
            pos_(_pos),
            primitive_(_primitive)
          {}

          void put(scalar_type _pos, const primitive_type* _primitive)
          {
            primitive_=_primitive;
            pos_=_pos;
          }

          TBD_PROPERTY_RO(scalar_type,pos);
          TBD_PROPERTY_RO(const primitive_type*,primitive);
        };

      }
    }
  }
}
