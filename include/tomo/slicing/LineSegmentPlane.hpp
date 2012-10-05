#include "Slice.hpp"

#include <tomo/geometry/aux/Compound.hpp>
#include <tomo/geometry/prim/LineSegment.hpp>

namespace tomo
{
  namespace slicing
  {
    /// The class LineSegmentContainer is used to store line segments to make polygons out of them
    struct LineSegmentPlane : geometry::aux::Compound<geometry::prim::LineSegment,2,float>
    {
      TOMO_COMPOUND_PRIMITIVE_NAME(lineSegments);

      typedef float Scalar;
      typedef geometry::prim::LineSegment LineSegment;
      typedef geometry::aux::Ray<2,Scalar> Ray;
      typedef geometry::base::Vec<2,Scalar> Vec;
      
      LineSegmentPlane(Slice* _slice = NULL);

      std::vector<Polygon> makePolygons(float _simplifyThreshold = 0.0);

      float pos() const;
      void addSegment(const point_type& _p0, const point_type& _p1);
      void aggregate(const LineSegmentPlane& _lineSegmentPlane);

      bool intersect(Ray& _ray, Scalar& _tNear, Scalar& _tFar, Vec* _normal = NULL) const
      {
        return false;
      }

      TBD_PROPERTY_RO(Slice*,slice);

    private:
      typedef enum { PT_NONE, PT_CLOSURE, PT_HOLE } PolygonType;

      /// Calculates the cross product of front and back and is used to determine orientation of the segment (CW or CCW)
      float orientation(const LineSegment* _a, const LineSegment* _b) const;

      PolygonType asPolygon(const LineSegment* _lineSegment,
                            std::set<const LineSegment*>& _usedSegments,
                            Polygon& _polygon);


      LineSegment* nearestSegment(LineSegment* _lineSegment);
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
