#pragma once

#include "tomo/geometry/prim/BoundingBox.hpp"

#include <vector>
#include <stack>

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      template <typename PRIMITIVE, int DIMENSIONS = 3, typename SCALAR = base::DEFAULT_TYPE>
      union KDNode
      {
        typedef SCALAR scalar_type;
        typedef std::vector<PRIMITIVE*> PrimCont;
        typedef Ray<DIMENSIONS,scalar_type> ray_type;
        typedef base::Vec<DIMENSIONS,scalar_type> vector_type;

        bool isLeaf() const
        {
          return !inner_.leafFlag();
        }

        /// Inner node
        struct Inner
        {
          typedef std::vector<KDNode> NodeCont;

          inline bool leafFlag() const
          {
            return data_ & (1 << 31);
          }
          inline base::Axis axis() const
          {
            return base::Axis(data_ & 3);
          }

          /// Insert children
          inline void setup(NodeCont& _nodes, base::Axis _axis, scalar_type _splitPos)
          {
            data_ = (0x7FFFFFFC & (_nodes.size() << 2)) | (1 << 31) | _axis;
            splitPos_ = _splitPos;
          }

          inline unsigned left() const
          {
            return (data_ & 0x7FFFFFFC) >> 2;
          }
          inline unsigned right() const
          {
            return ((data_ & 0x7FFFFFFC) >> 2) + 1;
          }

          inline scalar_type splitPos() const
          {
            return splitPos_;
          }

        private:
          unsigned int data_;
          scalar_type splitPos_;
        };

        /// Leaf node
        struct Leaf
        {
          typedef std::vector<PRIMITIVE*> PrimCont;

          /// Insert Primitive pointers from _src into _dest
          inline void insert(const PrimCont& _src, PrimCont& _dest)
          {
            // Make leaf node
            offset_ = _dest.size();
            size_ = _src.size();
            _dest.resize(offset_ + size_);
            for (unsigned i = 0; i < size_; i++)
              _dest[offset_+i] = _src[i];
          }

          inline bool intersect(ray_type& _ray, const PrimCont& _primLists, scalar_type _tNear, scalar_type _tFar, vector_type* _normal = NULL) const
          {
            bool _found = false;
            for (unsigned i = offset_; i < offset_ + size_; i++)
            {
              PRIMITIVE* _prim = _primLists[i];
              _found |= _prim->intersect(_ray,_tNear,_tFar,_normal);
            }
            return _found;
          }

          inline PrimCont primitives(const PrimCont& _primLists) const
          {
            PrimCont _primList;
            _primList.resize(size_);
            for (unsigned i = offset_, j = 0; i < offset_ + size_; i++, j++)
              _primList[j] = _primLists[i];
            return _primList;
          }

        private:
          unsigned int size_;
          unsigned int offset_;
        };

        Inner inner_;
        Leaf leaf_;
      };

      template <typename PRIMITIVE,
               int DIMENSIONS = 3,
               typename SCALAR = base::DEFAULT_TYPE>
      struct KDTree
      {
        /// Node type
        typedef KDNode<PRIMITIVE,DIMENSIONS,SCALAR> Node;
        typedef typename KDNode<PRIMITIVE>::Inner NodeInner;

        typedef SCALAR scalar_type;
        typedef base::Vec<DIMENSIONS,scalar_type> vector_type;
        typedef Bounds<DIMENSIONS,scalar_type> bounds_type;
        typedef Ray<DIMENSIONS,scalar_type> ray_type;

        /// Node container
        typedef std::vector<Node> NodeCont;

        /// Primitive container
        typedef std::vector<PRIMITIVE*> PrimCont;

        NodeCont nodes_;
        PrimCont primLists_;

        struct NodeStackElement
        {
          bounds_type bounds_;
          PrimCont primList_;
          unsigned node_;
          int depth_;

          Node* node(NodeCont& _nodes)
          {
            return &_nodes[node_];
          }
        };

        const Node& root() const
        {
          return nodes_[0];
        }
        Node& root()
        {
          return nodes_[0];
        }

        const Node* node(unsigned _nodeIndex) const
        {
          return &nodes_[_nodeIndex];
        }

        /** @brief Traverses kd-tree along a ray recursively
         * @param _ray        Ray which traverses KDTree
         * @param _node       Current node
         * @param _tnear      Near ray section
         * @param _tfar       Far ray section
         * @param _found      Flag which determines if a ray was found
         * @param _normal     Normal of intersecting triangle
         * @param _texCoords  Texture coordinates of intersecting triangle
         */
        /*    float recKDTreeTraverse(Ray& ray, const Node* node, float tnear, float tfar, bool& found,
                Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
            {
              if (node->isLeaf())
              {
                found |= node->leaf_.intersect(ray,primLists_);
                return ray.tMax_;
              }

              int k = node->inner_.axis();
              float d = (node->inner_.splitPos() - ray.org_[k]) / ray.dir_[k];

              const Node* front = &this->nodes_[node->inner_.left()];
              const Node* back  = &this->nodes_[node->inner_.right()];
              if (ray.dir_[k] < 0) std::swap(front,back);

              if (d <= tnear)
              {
                recKDTreeTraverse(ray,back,tnear,tfar,found);
              } else
                if (d >= tfar)
                {
                  recKDTreeTraverse(ray,front,tnear,tfar,found);
                } else
                {
                  float t_hit = recKDTreeTraverse(ray,front,tnear,d,found);
                  if (t_hit <= d) return t_hit;
                  return recKDTreeTraverse(ray,back,d,tfar,found,_normal,_texCoords);
                }
              return INF;
            }*/

        bool traversal(ray_type& _ray, const bounds_type& _bounds, vector_type* _normal = NULL) const
        {
          struct NodeTraversalStackItem
          {
            const Node* node_;
            scalar_type tNear_,tFar_;
          };

          NodeTraversalStackItem _stack[16];
          int _stackPt = -1;

          scalar_type _tNear = _ray.tNear(), _tFar = _ray.tFar();


          ///@todo Make bounds intersection method
          //if (!_boundingBox.intersect(_ray,_tNear,_tFar)) return false;

          const Node* _node = &root();
          bool _found = false;

          while (1)
          {
            while (!_node->isLeaf())
            {
              int k = _node->inner_.axis();
              scalar_type d = (_node->inner_.splitPos() - _ray.org()[k]) / _ray.dir()[k];

              const Node* _front = &nodes_[_node->inner_.left()];
              const Node* _back  = &nodes_[_node->inner_.right()];
              if (_ray.dir()[k] < 0) std::swap(_front,_back);

              if (d < _tNear)
              {
                _node = _back;
              }
              else if (d >= _tFar)
              {
                _node = _front;
              }
              else
              {
                _stackPt++;
                _stack[_stackPt].node_  = _back;
                _stack[_stackPt].tNear_ = d;
                _stack[_stackPt].tFar_  = _tFar;

                _node = _front;
                _tFar = d;
              }
            }

            //    LOG_MSG << fmt("% %") % _tNear % _ray.tFar();
            _found |= _node->leaf_.intersect(_ray,primLists_,_tNear,_ray.tFar(),_normal);
            //    LOG_MSG << fmt("% %") % _tNear % _ray.tFar();
            if (_found) return true;
            if (_stackPt < 0) return _found;

            _node  = _stack[_stackPt].node_;
            _tNear = _stack[_stackPt].tNear_;
            _tFar  = _stack[_stackPt].tFar_;
            _stackPt--;
          }
          return _found;
        }
//#define ITERATIVE

#ifdef ITERATIVE
        void build(std::vector<PRIMITIVE>& _objs, const bounds_type& _bounds, unsigned _primitivesPerNode)
        {
          static NodeStackElement _stack[32];
          nodes_.clear();
          nodes_.reserve(2*maxDepth()*_objs.size()/_primitivesPerNode);
          nodes_.resize(1);

          primLists_.clear();

          // Initialize root node
          NodeStackElement _node;
          _node.bounds_ = _bounds;  // Object's bounds
          _node.node_ = 0;
          _node.depth_ = 0;
          nodes_[0].inner_.setup(nodes_,X,0);


          // Init primitive lists
          _node.primList_.reserve(_objs.size());
          for (unsigned i = 0; i < _objs.size(); i++)
            _node.primList_.push_back(&_objs[i]);

          _stack[0] = _node;
          int _stackPt = 0;

          while (1)
          {
            NodeStackElement* _nodePt = &_stack[_stackPt];

            // Perform depth-in first search until leaf node
            while (_nodePt->primList_.size() > _primitivesPerNode && _nodePt->depth_ < maxDepth())
            {
              base::Axis _axis = _nodePt->bounds_.dominantAxis();
              scalar_type _splitPos = 0.5*(_nodePt->bounds_.min()[_axis] + _nodePt->bounds_.max()[_axis]);

              _nodePt->node(nodes_)->inner_.setup(nodes_,_axis,_splitPos);
              nodes_.resize(nodes_.size()+2);

              _nodePt->depth_++;
              _stackPt++;
              NodeStackElement* _right = &_stack[_stackPt];
              _right->primList_.clear();
              _right->primList_.reserve(_nodePt->primList_.size());
              _right->depth_ = _nodePt->depth_;
              _right->bounds_ = _nodePt->bounds_;

              PrimCont _leftList;

              // Make bounding boxes
              bounds_type _leftBounds(_nodePt->bounds_);
              insertion(_splitPos,_axis,_nodePt->bounds_,_nodePt->primList_,_leftList,_right->primList_,_leftBounds,_right->bounds_);
              _nodePt->bounds_(_leftBounds.min(),_leftBounds.max());
              _nodePt->primList_ = _leftList;

              _nodePt->node_ = _nodePt->node(nodes_)->inner_.left();
              _right->node_ = _nodePt->node(nodes_)->inner_.right();
            }

            // Make leaf node
            _nodePt->node(nodes_)->leaf_.insert(_nodePt->primList_,primLists_);

            _stackPt--;
            if (_stackPt < 0) break;
          }

          for (size_t i = 0; i < nodes_.size() ; i++)
            std::cout << nodes_[i].isLeaf();
          std::cout << std::endl;
        }
#else
        void build(std::vector<PRIMITIVE>& objs, const bounds_type& _bounds, unsigned _primitivesPerNode = 10)
        {
          primLists_.clear();
          nodes_.clear();
          nodes_.resize(1);
          PrimCont _nodeObjs;
          _nodeObjs.reserve(objs.size());

          for (unsigned i = 0; i < objs.size(); i++)
            _nodeObjs.push_back(&objs[i]);

          divideNode(0,_bounds,_nodeObjs,0,_primitivesPerNode);
          //   for (size_t i = 0; i < nodes_.size() ; i++)
          //   std::cout << nodes_[i].isLeaf();
        }

        virtual void divideNode(unsigned nodeIndex, bounds_type _bounds, PrimCont& _nodeObjs, unsigned depth, unsigned _primitivesPerNode)
        {
          if (depth >= maxDepth() || _nodeObjs.size() <= _primitivesPerNode )
          {
            // We have a leaf node!
            nodes_[nodeIndex].leaf_.insert(_nodeObjs,primLists_);
            return;
          }

          // Setup node
          base::Axis _axis = _bounds.dominantAxis();
          float _splitPos = 0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]);

          //      std::cout << _axis << " " << _splitPos << std::endl;
          nodes_[nodeIndex].inner_.setup(nodes_,_axis,_splitPos);
          bounds_type _leftBounds, _rightBounds;
          nodes_.resize(nodes_.size()+2);

          PrimCont _leftObjs, _rightObjs;
          insertion(_splitPos,_axis,_bounds,_nodeObjs,_leftObjs,_rightObjs,_leftBounds,_rightBounds);

          _nodeObjs.clear();
          //     LOG_MSG << fmt("% %") % nodes_[nodeIndex].inner_.left() % nodes_[nodeIndex].inner_.right();
          divideNode(nodes_[nodeIndex].inner_.left(),_leftBounds,_leftObjs,depth+1,_primitivesPerNode);
          divideNode(nodes_[nodeIndex].inner_.right(),_rightBounds,_rightObjs,depth+1,_primitivesPerNode);
        }
#endif

        inline void insertion( scalar_type _splitPos,
                               base::Axis _axis,
                               bounds_type& _bounds,
                               PrimCont& _primList,
                               PrimCont& _leftList,
                               PrimCont& _rightList,
                               bounds_type& _leftBounds,
                               bounds_type& _rightBounds)
        {
          _bounds.split(_splitPos,_axis,_leftBounds,_rightBounds);
          for (unsigned i = 0; i < _primList.size(); i++)
          {
            PRIMITIVE* _prim = _primList[i];
            prim::SplitPlaneIntersect _result = _prim->intersect(_axis,_splitPos,_leftBounds,_rightBounds);

            if (_result.left())  _leftList.push_back(_prim);
            if (_result.right()) _rightList.push_back(_prim);
          }
          _primList.clear();
        }

        virtual float splitPos(const PrimCont& _primList, NodeInner* _inner, const bounds_type& _bounds) const
        {
          return 0.5*(_bounds.min()[_inner->axis()] + _bounds.max()[_inner->axis()]);
        }

        static unsigned maxDepth()
        {
          return 16;
        }
      };
    }
  }
}
