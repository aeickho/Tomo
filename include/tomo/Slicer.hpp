#include "tomo/Slice.hpp"
#include "tomo/SceneObject.hpp"
#include <tbd/config.h>

namespace tomo
{
  using tbd::Config;
  using tbd::ConfigurableObject;

  class Slicer : public ConfigurableObject
  {
  public:
    Slicer(Config* _config = NULL) : ConfigurableObject(_config) {}

    Slices slice(const Primitive3f& obj, unsigned nSlices = 0) const;

    //  boost::ptr_vector<Isle> fetchIsles(const SceneObject& obj, const SplitPlane& splitPlane, IsleType = ALL);

    TBD_PROPERTY_CFG(unsigned,nSlices,"NUMBER_OF_SLICES",10);
    TBD_PROPERTY_CFG(unsigned,resX,"RESOLUTION_X",1000);
    TBD_PROPERTY_CFG(unsigned,resY,"RESOLUTION_Y",1000);

  private:
    /*
    struct RaySegments
      {
        std::map<float,bool> segments_;
      }

      std::map<float,RaySegments> inter

      Slice generateSlice(SceneObject& obj, unsigned sliceIdx)
      {
      }
    */

  };
}


