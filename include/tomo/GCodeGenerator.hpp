#pragma once
#include <tbd/config.hpp>

namespace tomo
{
  class GCodeGenerator : public ConfigurableObject
  {
    GCodeGenerator(Config* _config = NULL) : ConfigurableObject(_config) {};

    GCode generate(const Slices& slices const PrinterSetup& printerSetup);

  };
}
