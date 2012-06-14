#pragma once
#include <tbd/config.h>

namespace tomo
{
  class GCodeGenerator : public tbd::ConfigurableObject
  {
    GCodeGenerator(Config* _config = NULL) : tbd::ConfigurableObject(_config) {};

    GCode generate(const Slices& slices const PrinterSetup& printerSetup);

  };
}
