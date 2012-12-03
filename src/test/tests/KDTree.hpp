#pragma once

#include "tomo/geometry/kd/nearest/SegmentPointTree.hpp"

#include "../generators/Generator.hpp"
#include "../generators/create/circleWith3Holes.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      struct KDTree : Generator<geometry::prim::Polygon>
      {
        typedef geometry::prim::Polygon Polygon;
        TOMO_TEST_GENERATOR_DECL(KDTree,Polygon)

        ObjectList generate() const
        {
          using geometry::base::Point2f;
          ObjectList _objs;
          _objs.push_back(create::circleWith3Holes(Point2f(0.5,0.5),0.4,32));
          return _objs;
        }

        void draw(Wrapper& _wrapper, const Object& _obj)
        {
          _wrapper.draw(_obj,Magick::Color("green"));
        }
      };
    }
  }
}


TOMO_TEST_CASE( KDTree, 1024 )
{
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::base::Point2f;
  using tomo::geometry::prim::Segment;
  using tomo::geometry::Model2f;
  using tomo::geometry::kd::nearest::SegmentPointTree;
  using tomo::geometry::kd::nearest::SegmentPointTreeBuildPolicy;

  std::vector<Segment> _segments;
  _obj.fetchSegments(_segments);
  _w.drawEndings(true);
  _w.draw(_segments,Color("red"));
  writeImage();

/*  SegmentTree _segmentTree(_segments);
  _w.drawKDTree(_segmentTree,Color("white"));
  writeImage();
*/
  _w.clear();
  _w.draw(_segments,Color("red"));
  SegmentPointTree _segmentPointTree(_segments);
  _w.drawKDTree(_segmentPointTree,Color("white"));
  writeImage();
}
TOMO_TEST_CASE_END

