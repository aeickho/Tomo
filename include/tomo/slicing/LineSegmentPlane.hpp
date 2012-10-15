#include "Slice.hpp"

#include <tomo/geometry/aux/Compound.hpp>
#include <tomo/geometry/prim/Ring.hpp>

namespace tomo
{
  namespace slicing
  {
    /// The class LineSegmentContainer is used to store line segments to make polygons out of them
    struct LineSegmentPlane : geometry::aux::Compound<geometry::prim::LineSegment>
    {
      TOMO_COMPOUND_PRIMITIVE_NAME(lineSegments);
      typedef geometry::prim::LineSegment linesegment_type;
      typedef geometry::prim::Ring ring_type;
      
      LineSegmentPlane(Slice* _slice = NULL);

      void makeRings(std::vector<ring_type>& _rings);

      float pos() const;
      void addSegment(const point_type& _p0, const point_type& _p1);
      void aggregate(const LineSegmentPlane& _lineSegmentPlane);

      bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vec_type* _normal = NULL) const
      {
        return false;
      }

      TBD_PROPERTY_RO(Slice*,slice);

    private:
      typedef enum { PT_NONE, PT_CLOSURE, PT_HOLE } PolygonType;

      /// Calculates the cross product of front and back and is used to determine orientation of the segment (CW or CCW)
      float orientation(const linesegment_type* _a, const linesegment_type* _b) const;

      PolygonType asPolygon(const linesegment_type* _lineSegment,
                            std::set<const linesegment_type*>& _usedSegments,
                            Polygon& _polygon);


      linesegment_type* nearestSegment(linesegment_type* _lineSegment);
    };

    struct LineSegmentContainer : PlaneStack<float,LineSegmentPlane>
    {
      LineSegmentContainer(Slices& _slices);
      void produceSlices();

      void aggregate(const LineSegmentContainer& _lineSegmentContainer);

    private:
      void initializePlanes();

      Slices& slices_;
    };
  }
}
