////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\MenuTraits.hpp
//! \brief Defines menu traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MENU_TRAITS_HPP
#define WTL_MENU_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HMENU> - Encapsulates menu handle allocation
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HMENU>
  {
    //! \var npos - Invalid handle sentinel value
    static const ::HMENU npos; 
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::create
    //! Create empty menu
    //! 
    //! \param[in] type - Menu type
    //! \return HAlloc<::HMENU> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to create handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HMENU> create(MenuType type) 
    { 
      switch (type)
      {
      // [WINDOW] Create menu
      case MenuType::Window:
        if (::HMENU menu = ::CreateMenu())
          return { menu, AllocType::Create };
        break;

      // [POPUP] Create menu
      case MenuType::Popup:
        if (::HMENU menu = ::CreatePopupMenu())
          return { menu, AllocType::Create };
        break;
      }

      // Error: Failed  
      throw platform_error(HERE, "Unable to create menu");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::create
    //! Load menu from resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] instance - Instance containing menu
    //! \param[in] ident - Menu identifier
    //! \return HAlloc<::HMENU> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<::HMENU> create(HINSTANCE instance, ResourceId<ENC> ident) 
    { 
      // Load menu 
      if (::HMENU menu = getFunc<ENC>(::LoadMenuA,::LoadMenuW)(instance, ident))
        return { menu, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load menu");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::clone
    //! Clone handle
    //! 
    //! \param[in] menu - Menu handle
    //! \return HAlloc<::HMENU> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HMENU> clone(HAlloc<::HMENU> menu);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::destroy noexcept
    //! Release Menu handle
    //! 
    //! \param[in] menu - Menu handle
    //! \return bool - True iff closed successfully
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HMENU> menu) noexcept
    {
      // Delete without checking if handle is valid
      switch (menu.Method)
      {
      case AllocType::Accquire: return ::DestroyMenu(menu.Handle) != FALSE;
      case AllocType::Create:   return ::DestroyMenu(menu.Handle) != FALSE;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
  };


  //! \alias HMenu - Shared menu handle
  using HMenu = Handle<::HMENU>;

  
} //namespace wtl
#endif // WTL_MENU_TRAITS_HPP

