#include <Magick++.h>
#include <tomo/slicing/Slice.hpp>
#include <tomo/geometry/aux/KDTree.hpp>
#include "tomo/geometry/aux/Compound.hpp"
#include "tomo/geometry/prim/Vertex.hpp"
#include "tomo/geometry/prim/Polygon.hpp"
#include "tomo/slicing/LineSegmentPlane.hpp"

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
    using tg::base::Point3f;
    using tg::aux::Bounds;
    using tg::aux::KDTree;
    using tg::aux::KDNode;
    using tg::aux::Compound;
    using tg::aux::Ray2f;

    struct Wrapper
    {
      Wrapper(Magick::Image& _image) : vertexWidth_(4.0), drawEndings_(false), image_(_image) 
      {
        image_.fillColor("none");
      }

/*
      template<typename STATE, typename NODE>
      struct KDTreeVisitor
      {
        virtual void innerNode() = 0;
        virtual void leafNode() = 0;

      };

      struct KDTreeState
      {

      };

      struct KDTreeDrawState : KDTreeState
      {
        TBD_PROPERTY_REF(KDNODE*,node);
        TBD_PROPERTY_REF(Bounds,bounds);
      };

      template<typename STATE>
      struct KDTreeDrawVisitor : 
        KDTreeVisitor< KDTreeDrawState<KDTree<2,float>::Node> >
      {
        SplitPlaneIntersect innerNode()
        {
          SplitPlaneIntersect _result;
          Bounds<DIMENSIONS,SCALAR> _left, _right;
          tg::base::Axis _axis = state_.node()->inner_.axis();
          SCALAR _splitPos = state_.node()->inner_.splitPos();
          state_.bounds().split(_splitPos,_axis,_left,_right);

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
        }

        bool leafNode() { return false; }
      
        TBD_PROPERTY(Image&,image);
        TBD_PROPERTY(Magick::Color,color);
        
      private:
        STATE state_;
      };
*/
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




        //KDTreeDrawVisitor _visitor(image_,_color);
        //_compound.kdTree().traverse<KDTreeDrawVisitor>();
        
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

    private:
      Magick::Image& image_;
    };

  }
}
