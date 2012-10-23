#pragma once

#include <Magick++.h>
#include "tomo/draw/DrawInterface.hpp"
#include <boost/foreach.hpp>

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
        typedef geometry::base::Point3f Point3f;

        Wrapper(Magick::Image& _image) :
          image_(_image),
          drawEndings_(false),
          drawNormals_(false),
          vertexWidth_(4.0)
        {
          image_.fillColor("none");
        }

        ///@todo Make TreeDrawVisitor generic
        template<typename KDTREE>
        struct TreeDrawVisitor
        {
          typedef typename KDTREE::bounds_type bounds_type;
          typedef typename KDTREE::scalar_type scalar_type;
          typedef typename KDTREE::Node Node;

          struct State
          {
            TBD_PROPERTY_REF(bounds_type,bounds);
            TBD_PROPERTY(const Node*,node);
          };

          TreeDrawVisitor(const KDTREE& _kdTree, Magick::Image& _image, Magick::Color _color) :
            color_(_color),
            image_(_image),
            kdTree_(_kdTree)
          {
            state_.node(_kdTree.root());
            state_.bounds(_kdTree.bounds_);
          }

          /// Define what to do in the root node (draw bounding box)
          bool root()
          {
            image_.strokeColor(color_);
            image_.draw( Magick::DrawableRectangle(
                           kdTree_.bounds_.min().x(),
                           kdTree_.bounds_.min().y(),
                           kdTree_.bounds_.max().x(),
                           kdTree_.bounds_.max().y() ));
            return true;
          }

          void traverseLeft(State& _state)
          {
            _state.node(kdTree_.node(state_.node()->inner_.left()));
          }

          void traverseRight(State& _state)
          {
            _state.node(kdTree_.node(state_.node()->inner_.right()));
          }

          /// Define what to do in an inner node (draw split plane)
          bool inner(State& _nextState)
          {
            bounds_type _left, _right;
            geometry::base::Axis _axis = state_.node()->inner_.axis();
            scalar_type _splitPos = state_.node()->inner_.splitPos();
            state_.bounds().split(_splitPos,_axis,_left,_right);

            unsigned _alphaValue = 10000+55000*state_.bounds().radius()/kdTree_.bounds_.radius();
            image_.strokeColor(Magick::Color(_alphaValue,_alphaValue,_alphaValue));

            switch (_axis)
            {
            case geometry::base::X:
              image_.draw( Magick::DrawableLine(_splitPos,
                                                state_.bounds().min().y(),
                                                _splitPos,
                                                state_.bounds().max().y()) );
              break;
            case geometry::base::Y:
              image_.draw( Magick::DrawableLine(state_.bounds().min().x(),
                                                _splitPos,
                                                state_.bounds().max().x(),
                                                _splitPos) );
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
          Magick::Image& image_;
          const KDTREE& kdTree_;
        };

        template <typename PRIMITIVE>
        void draw(const geometry::kd::Tree<PRIMITIVE>& _kdTree,
                  Magick::Color _color)
        {
          TreeDrawVisitor< geometry::kd::Tree<PRIMITIVE> > _visitor(_kdTree,image_,_color);
          _kdTree.traversal(_visitor);
        }

        template <typename PRIMITIVE>
        void draw(const geometry::comp::Compound<PRIMITIVE>& _compound, Magick::Color _color)
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

        void draw(const bounds_type& _bounds, Magick::Color _color)
        {
          image_.strokeColor(_color);
          image_.draw( Magick::DrawableRectangle(
                         _bounds.min().x(),
                         _bounds.min().y(),
                         _bounds.max().x(),
                         _bounds.max().y() ));

        }

        void draw(const geometry::prim::Vertex2f& _vertex, Magick::Color _color)
        {
          image_.strokeColor(_color);
          point_type _p(_vertex.v().x(),_vertex.v().y());
          image_.draw( Magick::DrawableCircle( _p.x(),_p.y(),vertexWidth_+_p.x(),_p.y() ));

          if (drawNormals_)
          {
            image_.draw( Magick::DrawableLine(_p.x(),_p.y(),
                                              _p.x() + _vertex.n().x(),
                                              _p.y() + _vertex.n().y()));
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

          image_.draw( Magick::DrawableLine(_f.x(),_f.y(),_b.x(),_b.y()) );
          draw( _pNear, Magick::Color("yellow") );
          draw( _pFar, Magick::Color("blue") );
        }

        void draw(const Segment& _segment, Magick::Color _color)
        {
          point_type _b = _segment[0];
          point_type _f = _segment[1];
          image_.strokeColor(_color);
          image_.draw( Magick::DrawableLine(_f.x(),_f.y(),_b.x(),_b.y()) );

          if (drawEndings_)
          {
            image_.strokeColor("yellow");
            image_.draw( Magick::DrawableCircle(_f.x(),_f.y(),4+_f.x(),_f.y() ));
            image_.strokeColor("blue");
            image_.draw( Magick::DrawableCircle(_b.x(),_b.y(),7+_b.x(),_b.y() ));
          }

          if (drawNormals_)
          {
            image_.strokeColor("green");
            vec_type _c = 0.5*(_b.vec() + _f.vec());
            vec_type _n = _segment.normal();
            image_.draw( Magick::DrawableLine( _c.x(),_c.y(),_c.x()+_n.x(),_c.y()+_n.y()) );
          }
        }

        template<class PRIMITIVE>
        void draw(const std::vector<PRIMITIVE> _primitives, Magick::Color _color)
        {
          BOOST_FOREACH( const PRIMITIVE& _primitive, _primitives )
          draw(_primitive,_color);
        }

        void draw(const std::vector<Segment>& _lineSegments, Magick::Color _color)
        {
          draw<Segment>(_lineSegments,_color);
        }

        void draw(const Ring& _ring, Magick::Color _color)
        {
          std::vector<Segment> _lineSegments;
          _ring.fetchSegments(_lineSegments);
          draw(_lineSegments,_color);
        }

        void draw(const Polygon& _polygon, Magick::Color _color)
        {
          std::vector<Segment> _lineSegments;
          _polygon.fetchSegments(_lineSegments);
          draw(_lineSegments,_color);
        }

        void clear()
        {
          image_ = Magick::Image( Magick::Geometry(image_.columns(),image_.rows()), Magick::Color("black") );
        }

        Magick::Image& image_;

        TBD_PROPERTY(bool,drawEndings);
        TBD_PROPERTY(bool,drawNormals);
        TBD_PROPERTY(float,vertexWidth);
      };

    }
  }
}
