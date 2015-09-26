//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\WindowTraits.hpp
//! \brief Defines window traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TRAITS_HPP
#define WTL_WINDOW_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"           //!< Handle
#include "wtl/utils/CharArray.hpp"        //!< CharArray
#include "wtl/utils/String.hpp"           //!< String
#include "wtl/traits/EnumTraits.hpp"      //!< is_attribute
#include "wtl/traits/EncodingTraits.hpp"  //!< Encoding
#include "wtl/platform/ResourceId.hpp"    //!< ResourceId
#include "wtl/platform/WindowFlags.hpp"   //!< WindowId, WindowStyle, WindowStyleEx
#include "wtl/utils/Point.hpp"            //!< Point
#include "wtl/utils/Size.hpp"             //!< Size
#include "wtl/windows/WindowClass.hpp"    //!< WindowClass

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HWnd - Shared window handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HWnd = Handle<::HWND>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HWND> - Encapsulates window handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HWND>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HWND npos = defvalue<::HWND>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::create
    //! Create window handle
    //! 
    //! \tparam ENC - String encoding
    //! \tparam OBJ - Window object type
    //!
    //! \param[in] const& wndClass - Win32 Window class 
    //! \param[in] object - WTL Window object
    //! \param[in] owner - [optional] Parent/owner window
    //! \param[in] style - Window styles
    //! \param[in] ext - Extended styles
    //! \param[in] menu - [optional] Window Menu if any, otherwise nullptr
    //! \param[in] const& title - Window title
    //! \param[in] pos - Initial position
    //! \param[in] size - Initial size
    //! \return HAlloc<::HWND> - Created handle
    //! 
    //! \throw wtl::platform_error - Unable to create window
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, typename OBJ>
    static HAlloc<::HWND> create(const WindowClass<ENC>& wndClass, OBJ& object, ::HWND owner, WindowStyle style, WindowStyleEx exStyle, ::HMENU menu, const String<ENC>& title, PointL pos, SizeL size)
    { 
      using char_t = encoding_char_t<ENC>;    //!< Character encoding type

      // Lookup function
      auto createWindow = getFunc<char_t>(::CreateWindowExA,::CreateWindowExW);

      // Create window
      if (::HWND hwnd = createWindow(enum_cast(exStyle),                        //!< Extended style
                                     wndClass.Name.toString(),                  //!< Class name
                                     title.c_str(),                             //!< Text
                                     enum_cast(style),                          //!< Style
                                     pos.x, pos.y, size.width, size.height,     //!< Size/Position
                                     owner,                                     //!< Owner/parent
                                     menu,                                      //!< Menu or ChildID
                                     wndClass.Instance,                         //!< Module containing class
                                     &object))                                  //!< Associate WTL window object
        // Success!
        return { hwnd, AllocType::Create };
      
      // [ERROR] Failed to create
      throw platform_error(HERE, "Unable to create window");    //throw platform_error(HERE, "Unable to create window: class='%s' title='%s'", CharArray<Encoding::ANSI>(wndClass), CharArray<Encoding::ANSI>(title));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::create
    //! Create child window handle
    //! 
    //! \tparam ENC - String encoding
    //! \tparam OBJ - Window object type
    //!
    //! \param[in] const& wndClass - Win32 Window class 
    //! \param[in] &object - WTL window object
    //! \param[in] parent - Parent window
    //! \param[in] id - Window id
    //! \param[in] style - Window styles
    //! \param[in] ext - Extended styles
    //! \param[in] const& title - Window title
    //! \param[in] pos - Initial position
    //! \param[in] size - Initial size
    //! \return HAlloc<::HWND> - Created handle
    //! 
    //! \throw wtl::platform_error - Unable to create window
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, typename OBJ>
    static HAlloc<::HWND> create(const WindowClass<ENC>& wndClass, OBJ& object, ::HWND parent, WindowId id, WindowStyle style, WindowStyleEx ext, const String<ENC>& title, PointL pos, SizeL size)
    { 
      // Create child window
      return create(wndClass, object, parent, style, ext, reinterpret_cast<::HMENU>(enum_cast(id)), title, pos, size);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::clone
    //! Clone handle
    //! 
    //! \param[in] wnd - Handle
    //! \return HAlloc<::HWND> - Duplicate of handle
    //!
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HWND> clone(HAlloc<::HWND> wnd);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] wnd - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HWND> wnd) noexcept
    {
      // Delete without checking if handle is valid
      switch (wnd.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return ::DestroyWindow(wnd.Handle) != False;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_WINDOW_TRAITS_HPP

