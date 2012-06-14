#pragma once
#include <vector>
#include <string>

namespace tomo 
{
  class GCodeInstruction
  {
    virtual std::string asString() = 0;
  };

  std::vector<GCodeInstruction*> GCodeInstructions;

  class GCode : public GCodeInstructions
  {
  };
}


