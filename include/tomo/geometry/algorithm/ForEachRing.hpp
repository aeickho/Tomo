#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace algorithm
    {
      /// Template concept for letting a functor perform on each ring of a certain primitve
      template<typename PRIMITIVE>
      struct ForEachRing
      {

      };

      /// Template concept for letting a functor perform on a std::vector of rings
      template<>
      struct ForEachRing< std::vector<prim::Ring> >
      {
        typedef prim::Ring Ring;
        typedef std::vector<Ring> primitive_type;
        
        template<typename FUNCTOR>
        void operator()(primitive_type& _rings, FUNCTOR& _f) const
        {
          for ( Ring& _ring : _rings) _f(_ring);
        }

        template<typename FUNCTOR>
        void operator()(const primitive_type& _rings, FUNCTOR& _f) const
        {
          for ( const Ring& _ring : _rings) _f(_ring);
        }
      };

      /// Template concept for letting a functor perform on the rings of a polygon
      template<>
      struct ForEachRing<prim::Polygon>
      {
        typedef prim::Polygon primitive_type;
        
        template<typename FUNCTOR>
        void operator()(const primitive_type& _polygon, FUNCTOR& _f) const
        {
          _f(_polygon.boundary());
          ForEachRing()(_polygon.holes(),_f);
        }

        template<typename FUNCTOR>
        void operator()(primitive_type& _polygon, FUNCTOR& _f) const
        {
          _f(_polygon.boundary());
          ForEachRing()(_polygon.holes(),_f);
        }
      };

      /// Template concept for letting a functor perform on the rings of a multipolygon
      template<>
      struct ForEachRing<prim::MultiPolygon>
      {
        typedef prim::Polygon Polygon;
        typedef prim::MultiPolygon primitive_type;
        
        template<typename FUNCTOR>
        void operator()(const primitive_type& _multiPolygon, FUNCTOR& _f) const
        {
          for ( const Polygon& _polygon : _multiPolygon )
            ForEachRing()(_polygon,_f);  
        }

        template<typename FUNCTOR>
        void operator()(primitive_type& _multiPolygon, FUNCTOR& _f) const
        {
          for ( Polygon& _polygon : _multiPolygon )
            ForEachRing()(_polygon,_f);          
        }
      };
    }
  }
}
  
