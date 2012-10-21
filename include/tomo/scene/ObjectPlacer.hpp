#include "PrintBounds.hpp"

#include "tomo/slicing/Slice.hpp"
#include "tomo/slicing/SlicableObject.hpp"

#include <map>

namespace tomo
{
  namespace scene
  {
    class ObjectPlacer
    {
      ObjectPlacer(const PrintBounds& _printBounds);
        
      typedef std::map<slicing::SlicableObject*,slicing::Slices*> map_type;
      typedef geometry::base::Bounds2f rect_type;

      void place(map_type& _objects);

      /// Object border in mm
      TBD_PROPERTY(float,objectBorder);
      TBD_PROPERTY_RO(const PrintBounds&,printBounds);

    private:

      void place(slicing::SlicableObject* _object, 
                 slicing::Slices* _slices,
                 std::vector<rect_type>& _rectList);
    };
  }
}
