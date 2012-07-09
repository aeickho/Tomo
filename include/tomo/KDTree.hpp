#pragma once

#include "tomo/BoundingBox.hpp"

#include <vector>
#include <stack>

namespace tomo 
{
  template <typename PRIMITIVE> 
  union KDNode
  {
    bool isLeaf() const { return !inner_.leafFlag(); }

    /// Inner node
    struct Inner
    {
      typedef std::vector<KDNode> NodeCont;

    public:
      inline bool leafFlag() const { return data_ & (1 << 31); }
      inline Axis axis() const { return Axis(data_ & 3); }

      /// Insert children
      inline void setup(NodeCont& _nodes, Axis _axis, float _splitPos)
      {
        data_ = (0x7FFFFFFC & (_nodes.size() << 2)) | (1 << 31) | _axis;
        splitPos_ = _splitPos;
      }

      inline unsigned left() const { return (data_ & 0x7FFFFFFC) >> 2; }
      inline unsigned right() const { return ((data_ & 0x7FFFFFFC) >> 2) + 1; }

      inline float splitPos() const { return splitPos_; }
 
    private:
      unsigned int data_;
      float splitPos_;
    };

    /// Leaf node
    struct Leaf
    {
    public:
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

      inline bool intersect(Ray& _ray, const PrimCont& _primLists, float _tNear, float _tFar, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        bool _found = false;
        for (unsigned i = offset_; i < offset_ + size_; i++)
        {
          PRIMITIVE* _prim = _primLists[i]; 
          _found |= _prim->intersect(_ray,_tNear,_tFar,_normal,_texCoords);
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

  template <typename PRIMITIVE, unsigned int PRIMITIVES_PER_NODE = 10, unsigned int MAX_DEPTH = 16> 
  struct KDTree
  {
    /// Node type
    typedef KDNode<PRIMITIVE> Node;
    typedef typename KDNode<PRIMITIVE>::Inner NodeInner;
    
    /// Node container
    typedef std::vector<Node> NodeCont;
    
    /// Primitive container
    typedef std::vector<PRIMITIVE*> PrimCont;


    NodeCont nodes_;
    PrimCont primLists_;
    
    struct NodeStackElement
    {
      Bounds bounds_;
      PrimCont primList_;
      unsigned node_;
      int depth_;

      Node* node(NodeCont& _nodes) { return &_nodes[node_]; }
    };

    const Node& root() const { return nodes_[0]; }
    Node& root() { return nodes_[0]; }

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

     bool traversal(Ray& _ray, const BoundingBox& _boundingBox, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
    {
      struct NodeTraversalStackItem
      {
        const Node* node_;
        float tNear_,tFar_;
      };

      NodeTraversalStackItem _stack[MAX_DEPTH];
      int _stackPt = -1;

      float _tNear = _ray.tNear(), _tFar = _ray.tFar();
      if (!_boundingBox.intersect(_ray,_tNear,_tFar)) return false;

      const Node* _node = &root();
      bool _found = false;

      while (1)
      {
        while (!_node->isLeaf())
        {
          int k = _node->inner_.axis();
          float d = (_node->inner_.splitPos() - _ray.org()[k]) / _ray.dir()[k];

          const Node* _front = &nodes_[_node->inner_.left()];
          const Node* _back  = &nodes_[_node->inner_.right()];
          if (_ray.dir()[k] < 0) std::swap(_front,_back); 

          if (d < _tNear)
          {
            _node = _back;
          } else if (d >= _tFar)
          {
            _node = _front;
          } else
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
        _found |= _node->leaf_.intersect(_ray,primLists_,_tNear,_ray.tFar(),_normal,_texCoords);
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
    void build(std::vector<PRIMITIVE>& _objs, const Bounds& _bounds)
    {
      static NodeStackElement _stack[MAX_DEPTH+1];

      nodes_.clear();
      nodes_.reserve(2*MAX_DEPTH*_objs.size()/PRIMITIVES_PER_NODE);
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
        while (_nodePt->primList_.size() > PRIMITIVES_PER_NODE && _nodePt->depth_ < MAX_DEPTH) 
        { 
          Axis _axis = _nodePt->bounds_.dominantAxis(); 
          float _splitPos = 0.5*(_nodePt->bounds_.min()[_axis] + _nodePt->bounds_.max()[_axis]);
          
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
          Bounds _leftBounds(_nodePt->bounds_);
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
     void build(std::vector<PRIMITIVE>& objs, const Bounds& _bounds)
    {
      primLists_.clear();
      nodes_.clear();
      nodes_.resize(1);
      PrimCont _nodeObjs; _nodeObjs.reserve(objs.size());
      
      for (unsigned i = 0; i < objs.size(); i++)
        _nodeObjs.push_back(&objs[i]);

      divideNode(0,_bounds,_nodeObjs,0);
   //   for (size_t i = 0; i < nodes_.size() ; i++)
     //   std::cout << nodes_[i].isLeaf();
    }

   virtual void divideNode(unsigned nodeIndex, Bounds _bounds, PrimCont& _nodeObjs, int depth)
   {
        if (depth >= MAX_DEPTH || _nodeObjs.size() <= PRIMITIVES_PER_NODE )
        { // We have a leaf node!
          nodes_[nodeIndex].leaf_.insert(_nodeObjs,primLists_);
          return;
        }

        // Setup node
        Axis _axis = _bounds.dominantAxis(); 
        float _splitPos = 0.5*(_bounds.min()[_axis] + _bounds.max()[_axis]);
 
    //      std::cout << _axis << " " << _splitPos << std::endl;
        nodes_[nodeIndex].inner_.setup(nodes_,_axis,_splitPos);          
        Bounds _leftBounds, _rightBounds;
        nodes_.resize(nodes_.size()+2);
 
        PrimCont _leftObjs, _rightObjs;
        insertion(_splitPos,_axis,_bounds,_nodeObjs,_leftObjs,_rightObjs,_leftBounds,_rightBounds);

        _nodeObjs.clear();
   //     LOG_MSG << fmt("% %") % nodes_[nodeIndex].inner_.left() % nodes_[nodeIndex].inner_.right();
        divideNode(nodes_[nodeIndex].inner_.left(),_leftBounds,_leftObjs,depth+1);
        divideNode(nodes_[nodeIndex].inner_.right(),_rightBounds,_rightObjs,depth+1);
   }
#endif

   inline void insertion( float _splitPos, 
                     Axis _axis, 
                     Bounds& _bounds, 
                     PrimCont& _primList, 
                     PrimCont& _leftList, 
                     PrimCont& _rightList,
                     Bounds& _leftBounds,
                     Bounds& _rightBounds)
   {
        _bounds.split(_splitPos,_axis,_leftBounds,_rightBounds);
        for (unsigned i = 0; i < _primList.size(); i++)
        {
          PRIMITIVE* _prim = _primList[i];
          SplitPlaneIntersect _result = _prim->intersect(_axis,_splitPos,_leftBounds,_rightBounds);
     
          if (_result.left())  _leftList.push_back(_prim);
          if (_result.right()) _rightList.push_back(_prim); 
      }
        _primList.clear();
   }


    virtual float splitPos(const PrimCont& _primList, NodeInner* _inner, const Bounds& _bounds) const
    {
      return 0.5*(_bounds.min()[_inner->axis()] + _bounds.max()[_inner->axis()]);
    }
  };
}

