#pragma once

#include <Magick++.h>
#include "tomo/draw/DrawInterface.hpp"

namespace tomo
{
  namespace draw
  {
    namespace magick
    {
      struct Wrapper : DrawInterface<geometry::Model2f>
      {
        typedef geometry::prim::Ring Ring;
        typedef geometry::prim::Segment Segment;
        typedef geometry::prim::Polygon Polygon;
        typedef geometry::prim::MultiPolygon MultiPolygon;
        typedef geometry::prim::LineString LineString;
        typedef geometry::prim::MultiLineString MultiLineString;
        typedef geometry::base::Point3f Point3f;

        Wrapper(int _width, int _height) :
          drawEndings_(false),
          drawLineStringEndings_(false),
          drawNormals_(false),
          drawBounds_(false),
          vertexWidth_(4.0),
          scale_(1,1),
          offset_(0,0)
        {
          clear(_width,_height);
        }

        ///@todo Make TreeDrawVisitor generic
        template<typename KDTREE>
        struct TreeDrawVisitor
        {
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::node_type node_type;

          struct State
          {
            TBD_PROPERTY_REF(bounds_type,bounds);
            TBD_PROPERTY(const node_type*,node);
          };

          TreeDrawVisitor(const KDTREE& _kdTree, Wrapper& _wrapper, Magick::Color _color) :
            color_(_color),
            wrapper_(_wrapper),
            kdTree_(_kdTree)
          {
            state_.node(&_kdTree.nodes().getRoot());
            state_.bounds(_kdTree.bounds());
          }

          /// Define what to do in the root node (draw bounding box)
          bool root()
          {
            wrapper_.image().strokeColor(color_);
            wrapper_.drawRect(kdTree_.bounds().min(),kdTree_.bounds().max());
            return true;
          }

          void traverseLeft(State& _state)
          {
            _state.node(&kdTree_.nodes().getNode(state_.node()->inner().left()));
          }

          void traverseRight(State& _state)
          {
            _state.node(&kdTree_.nodes().getNode(state_.node()->inner().right()));
          }

          /// Define what to do in an inner node (draw split plane)
          bool inner(State& _nextState)
          {
            bounds_type _left, _right;
            geometry::base::Axis _axis = state_.node()->inner().axis();
            scalar_type _splitPos = state_.node()->inner().splitPos();
            state_.bounds().split(_splitPos,_axis,_left,_right);

            unsigned _alphaValue = 10000+55000*state_.bounds().radius()/kdTree_.bounds().radius();
            wrapper_.image().strokeColor(Magick::Color(_alphaValue,_alphaValue,_alphaValue));

            switch (_axis)
            {
            case geometry::base::X:
              wrapper_.drawLine(point_type(_splitPos,state_.bounds().min().y()),point_type(_splitPos,state_.bounds().max().y()));
              break;
            case geometry::base::Y:
              wrapper_.drawLine(point_type(state_.bounds().min().x(),_splitPos),point_type(state_.bounds().max().x(),_splitPos));
              break;
            default:
              break;
            }

            traverseRight(_nextState);
            _nextState.bounds(_right);
            traverseLeft(state_);
            state_.bounds(_left);
            return true;
          }

          /// Define what to do in a leaf node (nothingg)
          bool leaf()
          {
            return false;
          }

          TBD_PROPERTY(Magick::Color,color);
          TBD_PROPERTY_REF(State,state);
        private:
          Wrapper& wrapper_;
          const KDTREE& kdTree_;
        };

        template <typename KDTREE>
        void drawKDTree(const KDTREE& _kdTree,
                  Magick::Color _color)
        {
          TreeDrawVisitor< KDTREE > _visitor(_kdTree,*this,_color);
          _kdTree.traversal(_visitor);
        }

        template <typename PRIMITIVE>
        void draw(const std::vector<PRIMITIVE*> _primitives, Magick::Color _color)
        {
          for ( PRIMITIVE* _primitive : _primitives )
            draw(*_primitive,_color);
        }

        void draw(const bounds_type& _bounds, Magick::Color _color)
        {
          image_.strokeColor(_color);
          drawRect(_bounds.min(),_bounds.max());
        }

        void draw(const geometry::prim::Vertex2f& _vertex, Magick::Color _color)
        {
          image_.strokeColor(_color);
          point_type _p(_vertex.v().x(),_vertex.v().y());
          
          drawCircle(_p,vertexWidth_);
          if (drawNormals_)
          {
            drawLine(_p,_p + _vertex.n());
          }
        }

        void draw(const point_type _point, Magick::Color _color)
        {
          geometry::prim::Vertex2f _v(_point);
          draw(_v,_color);
        }

        void draw(const ray_type& _ray, Magick::Color _color)
        {
          image_.strokeColor(_color);

          point_type _pNear = _ray.org() + _ray.tNear() * _ray.dir();
          point_type _pFar = _ray.org() + _ray.tFar() * _ray.dir();
          point_type _f(_ray.org().x(),_ray.org().y()),
                     _b(_pFar.x(),_pFar.y());
          drawLine(_f,_b);

          draw( _pNear, Magick::Color("yellow") );
          draw( _pFar, Magick::Color("blue") );
        }

        void draw(const Segment& _segment, Magick::Color _color)
        {
          point_type _b = _segment[0];
          point_type _f = _segment[1];
          image_.strokeColor(_color);
          drawLine(_f,_b);

          if (drawEndings_)
          {
            image_.strokeColor("yellow");
            drawCircle(_f,4);
            image_.strokeColor("blue");
            drawCircle(_b,7);
          }

          if (drawNormals_)
          {
            image_.strokeColor("green");
            vec_type _c = 0.5*(_b.vec() + _f.vec());
            vec_type _n = _segment.normal();
            drawLine(_c,_c+_n);
          }
        }

        template<class PRIMITIVE>
        void draw(const std::vector<PRIMITIVE> _primitives, Magick::Color _color)
        {
          for( const PRIMITIVE& _primitive : _primitives )
            draw(_primitive,_color);
        }

        void draw(const std::vector<Segment>& _lineSegments, Magick::Color _color)
        {
          draw<Segment>(_lineSegments,_color);
        }

        void draw(const LineString& _lineString, Magick::Color _color)
        {
          std::vector<Segment> _lineSegments;
          _lineString.fetchSegments(_lineSegments);
          draw(_lineSegments,_color);
          if (drawLineStringEndings())
          {
            image_.strokeColor("yellow");
            drawCircle(_lineSegments.front()[0],4);
            image_.strokeColor("blue");
            drawCircle(_lineSegments.back()[1],7);
          }
        }

        void draw(const MultiLineString& _multiLineString, Magick::Color _color)
        {
          for (const LineString& _lineString : _multiLineString)
            draw(_lineString,_color);
        }

        void draw(const Ring& _ring, Magick::Color _color)
        {
          std::vector<Segment> _lineSegments;
          _ring.fetchSegments(_lineSegments);
          draw(_lineSegments,_color);
          if (drawBounds()) draw(_ring.bounds(),_color);
        }

        void draw(const Polygon& _polygon, Magick::Color _color)
        {
          std::vector<Segment> _lineSegments;
          _polygon.fetchSegments(_lineSegments);
          draw(_lineSegments,_color);
          if (drawBounds()) draw(_polygon.bounds(),_color);
        }

        void draw(const MultiPolygon& _polygon, Magick::Color _color)
        {
          std::vector<Segment> _lineSegments;
          _polygon.fetchSegments(_lineSegments);
          draw(_lineSegments,_color);
          if (drawBounds()) draw(_polygon.bounds(),_color);
        }

        void clear()
        {
          clear(image_.columns(),image_.rows());
        }

        void clear(int _width, int _height)
        {
          image_ = Magick::Image( Magick::Geometry(_width,_height), Magick::Color("black") );
          image_.fillColor(Magick::Color("none"));
        }

        TBD_PROPERTY(bool,drawEndings);
        TBD_PROPERTY(bool,drawLineStringEndings);
        TBD_PROPERTY(bool,drawNormals);
        TBD_PROPERTY(bool,drawBounds);
        TBD_PROPERTY(float,vertexWidth);
        TBD_PROPERTY_REF(vec_type,scale);
        TBD_PROPERTY_REF(vec_type,offset);
        TBD_PROPERTY_REF(Magick::Image,image);

      private:

        void transform(point_type& _p) 
        {
          _p = offset_ + _p.vec() * scale_;
        }

        void drawCircle(point_type _p, scalar_type _r)
        {
          transform(_p);
          image_.fillColor(Magick::Color("none"));
          image_.draw( Magick::DrawableCircle(_p.x(),_p.y(),_r+_p.x(),_p.y()));
        }

        void drawLine(point_type _a, point_type _b)
        {
          transform(_a);
          transform(_b);
          image_.draw( Magick::DrawableLine(_a.x(),_a.y(),_b.x(),_b.y()) );
        }

        void drawRect(point_type _a, point_type _b)
        {
          transform(_a);
          transform(_b);
          image_.draw(Magick::DrawableRectangle(_a.x(),_a.y(),_b.x(),_b.y()));
        }
      };
    }
  }
}
