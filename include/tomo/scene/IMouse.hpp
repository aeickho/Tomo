#pragma once

namespace tomo
{
  /** @brief mouse interface
   */
  struct IMouse
  {
    /** mouse drag (move while button is down)
     * @param _x relative x change
     * @param _y relative y change
     */
    virtual void drag( int _x, int _y ) { }
    /** mouse wheel change
     * @param _d wheel change
     */
    virtual void wheel( int _d ) { }
  };
}
