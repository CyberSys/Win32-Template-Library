////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\SystemVersion.hpp
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
  
  
  //! \enum ControlVersion - Defines common control library (ComCtl32) versions
  enum class ControlVersion : ulong32
  { 
    Explorer30 = 0x0400,        //!< Internet Explorer 3.00  (Windows 95)
    Explorer31 = 0x0470,        //!< Internet Explorer 3.01  (Windows NT 4, Windows 95 OSR2)

    Explorer40 = 0x0471,        //!< Internet Explorer 4.00 
    Explorer41 = 0x0472,        //!< Internet Explorer 4.01  (Windows 98)

    Explorer50 = 0x0580,        //!< Internet Explorer 5.00  (Windows 98 SE)
    Explorer51 = 0x0581,        //!< Internet Explorer 5.01  (Windows 2000)

    Explorer60 = 0x0582,        //!< Internet Explorer 6.00  (Windows XP)
    
    WinXp      = 0x0600,        //!< <Not packaged with OS> (Windows XP)
    WinVista   = 0x0610,        //!< <Not packaged with OS> (Windows Vista)
  };
  

  //! \enum ShellVersion - Defines shell library (Shell32) versions
  enum class ShellVersion : ulong32
  { 
    Win95      = 0x0400,        //!< Windows 95 / NT4 (Internet Explorer 3.0)
    Explorer40 = 0x0471,        //!< Windows 95 / NT4 (Internet Explorer 4.0)
    Win98      = 0x0472,        //!< Windows 98 / 98SE
    Win2000    = 0x0500,        //!< Windows 2000
    WinXp      = 0x0600,        //!< Windows XP / Vista
    Win7       = 0x0610,        //!< Windows 7
    Future,                     //!< Future
  };



  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<WindowVersion>  : std::false_type  {};
  template <> struct is_contiguous<WindowVersion> : std::true_type   {};

  //! Define limits traits
  template <> struct max_value<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::Future>  {};
  template <> struct min_value<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::Win95>   {};


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
      // [Win9x/NT3/NT4] Window 3, Win95, Win98, WinNT 3.5
      case 3:
      case 4: return WindowVersion::WinNT;   

      // [WINDOWS 5] Windows 2000 or Windows XP
      case 5:
        switch (major)
        {
        case 0:  return WindowVersion::Win2000;   
        case 1:  return WindowVersion::WinXp;     
        case 2:  return WindowVersion::Win2003;   
        }
        break;

      // [WINDOWS 6] Windows Vista, 7, 8, or future
      case 6:
        switch (minor)
        {
        case 0:  return WindowVersion::WinVista;   
        case 1:  return WindowVersion::Win7; 
        case 2:  return WindowVersion::Win8; 
        case 3:  return WindowVersion::Win81; 
        default: return WindowVersion::Future;     
        }
        break;

      // [FUTURE] Newer
      default: return WindowVersion::Future;
      }
    }

    WindowVersion  Version;     //!< Windows version identifier
  };

}

#endif // WTL_SYSTEM_VERSION_HPP
