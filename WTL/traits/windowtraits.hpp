////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\WindowTraits.hpp
//! \brief Defines window traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_TRAITS_HPP
#define WTL_WINDOW_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum WindowId - Represents a Window Id
  enum class WindowId : uint16
  {
    Ok = 1,			        //!< 'Ok' button
    Cancel = 2,			    //!< 'Cancel' button
    Abort = 3,			    //!< 'Abort' button
    Retry = 4,			    //!< 'Retry' button
    Ignore = 5,			    //!< 'Ignore' button
    Yes = 6,			      //!< 'Yes' button
    No = 7,			        //!< 'No' button
    Close = 8,			    //!< 'Close' button
    Help = 9,			      //!< 'Help' button
    TryAgain = 10,			//!< [Windows 5.00] 'Try Again' button
    Continue = 11,			//!< [Windows 5.00] 'Continue' button
    User = 0x0100,      //!< User defined
    Timeout = 32000,		//!< [Windows 5.01] Message box timed out
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<WindowId>  : std::false_type  {};
  template <> struct is_contiguous<WindowId> : std::false_type  {};

  //! Define limits traits
  template <> struct max_value<WindowId>     : std::integral_constant<WindowId,WindowId::Timeout>   {};
  template <> struct min_value<WindowId>     : std::integral_constant<WindowId,WindowId::Ok>        {};


  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::window_id
  //! Creates a strongly typed window id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing window id
  //! \return WindowId - WindowId representation of 'id'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = std::enable_if_t<std::is_integral<VALUE>::value || std::is_enum<VALUE>::value>>
  WindowId window_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<WindowId>( static_cast<std::underlying_type_t<WindowId>>(id) );
  }
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HWND> - Encapsulates window handle allocation
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HWND>
  {
    //! \var npos - Invalid handle sentinel value
    static const ::HWND npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::create
    //! Create window handle
    //! 
    //! \tparam ENC - String encoding
    //! \tparam LEN - Title string length
    //! \tparam OBJ - Custom data type
    //!
    //! \param[in] instance - Module instance
    //! \param[in] const& wndClass - Window class atom
    //! \param[in] object - Creation parameter (Typically Window object pointer)
    //! \param[in] style - Window styles
    //! \param[in] ext - Extended styles
    //! \param[in] const& title - Window title
    //! \param[in] const& rc - Initial position
    //! \param[in] parent - Parent window
    //! \param[in] menu - Window Menu
    //! \return HAlloc<::HWND> - Created handle
    //! 
    //! \throw wtl::platform_error - Unable to create window
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename OBJ>
    static HAlloc<::HWND> create(HINSTANCE instance, const ResourceId<ENC>& wndClass, OBJ* object, WindowStyle style, WindowStyleEx ext, const CharArray<ENC,LEN>& title, const Rect<int32>& rc, ::HWND parent, HMENU menu)
    { 
      using char_t = encoding_char_t<ENC>;    //!< Character encoding type

      // Lookup function
      auto createWindow = getFunc<char_t>(::CreateWindowExA,::CreateWindowExW);

      // Create window
      ::HWND hwnd = createWindow(enum_cast(ext), 
                               wndClass.toString(), 
                               title, 
                               enum_cast(style), 
                               rc.left, rc.top, rc.width(), rc.height(), 
                               parent, 
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
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::create
    //! Create child window handle
    //! 
    //! \tparam ENC - String encoding
    //! \tparam LEN - Title string length
    //! \tparam OBJ - Custom data type
    //!
    //! \param[in] instance - Module instance
    //! \param[in] const& wndClass - Window class atom
    //! \param[in] object - Creation parameter (Typically Window object pointer)
    //! \param[in] id - Window id
    //! \param[in] style - Window styles
    //! \param[in] ext - Extended styles
    //! \param[in] const& title - Window title
    //! \param[in] const& rc - Initial position
    //! \param[in] parent - Parent window
    //! \return HAlloc<::HWND> - Created handle
    //! 
    //! \throw wtl::platform_error - Unable to create window
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN, typename OBJ>
    static HAlloc<::HWND> create(HINSTANCE instance, const ResourceId<ENC>& wndClass, OBJ* object, WindowId id, WindowStyle style, WindowStyleEx ext, const CharArray<ENC,LEN>& title, const Rect<int32>& rc, ::HWND parent)
    { 
      // Create child window
      return create(instance, wndClass, object, style, ext, title, rc, parent, reinterpret_cast<HMENU>(enum_cast(id)));
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::clone
    //! Clone handle
    //! 
    //! \param[in] wnd - Handle
    //! \return HAlloc<::HWND> - Duplicate of handle
    //!
    //! \throw wtl::platform_error - Failed to clone handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HWND> clone(HAlloc<::HWND> wnd);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HWND>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] wnd - Handle
    //! \return bool - True iff closed successfully
    ///////////////////////////////////////////////////////////////////////////////
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
  };

  
  //! \alias HWnd - Window handle
  using HWnd = Handle<HWND>;


} //namespace wtl
#endif // WTL_WINDOW_TRAITS_HPP

