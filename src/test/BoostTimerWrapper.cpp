#include "BoostTimerWrapper.hpp"

#include <boost/timer/timer.hpp>


namespace tomo
{
  namespace test
  {
    boost::timer::cpu_timer g_timer_;
    
    Timer::Timer(int _accuracy)
    {
      start();
    }

    void Timer::start()
    {
      g_timer_.start();
    }

    void Timer::stop()
    {   
      g_timer_.stop();
    }

    void Timer::output(std::string _format) const
    {
      std::cout << boost::timer::format(g_timer_.elapsed(),accuracy(),_format) << std::endl; 
    }

    int Timer::accuracy() const
    {
      return accuracy_;
    }

    void Timer::accuracy(int _accuracy)
    {
      accuracy_=_accuracy;
    }
  }
}
