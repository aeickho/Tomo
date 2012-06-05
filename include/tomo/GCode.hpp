#pragma once
#include <vector>

namespace tomo 
{
  class GCodeInstruction
  {
    virtual string asString() = 0;
  };

  vector<GCodeInstruction*> GCodeInstructions;

  class GCode : public GCodeInstructions
  {
  };
}


