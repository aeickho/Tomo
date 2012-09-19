#include <map>
#include <vector>

namespace tomo
{
  /** @brief Plane Stack is a class template to represent several planes ordered by position
   *  @details Internally, planes are stored in a map<POSITION,PLANE>
   */
  template<class POSITION, class PLANE>
  struct PlaneStack
  {
    typedef std::pair<POSITION,PLANE> key_value_type;
    typedef std::map<POSITION,PLANE> map_type;
    typedef typename map_type::iterator iterator;
    typedef typename map_type::const_iterator const_iterator;

    std::vector<PLANE*> fetch()
    {
      std::vector<PLANE*> _planes;
      _planes.reserve(planes_.size());
      for (iterator it = planes_.begin(); it != planes_.end(); ++it)
        _planes.push_back(&it->second);
      return _planes;
    }

    std::vector<const PLANE* > fetch() const
    {
      std::vector< const PLANE* > _planes;
      _planes.reserve(planes_.size());
      for (const_iterator it = planes_.begin(); it != planes_.end(); ++it)
        _planes.push_back(&it->second);
      return _planes;
    }

    const map_type& get() const
    {
      return planes_;
    }
    map_type& get()
    {
      return planes_;
    }


    /// Get slice by position
    iterator get(POSITION _pos)
    {
      return planes_.lower_bound(_pos);
    }

    const_iterator get(POSITION _pos) const
    {
      return planes_.lower_bound(_pos);
    }

    iterator end()
    {
      return planes_.end();
    }
    iterator begin()
    {
      return planes_.begin();
    }
    const_iterator end() const
    {
      return planes_.end();
    }
    const_iterator begin() const
    {
      return planes_.begin();
    }

  protected:
    map_type planes_;
  };
}
