////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowMenu.hpp
//! \brief Provides window menus and popup menus
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
  //! \enum MenuItemFlag - Item flags
  enum class MenuItemFlag : uint32
  {
    State = 0x00000001,			  //!< [Windows 4.00] 
    Id = 0x00000002,		    	//!< [Windows 4.00] 
    SubMenu = 0x00000004,			//!< [Windows 4.00] 
    CheckMarks = 0x00000008,	//!< [Windows 4.00] 
    Type = 0x00000010,			  //!< [Windows 4.00] 
    Data = 0x00000020,			  //!< [Windows 4.00] 

    String = 0x00000040,			//!< [Windows 5.00] 
    Bitmap = 0x00000080,			//!< [Windows 5.00] 
    FType = 0x00000100,			  //!< [Windows 5.00] 
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<MenuItemFlag>  : std::true_type  {};
  template <> struct is_contiguous<MenuItemFlag> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<MenuItemFlag>     : std::integral_constant<MenuItemFlag,MenuItemFlag::FType>   {};
  template <> struct min_value<MenuItemFlag>     : std::integral_constant<MenuItemFlag,MenuItemFlag::State>   {};


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct MenuItemInfo - Menu item info
  ///////////////////////////////////////////////////////////////////////////////
  struct MenuItemInfo : ::MENUITEMINFO
  {
    //! \alias base - Define base type
    using base = ::MENUITEMINFO;

    ///////////////////////////////////////////////////////////////////////////////
    // MenuItemInfo::MenuItemInfo
    //! Create menu item info
    //! 
    //! \param[in] const* str - Item text
    ///////////////////////////////////////////////////////////////////////////////
    MenuItemInfo(const char* str)
    {
      cbSize     = sizeof(base);
      fMask      = enum_cast(MenuItemFlag::String);
      dwTypeData = LPWSTR(str);
    }
  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct WindowMenu - Encapsulates a window menu
  ///////////////////////////////////////////////////////////////////////////////
  struct WindowMenu
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // WindowMenu::WindowMenu
    //! Create empty menu 
    ///////////////////////////////////////////////////////////////////////////////
    WindowMenu()
    {}

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
    // WindowMenu::insert
    //! Inserts an item representing a gui command
    //! 
    //! \tparam ENC - Message character encoding 
    //! \tparam CMD - Command Id
    //! 
    //! \param[in] const &cmd - Gui command
    //! \param[in] idx - Zero-based position index
    //! \param[in] &item - Item properties
    //! 
    //! \throw 
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, CommandId CMD>
    void insert(GuiCommand<ENC,CMD>& cmd, int32 idx)
    {
      MenuItemInfo item(cmd.name());
      // Insert item
      if (!getFunc<ENC>(::InsertMenuItemA,::InsertMenuItemW)(Handle, idx, FALSE, &item))
        throw platform_error(HERE, "Unable to insert menu item");
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
