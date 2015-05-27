//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\MenuFlags.hpp
//! \brief Defines flags for Win32 Api menu functions
//! \date 8 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MENU_FLAGS_HPP
#define WTL_MENU_FLAGS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum MenuType - Menu types
  enum class MenuType 
  {
    Window,     //!< Window menu
    Popup,      //!< Popup menu
  };
  

  //! \enum MenuItemFlag - Menu Item flags
  enum class MenuItemFlag : uint32_t
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
  
    
  
  //! \enum MenuItemState - Menu item states
  enum class MenuItemState : uint32_t
  {
    Grayed = 0x00000003L,			  //!< [Windows 4.00] 
    Disabled = MFS_GRAYED,			//!< [Windows 4.00] 
    Checked = MF_CHECKED,			  //!< [Windows 4.00] 
    Highlight = MF_HILITE,			//!< [Windows 4.00] 
    Enabled = MF_ENABLED,			  //!< [Windows 4.00] 
    UnChecked = MF_UNCHECKED,		//!< [Windows 4.00] 
    UnHighlight = MF_UNHILITE,	//!< [Windows 4.00] 
    Default = MF_DEFAULT,			  //!< [Windows 4.00] 
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<MenuItemState>  : std::true_type  {};
  template <> struct is_contiguous<MenuItemState> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<MenuItemState>     : std::integral_constant<MenuItemState,MenuItemState::Default>   {};
  template <> struct min_value<MenuItemState>     : std::integral_constant<MenuItemState,MenuItemState::Grayed>    {};



  //! \enum MenuItemType - Menu item types
  enum class MenuItemType : uint32_t
  {
    String       = MF_STRING,			      //!< [Windows 4.00] 
    Bitmap       = MF_BITMAP,			      //!< [Windows 4.00] 
    MenuBarBreak = MF_MENUBARBREAK,			//!< [Windows 4.00] 
    MenuBreak    = MF_MENUBREAK,			  //!< [Windows 4.00] 
    OwnerDraw    = MF_OWNERDRAW,			  //!< [Windows 4.00] 
    RadioCheck   = 0x00000200L,			    //!< [Windows 4.00] 
    Separator    = MF_SEPARATOR,			  //!< [Windows 4.00] 
    RightOrder   = 0x00002000L,			    //!< [Windows 4.00] 
    RightJustify = MF_RIGHTJUSTIFY,			//!< [Windows 4.00] 
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<MenuItemType>  : std::true_type  {};
  template <> struct is_contiguous<MenuItemType> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<MenuItemType>     : std::integral_constant<MenuItemType,MenuItemType::RightJustify>   {};
  template <> struct min_value<MenuItemType>     : std::integral_constant<MenuItemType,MenuItemType::String>         {};


} // WTL namespace



#endif // WTL_MENU_FLAGS_HPP


