////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\Menu.hpp
//! \brief Handles window menus and popup menus
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_MENU_HPP
#define WTL_WINDOW_MENU_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \struct WindowMenu - Encapsulates a window menu
  struct WindowMenu
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::WindowMenu
    //! Create from existing menu handle
    //!
    //! \param[in] menu - Menu handle
    ///////////////////////////////////////////////////////////////////////////////
    WindowMenu(const HMenu& menu) : Handle(menu)
    {}

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::handle const
    //! Get the native menu handle 
    //! 
    //! \return HMENU - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    HMENU handle() const
    {
      return this ? Handle.get() : nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::getSubMenu
    //! Retrieves the handle of a sub-menu
    //! 
    //! \return HMENU - Native sub-menu handle
    ///////////////////////////////////////////////////////////////////////////////
    HMENU getSubMenu(int32 idx) const
    {
      return ::GetSubMenu(Handle, idx);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::operator HMENU
    //! Implicit user conversion to native menu handle
    //! 
    //! \return HMENU - Native handle
    ///////////////////////////////////////////////////////////////////////////////
    operator HMENU() const
    {
      return handle();
    }

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------
  protected:
    HMenu   Handle;     //!< Menu handle
  };

}

#endif  // WTL_WINDOW_MENU_HPP
