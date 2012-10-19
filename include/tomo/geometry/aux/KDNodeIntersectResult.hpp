#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      /// Struct which holds the result of an intersection test between split plane and primitive
      struct KDNodeIntersectResult
      {
      public:
        KDNodeIntersectResult(bool _left = false, bool _right = false) : result_(0)
        {
          set(_left,_right);
        }

        void set(bool _left, bool _right)
        {
          result_ = _left | (_right << 1);
        }

        bool left()
        {
          return result_ & 1;
        }
        bool right()
        {
          return result_ & 2;
        }
        bool both()
        {
          return result_ != 0;
        }
      private:
        unsigned char result_;
      };
    }
  }
}

