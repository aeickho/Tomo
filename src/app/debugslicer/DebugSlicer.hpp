#pragma once

namespace tomo
{
  namespace slicing
  {
    class DebugSlicer : public Slicer
    {
      DebugSlicer(bool _box = true,
                  bool _tube = false,
                  bool _sphere = false,
                  bool _mesh = false,
                  string _meshFileName = string())
           box_(_box),
           tube_(_tube),
           sphere_(_sphere),
           mesh_(_mesh),
           meshFileName_(_meshFileName)
      {}

      typedef map<SlicableObject*,Slices*> SlicesMap;

      SlicesMap generateObjects(const vector<string>& _inputFiles
                                const UsedObjects& _usedObjects);
      {
        BOOST_FOREACH( const string& _inputFile, _inputFiles )
        {
          tbd::Config _configFile(_inputFile);
        }
      }


        TBD_PROPERTY(bool,box);
        TBD_PROPERTY(bool,tube);
        TBD_PROPERTY(bool,sphere);
        TBD_PROPERTY(bool,mesh);
        TBD_PROPERTY(string,meshFileName);
      };

    }
  }

