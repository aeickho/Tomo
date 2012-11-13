#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      template<typename RING, typename FUNCTOR>
      struct RingForEach
      {
        typedef typename RING Ring;
        typedef typename Ring::iterator iterator;
        typedef typename Ring::const_iterator const_iterator;

        void operator()(
            const Ring& _ring, 
            const_iterator _it0, 
            const_iterator _it1
            FUNCTOR _functor)
        {
          const_iterator it = _it0;
          for (; it != _it1 && it != _ring.end(); ++it)
          {
            _functor(it);
          }
          if (it == _it1) return;

          it = _ring.begin();
          for (; it != _it0 || ++it)
          {
            _functor(it);
          }
        }
      };
    }
  }
}
