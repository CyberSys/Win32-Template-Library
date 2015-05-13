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
#include "wtl/traits/EnumTraits.hpp"      //!< is_attribute
#include "wtl/traits/EncodingTraits.hpp"  //!< Encoding
#include "wtl/platform/ResourceId.hpp"    //!< ResourceId
#include "wtl/platform/WindowFlags.hpp"   //!< WindowId, WindowStyle, WindowStyleEx
#include "wtl/utils/Point.hpp"         //!< Point
#include "wtl/utils/Size.hpp"          //!< Size

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
    static constexpr ::HWND npos = default<::HWND>(); 
    
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
    //! \tparam LEN - Title string length
    //! \tparam OBJ - Custom data type
    //!
    //! \param[in] instance - Module instance
    //! \param[in] const& wndClass - Window class atom
    //! \param[in] owner - [optional] Parent/owner window
    //! \param[in] object - Creation parameter (Typically Window object pointer)
    //! \param[in] menu - [optional] Window Menu
    //! \param[in] style - Window styles
    //! \param[in] ext - Extended styles
    //! \param[in] const& title - Window title
    //! \param[in] pos - Initial position
    //! \param[in] size - Initial size
    //! \return HAlloc<::HWND> - Created handle
    //! 
    //! \throw wtl::platform_error - Unable to create window
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename OBJ>
    static HAlloc<::HWND> create(::HINSTANCE instance, const ResourceId<ENC>& wndClass, ::HWND owner, OBJ* object, ::HMENU menu, WindowStyle style, WindowStyleEx ext, const CharArray<ENC,LEN>& title, PointL pos, SizeL size)
    { 
      using char_t = encoding_char_t<ENC>;    //!< Character encoding type

      // Lookup function
      auto createWindow = getFunc<char_t>(::CreateWindowExA,::CreateWindowExW);

      // Create window
      ::HWND hwnd = createWindow(enum_cast(ext), 
                                 wndClass.toString(), 
                                 title, 
                                 enum_cast(style), 
                                 pos.x, pos.y, size.width, size.height, 
                                 owner, 
                                 menu, 
                                 instance,     //!< Instance handle
                                 object);      //!< Pass object as parameter data
      // Success
      if (hwnd)
        return { hwnd, AllocType::Create };
      
      // Error: Failed  
      //throw platform_error(HERE, "Unable to create window: class='%s' title='%s'", CharArray<Encoding::ANSI>(wndClass), CharArray<Encoding::ANSI>(title));
      throw platform_error(HERE, "Unable to create window");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::create
    //! Create child window handle
    //! 
    //! \tparam ENC - String encoding
    //! \tparam LEN - Title string length
    //! \tparam OBJ - Custom data type
    //!
    //! \param[in] instance - Module instance
    //! \param[in] const& wndClass - Window class atom
    //! \param[in] parent - Parent window
    //! \param[in] object - Creation parameter (Typically Window object pointer)
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
    template <Encoding ENC, unsigned LEN, typename OBJ>
    static HAlloc<::HWND> create(::HINSTANCE instance, const ResourceId<ENC>& wndClass, ::HWND parent, OBJ* object, WindowId id, WindowStyle style, WindowStyleEx ext, const CharArray<ENC,LEN>& title, PointL pos, SizeL size)
    { 
      // Create child window
      return create(instance, wndClass, parent, object, reinterpret_cast<::HMENU>(enum_cast(id)), style, ext, title, pos, size);
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
      case AllocType::Create:   return ::DestroyWindow(wnd.Handle) != FALSE;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_WINDOW_TRAITS_HPP

