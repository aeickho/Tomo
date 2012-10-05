#include <Magick++.h>
#include <tomo/slicing/Slice.hpp>
#include <tomo/geometry/aux/KDTree.hpp>
#include "tomo/geometry/aux/Compound.hpp"
#include "tomo/geometry/prim/Vertex.hpp"
#include "tomo/geometry/prim/Polygon.hpp"
#include "tomo/slicing/LineSegmentPlane.hpp"

#include "tomo/geometry/aux/Visitor.hpp"

#include <boost/foreach.hpp>
#include <vector>

namespace tomo
{
  namespace magick
  {
    namespace tg = tomo::geometry;
    using tg::prim::LineSegment;
    using tg::prim::Ring;
    using tg::prim::Polygon;
    using tg::base::Point2f;
    using tg::base::Vec2f;
    using tg::base::Point3f;
    using tg::aux::Bounds;
    using tg::aux::KDTree;
    using tg::aux::KDNode;
    using tg::aux::KDTreeVisitor;
    using tg::aux::KDTreeState;
    using tg::aux::Compound;
    using tg::aux::Ray2f;

    struct Wrapper
    {
      Wrapper(Magick::Image& _image) : vertexWidth_(4.0), drawEndings_(false), image_(_image) 
      {
        image_.fillColor("none");
      }


      template<typename KDTREE>
      struct KDTreeDrawState 
      {
        typedef typename KDTREE::bounds_type bounds_type;
        typedef typename KDTREE::Node Node;
        TBD_PROPERTY_REF(bounds_type,bounds);
        TBD_PROPERTY(const Node*,node);
      };

      template<typename KDTREE>
      struct KDTreeDrawVisitor  
      {
        KDTreeDrawVisitor(const KDTREE& _kdTree, Magick::Image& _image, Magick::Color _color) :
          color_(_color),
          image_(_image),
          kdTree_(_kdTree)
        {
          state_.node(_kdTree.root());
          state_.bounds(_kdTree.bounds_);
        }

        typedef KDTreeDrawState<KDTREE> State;
        typedef typename KDTREE::bounds_type bounds_type;
        typedef typename KDTREE::scalar_type scalar_type;

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
          tg::base::Axis _axis = state_.node()->inner_.axis();
          scalar_type _splitPos = state_.node()->inner_.splitPos();
          state_.bounds().split(_splitPos,_axis,_left,_right);
          
          unsigned _alphaValue = 10000+55000*state_.bounds().radius()/kdTree_.bounds_.radius();
          image_.strokeColor(Magick::Color(_alphaValue,_alphaValue,_alphaValue));
          
          switch (_axis)
          {
            case tg::base::X:
            image_.draw( Magick::DrawableLine(_splitPos,
                                              state_.bounds().min().y(),
                                              _splitPos,
                                              state_.bounds().max().y()) );
            break;
            case tg::base::Y:
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
        bool leaf() { return false; }
    
        TBD_PROPERTY(Magick::Color,color);
        TBD_PROPERTY_REF(State,state);
      private:
        Magick::Image& image_;
        const KDTREE& kdTree_;
      };

      template <typename PRIMITIVE>
      void draw(const KDTree<PRIMITIVE>& _kdTree, 
                        Magick::Color _color)
      {
        KDTreeDrawVisitor< KDTree<PRIMITIVE> > _visitor(_kdTree,image_,_color);  
        _kdTree.traversal(_visitor);
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
     
      void draw(const tg::aux::Bounds2f& _bounds, Magick::Color _color)
      {
        image_.strokeColor(_color);
        image_.draw( Magick::DrawableRectangle( 
              _bounds.min().x(),
              _bounds.min().y(),
              _bounds.max().x(),
              _bounds.max().y() ));

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

        if (drawNormals_)
        {
          image_.strokeColor("green");
          Vec2f _c = 0.5*(_b.vec() + _f.vec());
          Vec2f _n = _segment.normal();
          image_.draw( Magick::DrawableLine( _c.x(),_c.y(),_c.x()+_n.x(),_c.y()+_n.y()) );
        }
      }

      void draw(const Ring& _ring, Magick::Color _color)
      {
        draw<LineSegment,2,float>(_ring,_color);
      }

      void draw(const Polygon& _polygon, Magick::Color _color)
      {
        draw<Ring,2,float>(_polygon,_color);
      }

      TBD_PROPERTY(float,vertexWidth);
      TBD_PROPERTY(bool,drawEndings);
      TBD_PROPERTY(bool,drawNormals);

    private:
      Magick::Image& image_;
    };

  }
}
