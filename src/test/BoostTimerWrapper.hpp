#pragma once

#include <string>

namespace tomo
{
  namespace test
  {
    struct Timer
    {
      Timer(int _accuracy = 5);

      void start();
      void stop(); 
      void output(std::string _format) const;

      int accuracy() const;
      void accuracy(int _accuracy);
    private:
      int accuracy_;
    };
  }
}
