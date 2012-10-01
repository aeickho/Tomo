#include <Magick++.h>
#include <tomo/slicing/Slice.hpp>
#include <tomo/geometry/aux/KDTree.hpp>
#include "tomo/geometry/aux/Compound.hpp"
#include "tomo/geometry/prim/Vertex.hpp"
#include "tomo/slicing/LineSegmentPlane.hpp"

#include <boost/foreach.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/register/multi_linestring.hpp>
#include <vector>


typedef boost::geometry::model::linestring< tomo::geometry::prim::PointXYf > LineString;
typedef std::vector<LineString> MultiLineString;
BOOST_GEOMETRY_REGISTER_MULTI_LINESTRING(MultiLineString);

namespace tomo
{
  namespace magick
  {
    namespace tg = tomo::geometry;
    using tg::prim::Polygon;
    using tg::prim::MultiPolygon;
    using tg::prim::LineSegment;
    using tg::base::Point2f;
    using tg::base::Point3f;
    using tg::aux::Bounds;
    using tg::aux::KDTree;
    using tg::aux::KDNode;
    using tg::aux::Compound;
    using tg::aux::Ray2f;

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
      typedef CoordinateWrapper<tg::prim::PointXYf,Magick::Coordinate> BoostToMagickCoord;

      Wrapper(Magick::Image& _image) : vertexWidth_(4.0), drawEndings_(false), image_(_image) 
      {
        image_.fillColor("none");
      }

      void draw(const Polygon& _polygon, Magick::Color _color)
      {
        BoostToMagickCoord _coordWrap(image_);
        /// XXX: Improve this...
        _coordWrap.resX(1);
        _coordWrap.resY(1);

        std::list<Magick::Coordinate> _coords;
        _coordWrap.coords_ = &_coords;

        image_.fillColor("none");
        boost::geometry::for_each_point( _polygon(), _coordWrap );
        image_.strokeColor(Magick::Color(_color)); // Outline color
        _coords.push_back(_coords.front());
        image_.draw( Magick::DrawablePolyline( _coords )) ;
      }

      void draw(const MultiPolygon _multiPolygon, Magick::Color _color)
      {
        BOOST_FOREACH( const Polygon& _polygon, _multiPolygon)
        draw(_polygon,_color);
      }

      void draw(const LineString& _lineString, Magick::Color _color)
      {
        BoostToMagickCoord _coordWrap(image_);
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
      void drawKDNode(const KDTree<PRIMITIVE,DIMENSIONS,SCALAR>& _kdTree,
                 const KDNode<PRIMITIVE,DIMENSIONS,SCALAR>* _node,
                 Bounds<DIMENSIONS,SCALAR> _bounds)
      {
        if (_node->isLeaf()) return;

        Bounds<DIMENSIONS,SCALAR> _left, _right;
        tg::base::Axis _axis = _node->inner_.axis();
        SCALAR _splitPos = _node->inner_.splitPos();
        _bounds.split(_splitPos,_axis,_left,_right);

        switch (_axis)
        {
          case tg::base::X:
          image_.draw( Magick::DrawableLine(_splitPos,_bounds.min().y(),_splitPos,_bounds.max().y()) );
          break;
          case tg::base::Y:
          image_.draw( Magick::DrawableLine(_bounds.min().x(),_splitPos,_bounds.max().x(),_splitPos) );
          break;
        default:
          break;
        }

        drawKDNode(_kdTree,_kdTree.node(_node->inner_.left()),_left);
        drawKDNode(_kdTree,_kdTree.node(_node->inner_.right()),_right);
      }

      template <typename PRIMITIVE, int DIMENSIONS , typename SCALAR>
      void drawKDTree(const Compound<PRIMITIVE,DIMENSIONS,SCALAR>& _compound, 
                        Magick::Color _color)
      {
        image_.strokeColor(_color);
        drawKDNode<PRIMITIVE,DIMENSIONS,SCALAR>(_compound.kdTree(),&_compound.kdTree().root(),_compound.bounds());
      }

      template <typename PRIMITIVE, int DIMENSIONS , typename SCALAR>
      void draw(const Compound<PRIMITIVE,DIMENSIONS,SCALAR>& _compound, Magick::Color _color)
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
      

      void draw(const tg::prim::Vertex2f& _vertex, Magick::Color _color)
      {
        image_.strokeColor(_color);
        tg::base::Point2us _p(_vertex.v.x(),_vertex.v.y());  
        image_.draw( Magick::DrawableCircle( _p.x(),_p.y(),vertexWidth_+_p.x(),_p.y() ));
      }

      void draw(const tg::base::Point2f _point, Magick::Color _color)
      {
        tg::prim::Vertex2f _v(_point);
        draw(_v,_color);
      }

      void draw(const tg::aux::Ray2f& _ray, Magick::Color _color)
      {
        image_.strokeColor(_color);

        tg::base::Point2f _pNear = _ray.org() + _ray.tNear() * _ray.dir();
        tg::base::Point2f _pFar = _ray.org() + _ray.tFar() * _ray.dir();
        
        tg::base::Point2us _f(_ray.org().x(),_ray.org().y()),
                           _b(_pFar.x(),_pFar.y());

        image_.draw( Magick::DrawableLine(_f.x(),_f.y(),_b.x(),_b.y()) );
        draw( _pNear, Magick::Color("yellow") );
        draw( _pFar, Magick::Color("blue") );
      }

      void draw(const LineSegment& _segment, Magick::Color _color)
      {
        Point2f _b = _segment.back();
        Point2f _f = _segment.front();
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
