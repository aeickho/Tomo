#include "tomo/scene/ObjectPlacer.hpp"

#include <map>

using namespace std;

namespace tomo
{
  namespace scene
  {
    using geometry::base::Bounds3f;
    using geometry::base::Vec3f;
    using geometry::base::Vec2f;
    using slicing::SliceStack;
    using slicing::SlicableObject;

    ObjectPlacer::ObjectPlacer(const PrintBounds& _printBounds) :
      objectBorder_(10.0),
      printBounds_(_printBounds)
    {

    }

    void ObjectPlacer::place(ObjectPlacer::map_type& _objects)
    {
      typedef multimap<float,SlicableObject*> AreaMap;
      AreaMap _objectsByArea;

      for (map_type::iterator it = _objects.begin(); it != _objects.end(); ++it)
      {
/*        Bounds3f _bounds = it->first->bounds();
        Vec3f _size = _bounds.size(); 
        float _area = _size.x() * _size.y();

        // Move object to ground
        Vec3f _groundMove(0.0,0.0,_bounds.min().z());
        it->first->pos() -= _groundMove;

        // Insert object into map, sorted by area
        _objectsByArea.insert(make_pair<float,SlicableObject*>(_area,it->first));
  */    }

      std::vector<rect_type> _rectList;

      // Place all objects
      AreaMap::iterator it;
      for (it = _objectsByArea.begin(); it != _objectsByArea.end(); ++it)
        place(it->second,_objects[it->second],_rectList);
    }

    void ObjectPlacer::place(SlicableObject* _object, 
                             SliceStack* _slices,
                             vector<rect_type>& _rectList)
    {
      /*
      Bounds3f _objBounds = _object->bounds(); 
      Bounds2f _printBounds(Vec2f(printBounds_.min().x(),printBounds_.min().y()),
                            Vec2f(printBounds_.max().x(),printBounds_.max().y()));
      Vec2f _borderVec(objectBorder_,objectBorder_);
      
      Bounds2f _objRect(Vec2f(_objBounds.min().x(),_objBounds.min().y()) + _borderVec,
                        Vec2f(_objBounds.min().x(),_objBounds.max().y()) + _borderVec);

      BOOST_FOREACH( Bounds2f& _rect, _rectList )
      {
        if (overlap(_rect,_objRect))
        {
          /// TODO Use guillotine packing here...
        } 
      }*/
    }

  }
}
