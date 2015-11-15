//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\MenuTraits.hpp
//! \brief Defines menu traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MENU_TRAITS_HPP
#define WTL_MENU_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>               //!< Handle
#include <wtl/traits/EncodingTraits.hpp>      //!< Encoding
#include <wtl/platform/MenuFlags.hpp>         //!< MenuType

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HMenu - Shared menu handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HMenu = Handle<::HMENU>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HMENU> - Encapsulates menu handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HMENU>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HMENU npos = defvalue<::HMENU>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::create
    //! Create empty menu
    //! 
    //! \param[in] type - Menu type
    //! \return NativeHandle<::HMENU> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to create handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HMENU> create(MenuType type) 
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
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::create
    //! Load menu from resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] instance - Instance containing menu
    //! \param[in] ident - Menu identifier
    //! \return NativeHandle<::HMENU> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static NativeHandle<::HMENU> create(HINSTANCE instance, ResourceId<ENC> ident) 
    { 
      // Load menu 
      if (::HMENU menu = WinAPI<ENC>::loadMenu(instance, ident))
        return { menu, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load menu");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::clone
    //! Clone handle
    //! 
    //! \param[in] menu - Menu handle
    //! \return NativeHandle<::HMENU> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HMENU> clone(NativeHandle<::HMENU> menu);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMENU>::destroy noexcept
    //! Release Menu handle
    //! 
    //! \param[in] menu - Menu handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HMENU> menu) noexcept
    {
      // Delete without checking if handle is valid
      switch (menu.Method)
      {
      case AllocType::Accquire: return ::DestroyMenu(menu.Handle) != False;
      case AllocType::Create:   return ::DestroyMenu(menu.Handle) != False;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_MENU_TRAITS_HPP

