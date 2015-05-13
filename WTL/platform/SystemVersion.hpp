//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\SystemVersion.hpp
//! \brief Provides Windows OS version info
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SYSTEM_VERSION_HPP
#define WTL_SYSTEM_VERSION_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"
#include "wtl/traits/EncodingTraits.hpp"
#include <utility>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct OperatingSystem - Encapsulates operating system info
  //! 
  //! \tparam ENC - Character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct OperatingSystem : getType<encoding_char_t<ENC>,::OSVERSIONINFOA,::OSVERSIONINFOW>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    WindowVersion  Version;         //!< Windows version identifier
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // OperatingSystem::OperatingSystem
    //! Create operating system data
    /////////////////////////////////////////////////////////////////////////////////////////
    OperatingSystem() : Version(WindowVersion::Future)
    {
      static const auto getVersion = getFunc<encoding_char_t<ENC>>(::GetVersionExA,::GetVersionExW);

      // Prepare
      dwOSVersionInfoSize = sizeof(::OSVERSIONINFO);

      // Query windows version
      if (getVersion(this))
        Version = identify(dwMajorVersion, dwMinorVersion);
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(OperatingSystem);       //!< Can be deep copied
    ENABLE_MOVE(OperatingSystem);       //!< Can be moved
    ENABLE_POLY(OperatingSystem);       //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // OperatingSystem::identify
    //! Identifies the operating system 
    //! 
    //! \param[in] major - Major version number
    //! \param[in] minor - Minor version number
    //! \return WindowVersion - Windows version identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    static WindowVersion identify(long32 major, long32 minor)
    {
      switch (major)
      {
      // [Win9x/NT3/NT4] Window 3, Win95, Win98, WinNT 3.5
      case 3:
      case 4: 
        return WindowVersion::WinNT;   

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
      default: 
        return WindowVersion::Future;
      }
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

}

#endif // WTL_SYSTEM_VERSION_HPP
