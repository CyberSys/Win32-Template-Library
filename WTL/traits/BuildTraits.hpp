//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\BuildTraits.hpp
//! \brief Defines build traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUILD_TRAITS_HPP
#define WTL_BUILD_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"      //!< wtl::is_attribute, wtl::is_contiguous, wtl::enum_names,...

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum WindowVersion - Defines operating system (Kernel32) versions
  enum class WindowVersion : ulong32
  { 
    WinNT    = 0x0400,      //!< Windows 9x / NT4
    Win2000  = 0x0500,      //!< Windows 2000
    WinXp    = 0x0501,      //!< Windows XP 
    Win2003  = 0x0502,      //!< Windows Server 2003
    WinVista = 0x0600,      //!< Windows Vista
    Win7     = 0x0601,      //!< Windows 7
    Win8     = 0x0602,      //!< Windows 8
    Win81    = 0x0603,      //!< Windows 8.1
    Future,                 //!< Future
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<WindowVersion>  : std::false_type  {};
  template <> struct is_contiguous<WindowVersion> : std::false_type  {};

  //! Define limits traits
  template <> struct max_value<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::Future>  {};
  template <> struct min_value<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::WinNT>   {};


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct build_traits - Defines project build target traits 
  //! 
  //! \tparam T - Type used to specialize traits
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct build_traits;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct build_traits<void> - Defines project build target traits 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <> 
  struct build_traits<void>
  {
    //! \var target - Target operating system
    static constexpr WindowVersion  target = static_cast<WindowVersion>(_WIN32_WINNT);
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct enable_if_build_t - Defines an SFINAE expression for meeting a minimum build target version
  //! 
  //! \tparam MINIMUM - Minimum Windows operating system version 
  //! \tparam RETURN - [optional] Return type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <WindowVersion MINIMUM, typename RETURN = void>
  using enable_if_build_t = std::enable_if_t<(build_traits<void>::target >= MINIMUM), RETURN>;

  
} //namespace wtl
#endif // WTL_BUILD_TRAITS_HPP

