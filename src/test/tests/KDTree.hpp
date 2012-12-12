#pragma once

#include "tomo/geometry/kd/nearest/Tree.hpp"
#include "tomo/geometry/kd/object/Tree.hpp"
#include "tomo/geometry/kd/nearest/Visitor.hpp"

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
          _objs.push_back(create::circleWith3Holes(Point2f(0.5,0.5),0.4,7));
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


TOMO_TEST_CASE( KDTree, 512 )
{
  using tomo::geometry::base::Vec2f;
  using tomo::geometry::base::Point2f;
  using tomo::geometry::prim::Segment;
  using tomo::geometry::Model2f;
  using namespace tomo::geometry::kd;

  std::vector<Segment> _segments;
  _obj.fetchSegments(_segments);
  _w.drawEndings(true);
  _w.draw(_segments,Color("red"));
  writeImage();

  object::Tree<Segment> _segmentTree(_segments);
  _w.drawKDTree(_segmentTree,Color("white"));
  writeImage();

  _w.clear();
  _w.draw(_segments,Color("red"));
  nearest::Tree<Segment> _segmentPointTree(_segments);
  _w.drawKDTree(_segmentPointTree,Color("white"));
  writeImage();

  int i = 0;
  for (Segment& _segment : _segments)
  {
  _w.clear();
  _w.draw(_segments,Color("red"));
  _w.drawEndings(false);

  //Segment& _segment = _segments[1];
  nearest::Nearest<Segment> _nearest(_segmentPointTree);
  _nearest.find(_segment);
  const Segment* _nearestSegment = _nearest.container().getNearest();

  std::cout << i << ": Nearest distance: " << _nearest.container().minDistance() << std::endl;
  std::cout << i << ": Distance to next seg: " << algorithm::sqrDistance(_segment,_segments[(i+1) % _segments.size()]) << std::endl;

  _w.draw(_segment,Color("yellow"));

  if (_nearestSegment) _w.draw(*_nearestSegment,Color("white"));
  writeImage();
  ++i;
  }
}
TOMO_TEST_CASE_END

