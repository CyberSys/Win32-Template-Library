////////////////////////////////////////////////////////////////////////////////
//! \file wtl\win32\SystemVersion.hpp
//! \brief Provides Windows OS version info
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SYSTEM_VERSION_HPP
#define WTL_SYSTEM_VERSION_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
 
  //! \enum WindowVersion - Defines dialog control ids
  enum class WindowVersion : ulong32
  { 
    Legacy,     //!< Legacy
    Win2000,    //!< Windows 2000
    WinXp,      //!< Windows XP 
    Win2003,    //!< Windows Server 2003
    WinVista,   //!< Windows Vista
    Win7,       //!< Windows 7
    Win8,       //!< Windows 8
    Future,     //!< Future
  };
  

  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<WindowVersion>  : std::false_type  {};
  template <> struct is_contiguous<WindowVersion> : std::true_type   {};

  //! Define limits traits
  template <> struct max_value<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::Future>   {};
  template <> struct min_value<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::Legacy>   {};


  //! \struct OperatingSystem - Encapsulates operating system info
  template <Encoding ENC>
  struct OperatingSystem : getType<encoding_char_t<ENC>,OSVERSIONINFOA,OSVERSIONINFOW>
  {
    ///////////////////////////////////////////////////////////////////////////////
    // OperatingSystem::OperatingSystem
    //! Create operating system data
    ///////////////////////////////////////////////////////////////////////////////
    OperatingSystem() : Version(WindowVersion::Future)
    {
      static const auto getVersion = getFunc<encoding_char_t<ENC>>(::GetVersionExA,::GetVersionExW);

      // Prepare
      dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

      // Query windows version
      if (getVersion(this))
        Version = identify(dwMajorVersion, dwMinorVersion);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // OperatingSystem::identify
    //! Identifies the operating system 
    //! 
    //! \param[in] major - Major version number
    //! \param[in] minor - Minor version number
    //! \return WindowVersion - Windows version identifier
    ///////////////////////////////////////////////////////////////////////////////
    static WindowVersion identify(long32 major, long32 minor)
    {
      switch (major)
      {
      // [WINDOWS 3/4] Window 3, Win95, Win98, WinNT 3.5
      case 3:
      case 4: return WindowVersion::Legacy;   

      // [WINDOWS NT 5] Windows 2000 or Windows XP
      case 5:
        switch (major)
        {
        case 0:  return WindowVersion::Win2000;   
        case 1:  return WindowVersion::WinXp;     
        case 2:  return WindowVersion::Win2003;   
        }
        break;

      // [WINDOWS NT 6] Windows Vista, 7, or newer
      case 6:
        switch (minor)
        {
        case 0:  return WindowVersion::WinVista;   
        case 1:  return WindowVersion::Win7;       
        default: return WindowVersion::Future;     
        }
        break;

      // [WINDOWS NT 7] Windows 8
      case 7:
        switch (minor)
        {
        case 0:  return WindowVersion::Win8;       
        default: return WindowVersion::Future;     
        }
        break;

      // [WINDOWS NT ?] Newer
      default: return WindowVersion::Future;
      }
    }

    WindowVersion  Version;     //!< Windows version identifier
  };

}

#endif // WTL_SYSTEM_VERSION_HPP
