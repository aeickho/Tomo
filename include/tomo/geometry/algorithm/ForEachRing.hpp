#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace algorithm
    {
      /// Template concept for letting a functor perform on each ring of a certain primitve
      template<typename PRIMITIVE, typename FUNCTOR>
      struct ForEachRing
      {

      };

      /// Template concept for letting a functor perform on a std::vector of rings
      template<typename FUNCTOR>
      struct ForEachRing< prim::Polygon::holes_type >
      {
        typedef std::vector<prim::Ring> primitive_type;
        void operator(primitive_type& _rings, FUNCTOR& _f)
        {
          for ( Ring& _ring : _rings) _f(_ring);
        }

        void operator(const primitive_type& _rings, FUNCTOR& _f)
        {
          for ( const Ring& _ring : _rings) _f(_ring);
        }
      };

      /// Template concept for letting a functor perform on the rings of a polygon
      template<typename FUNCTOR>
      struct ForEachRing<prim::Polygon>
      {
        typedef prim::Polygon primitive_type;
        void operator(const primitive_type& _polygon, FUNCTOR& _f)
        {
          _f(_polygon.boundary());
          ForEachRing<primitive_type::holes_type,FUNCTOR>()(_polygon.holes(),_f);
        }

        void operator(primitive_type& _polygon, FUNCTOR& _f)
        {
          _f(_polygon.boundary());
          ForEachRing<primitive_type::holes_type,FUNCTOR>()(_polygon.holes(),_f);
        }
      };

      /// Template concept for letting a functor perform on the rings of a multipolygon
      template<typename FUNCTOR>
      struct ForEachRing<prim::MultiPolygon>
      {
        typedef prim::MultiPolygon primitive_type;
        void operator(const primitive_type& _multiPolygon, FUNCTOR& _f)
        {
          for ( const Polygon& _polygon : _multiPolygon )
            ForEachRing<Polygon,FUNCTOR>()(_polygon,_f);  
        }

        void operator(primitive_type& _multiPolygon, FUNCTOR& _f)
        {
          for ( Polygon& _polygon : _multiPolygon )
            ForEachRing<Polygon,FUNCTOR>()(_polygon,_f);          
        }
      };
    }
  }
}
  
