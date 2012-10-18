#include "Slice.hpp"

#include <tomo/geometry/aux/Compound.hpp>
#include <tomo/geometry/prim/ConnectableSegment.hpp>

namespace tomo
{
  namespace slicing
  {
    /// The class LineSegmentContainer is used to store line segments to make polygons out of them
    struct LineSegmentPlane : geometry::aux::Compound<geometry::prim::ConnectableSegment >
    {
      TOMO_COMPOUND_PRIMITIVE_NAME(lineSegments);
      typedef geometry::prim::ConnectableSegment linesegment_type;
      typedef geometry::prim::Ring ring_type;

      LineSegmentPlane(Slice* _slice = NULL);

      void makeRings(std::vector<ring_type>& _rings);

      float pos() const;
      void addSegment(const point_type& _p0, const point_type& _p1);
      void aggregate(const LineSegmentPlane& _lineSegmentPlane);

      TBD_PROPERTY_RO(Slice*,slice);
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
