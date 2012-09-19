#include "tomo/LineSegment.hpp"
#include <tbd/config.h>

namespace tomo
{
  using tbd::Config;
  using tbd::ConfigurableObject;

  class Slicer : public ConfigurableObject
  {
  public:
    Slicer(Config* _config = NULL) : ConfigurableObject(_config) {}

//    Slices slice(const Mesh& _mesh) const;
//    Slices slice(const PointCloud& _pointCloud) const;

    TBD_PROPERTY_CFG(float,sliceHeight,"SLICE_HEIGHT_MM",0.2);

    TBD_PROPERTY_CFG(int,numPerimeters,"NUM_PERIMETERS",3);
    TBD_PROPERTY_CFG(int,numSolidLayers,"NUM_SOLID_LAYERS",3);

    TBD_PROPERTY_CFG(bool,filling,"FILLING",true);
    TBD_PROPERTY_CFG(float,fillDensity,"FILL_DENSITY",0.4);
    TBD_PROPERTY_CFG(float,fillAngle,"FILL_ANGLE",0.0);

    /// Speed
    TBD_PROPERTY_CFG(float,speedPerimeter,"SPEED_PERIMETER",30);
    TBD_PROPERTY_CFG(float,speedExternalPerimeter,"EXTERNAL_PERIMETER",30);
    TBD_PROPERTY_CFG(float,speedInfill,"SPEED_INFILL",60);
    TBD_PROPERTY_CFG(float,speedSolidInfill,"SPEED_SOLID_INFILL",60);
    TBD_PROPERTY_CFG(float,speedTopSolid,"SPEED_TOPSOLID",50);
    TBD_PROPERTY_CFG(float,travel,"SPEED_TRAVEL",130);

    /// Support Material
    TBD_PROPERTY_CFG(bool,generateSupportMaterial,"GENERATE_SUPPORT_MATERIAL",false);
    TBD_PROPERTY_CFG(float,overhangThreshold,"OVERHANG_THRESHOLD",45.0);
    TBD_PROPERTY_CFG(float,patternSpacing,"PATTERN_SPACING_MM",2.5);

    /// Filament settings
    TBD_PROPERTY_CFG(float,diameter,"DIAMETER",3.0);
    TBD_PROPERTY_CFG(float,extrusionMultiplier,"EXTRUSION_MULTIPLIER",1.0);
    TBD_PROPERTY_CFG(float,temperature,"TEMPERATURE",200.0);
    TBD_PROPERTY_CFG(float,bedTemperature,"BED_TEMPERATURE",0.0);

    // Fan settings
    TBD_PROPERTY_CFG(bool,cooling,"COOLING",false);
    TBD_PROPERTY_CFG(float,minFanSpeed,"MIN_FAN_SPEED",0.35);
    TBD_PROPERTY_CFG(float,maxFanSpeed,"MAX_FAN_SPEED",1.00);


    TBD_PROPERTY_CFG(unsigned,resX,"RESOLUTION_X",1000);
    TBD_PROPERTY_CFG(unsigned,resY,"RESOLUTION_Y",1000);

  private:
  };
}


