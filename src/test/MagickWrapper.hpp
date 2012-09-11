#include <Magick++.h>
#include <tomo/Slice.hpp>
#include <tomo/KDTree.hpp>
#include "tomo/Compound.hpp"
#include "tomo/Vertex.hpp"
#include "tomo/LineSegment.hpp"

#include <boost/foreach.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/register/multi_linestring.hpp>
#include <vector>


typedef boost::geometry::model::linestring< tomo::PointXYf > LineString;
typedef std::vector<LineString> MultiLineString;
BOOST_GEOMETRY_REGISTER_MULTI_LINESTRING(MultiLineString);

namespace tomo
{
  namespace magick
  {
    template <typename Point, typename Coord>
    struct CoordinateWrapper
    {
      CoordinateWrapper(Magick::Image& _image)
      {
        resX(_image.columns());
        resY(_image.rows());
      }

      CoordinateWrapper(int _resX, int _resY) : resX_(_resX), resY_(_resY)
      {
      }

      void operator()(const Point& p)
      {
        using boost::geometry::get;
        coords_->push_back(Coord(int(get<0>(p)*resX_),int(get<1>(p)*resY_)));
      }

      std::list<Coord>* coords_;
      TBD_PROPERTY(int,resX);
      TBD_PROPERTY(int,resY);
    };

    struct Wrapper
    {
      Wrapper(Magick::Image& _image) : image_(_image) 
      {
        image_.fillColor("none");
      }

      void draw(const tomo::Polygon& _polygon, Magick::Color _color)
      {
        CoordinateWrapper<tomo::PointXYf,Magick::Coordinate> _coordWrap(image_);
        /// XXX: Improve this...
        _coordWrap.resX(1);
        _coordWrap.resY(1);

        std::list<Magick::Coordinate> _coords;
        _coordWrap.coords_ = &_coords;

        image_.fillColor("none");
        boost::geometry::for_each_point( _polygon, _coordWrap );
        image_.strokeColor(Magick::Color(_color)); // Outline color
        image_.draw( Magick::DrawablePolyline( _coords )) ;
      }

      void draw(const tomo::MultiPolygon _multiPolygon, Magick::Color _color)
      {
        BOOST_FOREACH( const tomo::Polygon& _polygon, _multiPolygon)
        draw(_polygon,_color);
      }

      void draw(const LineString& _lineString, Magick::Color _color)
      {
        CoordinateWrapper<tomo::PointXYf,Magick::Coordinate> _coordWrap(image_);
        std::list<Magick::Coordinate> _coords;
        _coordWrap.coords_ = &_coords;

        boost::geometry::for_each_point( _lineString, _coordWrap );

        image_.strokeColor(Magick::Color(_color)); // Outline color
        image_.draw( Magick::DrawablePolyline( _coords )) ;
      }

      void draw(const MultiLineString& _multiLineString, Magick::Color _color)
      {
        BOOST_FOREACH( const LineString& _lineString, _multiLineString)
          draw(_lineString,_color);
      }

      template <typename PRIMITIVE, int DIMENSIONS , typename SCALAR>
      void drawKDNode(const tomo::KDTree<PRIMITIVE,DIMENSIONS,SCALAR>& _kdTree,
                 const tomo::KDNode<PRIMITIVE,DIMENSIONS,SCALAR>* _node,
                 tomo::Bounds<DIMENSIONS,SCALAR> _bounds)
      {
        if (_node->isLeaf()) return;

        tomo::Bounds<DIMENSIONS,SCALAR> _left, _right;
        tomo::Axis _axis = _node->inner_.axis();
        SCALAR _splitPos = _node->inner_.splitPos();
        _bounds.split(_splitPos,_axis,_left,_right);

        switch (_axis)
        {
        case tomo::X:
          image_.draw( Magick::DrawableLine(_splitPos,_bounds.min().y(),_splitPos,_bounds.max().y()) );
          break;
        case tomo::Y:
          image_.draw( Magick::DrawableLine(_bounds.min().x(),_splitPos,_bounds.max().x(),_splitPos) );
          break;
        default:
          break;
        }

        drawKDNode(_kdTree,_kdTree.node(_node->inner_.left()),_left);
        drawKDNode(_kdTree,_kdTree.node(_node->inner_.right()),_right);
      }

      template <typename PRIMITIVE, int DIMENSIONS , typename SCALAR>
      void drawKDTree(const tomo::Compound<PRIMITIVE,DIMENSIONS,SCALAR>& _compound, 
                        Magick::Color _color)
      {
        image_.strokeColor(_color);
        drawKDNode<PRIMITIVE,DIMENSIONS,SCALAR>(_compound.kdTree(),&_compound.kdTree().root(),_compound.bounds());
      }

      template <typename PRIMITIVE, int DIMENSIONS , typename SCALAR>
      void draw(const tomo::Compound<PRIMITIVE,DIMENSIONS,SCALAR>& _compound, Magick::Color _color)
      {
        BOOST_FOREACH( const PRIMITIVE& _primitive, _compound.objs() )
          draw(_primitive,_color);
      }

      template <typename PRIMITIVE>
      void draw(const std::vector<PRIMITIVE*> _primitives, Magick::Color _color)
      {
        BOOST_FOREACH( PRIMITIVE* _primitive, _primitives )
          draw(*_primitive,_color);
      }
      

      void draw(const tomo::Vertex2f& _vertex, Magick::Color _color)
      {
        image_.strokeColor(_color);
        Point2us _p(_vertex.v.x(),_vertex.v.y());  
        image_.draw( Magick::DrawableCircle( _p.x(),_p.y(),vertexWidth_+_p.x(),_p.y() ));
      }

      void draw(const tomo::LineSegment& _segment, Magick::Color _color)
      {
        tomo::Point2f _b = _segment.back();
        tomo::Point2f _f = _segment.front();
        image_.strokeColor(_color);
        image_.draw( Magick::DrawableLine(_f.x(),_f.y(),_b.x(),_b.y()) );

        if (drawEndings_)
        {
          image_.strokeColor("yellow");
          image_.draw( Magick::DrawableCircle(_f.x(),_f.y(),4+_f.x(),_f.y() ));
          image_.strokeColor("blue");
          image_.draw( Magick::DrawableCircle(_b.x(),_b.y(),7+_b.x(),_b.y() ));
        }
      }

      TBD_PROPERTY(float,vertexWidth);
      TBD_PROPERTY(bool,drawEndings);

    private:
      Magick::Image& image_;
    };

  }
}
