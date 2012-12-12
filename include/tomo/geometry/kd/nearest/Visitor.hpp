#pragma once

#include "tomo/geometry/kd/Visitor.hpp"
#include "tomo/geometry/algorithm/sqrDistance.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        namespace
        {
          template<typename PRIMITIVE>
          struct VisitorState
          {
            typedef PRIMITIVE primitive_type;
            typedef Node<primitive_type> node_type;
            typedef typename primitive_type::bounds_type bounds_type;

            TBD_PROPERTY(const node_type*,node)
            TBD_PROPERTY_REF(bounds_type,bounds)
          };

          template<typename PRIMITIVE>
          struct NearestContainer 
          {
            typedef PRIMITIVE primitive_type;
            typedef typename primitive_type::scalar_type scalar_type;
            typedef std::pair<scalar_type,const primitive_type*> container_type;

            void clear()
            {
              nearest_.first = INF;
              nearest_.second = nullptr;
            }

            void insert(scalar_type _distance, const primitive_type* _primitive)
            {
              if (_distance <= minDistance())
              {
                nearest_ = container_type(_distance,_primitive);
              }
            };

            const primitive_type* getNearest() const
            {
              return nearest_.second;
            }

            scalar_type minDistance() const
            {
              return nearest_.first;
            }

            TBD_PROPERTY_REF_RO(container_type,nearest)
          };

          /// A container which can hold several nearest primitives
          template<typename PRIMITIVE>
          struct NearestContainerMulti 
          {
            typedef PRIMITIVE primitive_type;
            typedef typename primitive_type::scalar_type scalar_type;
            typedef std::set<const primitive_type*> nearest_primitives_cntr_type;
            typedef std::multimap<scalar_type,const primitive_type*> container_type;
            
            void clear()
            {
              nearest_.clear();
            }

            const primitive_type* getNearest() const
            {
              if (nearest_.empty()) return nullptr;
              return nearest_.begin().second;
            }

            scalar_type minDistance() const
            {
              if (nearest_.empty()) return INF;
              return nearest_.begin().first;
            }

            const container_type& nearest() const { return nearest_; }
          
          protected:
            container_type nearest_;
          };

          template<typename PRIMITIVE>
          struct KNearestContainer : NearestContainerMulti<PRIMITIVE>
          {
            typedef PRIMITIVE primitive_type;
            typedef typename primitive_type::scalar_type scalar_type;
            typedef NearestContainerMulti<primitive_type> base_container_type;
            typedef typename base_container_type::container_type container_type;

            void insert(scalar_type _distance, const primitive_type* _primitive)
            {
              container_type& _nearest = base_container_type::nearest_;
              if (_nearest.find(_primitive) != _nearest.end()) return;
              if (_distance <= base_container_type::minDistance() )
              {
                _nearest.insert(pair_type(_distance,_primitive));
              }
              /// Only hold at most k nearest primitives
              if (_nearest.size() > k_)
              {
                _nearest.erase(--_nearest.end());
              }
            }

            TBD_PROPERTY(int,k)
          };

          template<typename PRIMITIVE>
          struct InRadiusContainer : NearestContainerMulti<PRIMITIVE>
          {
            typedef PRIMITIVE primitive_type;
            typedef typename primitive_type::scalar_type scalar_type;
            typedef NearestContainerMulti<primitive_type> base_container_type;
            typedef typename base_container_type::container_type container_type;

            void insert(scalar_type _distance, const primitive_type* _primitive)
            {
              if (_distance < sqrRadius_ )
              {
                container_type& _nearest = base_container_type::nearest_;
                _nearest.insert(pair_type(_distance,_primitive));
              }
            }

            TBD_PROPERTY_MON(scalar_type,radius,updateRadius)
          private:
            void updateRadius()
            {
              sqrRadius_ = radius_ * radius_;
            }
            scalar_type sqrRadius_;
          };
        }

        /// Nearest visitor base concept
        template<typename NEAREST_CONTAINER>
        struct Visitor/* :
            kd::Visitor<
            Tree<typename NEAREST_CONTAINER::primitive_type>,
            VisitorState<typename NEAREST_CONTAINER::primitive_type>>*/
        {
          typedef NEAREST_CONTAINER cntr_type;
          typedef typename cntr_type::primitive_type primitive_type; 
          typedef VisitorState<primitive_type> state_type;
          typedef Tree<primitive_type> kdtree_type;
          typedef typename kdtree_type::bounds_type bounds_type;
          typedef typename primitive_type::scalar_type scalar_type;
          
          Visitor(kdtree_type& _kdTree) : kdTree_(_kdTree)
          {
            state_.node(&_kdTree.nodes().getRoot());
            state_.bounds(_kdTree.bounds());
          }

          void find(primitive_type& _primitive)
          {
            primitive(&_primitive);
            traversal(*this,kdTree_);
          }

          TBD_PROPERTY_REF(state_type,state)
          TBD_PROPERTY_REF_RO(cntr_type,container)
          TBD_PROPERTY_REF(primitive_type*,primitive)

        public:
          /// Root traversal
          bool root()
          {
            TOMO_ASSERT(primitive_);
            container_.clear();
            return true;
          }

          void traverseLeft(state_type& _state, bounds_type& _bounds)
          {
            _state.node(&kdTree_.nodes().getNode(state_.node()->inner().left()));
            _state.bounds(_bounds);
          }

          void traverseRight(state_type& _state, bounds_type& _bounds)
          {
            _state.node(&kdTree_.nodes().getNode(state_.node()->inner().right()));
            _state.bounds(_bounds);
          }

          /// Inner node traversal
          bool inner(state_type& _pushedState)
          {
            /// Split current node's bounds in to a left and right one
            bounds_type _left, _right;
            state_.bounds().split(state_.node()->inner().splitPos(),
                                  state_.node()->inner().axis(),
                                  _left,_right);

            insert(state_.node()->attributes().primitive());
            
            /// Calculate primitives squared distance to node's bounds
            scalar_type _leftDist = algorithm::sqrDistance(*primitive_,_left),
                        _rightDist = algorithm::sqrDistance(*primitive_,_right);
            bool _leftFirst = _leftDist <= _rightDist;
            scalar_type _minDist = container_.minDistance();

            bool _traverseLeft  = _leftDist <= _minDist;
            bool _traverseRight = _rightDist <= _minDist;

            if (_leftFirst)
            {
              if (_traverseRight) traverseRight(_pushedState,_right);
              traverseLeft(state_,_left);
              return _traverseRight;
            }
            else
            {
             if (_traverseLeft) traverseLeft(_pushedState,_left);
              traverseRight(state_,_right);
              return _traverseLeft;
            }
            return false;
          }

          /// Leaf node traversal
          bool leaf()
          {
            /// For each primitive in leaf node, calculate distance
            /// Distance is smaller than current found nearest primitive,
            /// replace it
            typename kdtree_type::node_cntr_type::prim_const_iterator _begin, _end;
            typedef typename kdtree_type::leaf_node_type leaf_node_type;
            const leaf_node_type& _leaf = state_.node()->leaf();
            
            insert(state_.node()->attributes().primitive());

            kdTree().nodes().leafRange(_leaf,_begin,_end);
            for (auto it = _begin; it != _end; ++it) insert(*it);

            return false;
          }

        const kdtree_type& kdTree() const
        {
          return kdTree_;
        }

        
        private:
          void insert(const primitive_type* _primitive)
          {
            if (_primitive == primitive_ || _primitive == nullptr) return;
            scalar_type _distance = algorithm::sqrDistance(*_primitive,*primitive_);

            if (_distance > 0.000001) return;
            container_.insert(_distance,_primitive);
          }

          kdtree_type& kdTree_;
        };

        template<typename PRIMITIVE>
        struct Nearest : Visitor<NearestContainer<PRIMITIVE>>
        {
          typedef PRIMITIVE primitive_type;
          typedef Tree<primitive_type> kdtree_type;
          Nearest(kdtree_type& _kdTree) : Visitor<NearestContainer<PRIMITIVE>>(_kdTree) {}
        };

        template<typename PRIMITIVE>
        struct KNearest : Visitor<KNearestContainer<PRIMITIVE>>
        {
          typedef PRIMITIVE primitive_type;
          typedef Visitor<KNearestContainer<primitive_type>> base_visitor_type;
          typedef Tree<primitive_type> kdtree_type;
          
          KNearest(kdtree_type& _kdTree, int _k) : base_visitor_type(_kdTree) 
          {
            base_visitor_type::nearest().k(_k);
          }
        };

        template<typename PRIMITIVE>
        struct InRadius : Visitor<NearestContainer<PRIMITIVE>>
        {
          typedef PRIMITIVE primitive_type;
          typedef typename primitive_type::scalar_type scalar_type;
          typedef Visitor<KNearestContainer<primitive_type>> base_visitor_type;
          typedef Tree<primitive_type> kdtree_type;

          InRadius(kdtree_type& _kdTree, scalar_type _radius) : base_visitor_type(_kdTree) 
          {
            base_visitor_type::nearest().radius(_radius);
          }
        };
      }
    }
  }
}

