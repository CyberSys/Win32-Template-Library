//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\SystemVersion.hpp
//! \brief Encapsulates platform version info
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SYSTEM_VERSION_HPP
#define WTL_SYSTEM_VERSION_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"      //!< Encoding
#include "wtl/utils/CharArray.hpp"            //!< CharArray
#include "wtl/platform/SystemFlags.hpp"       //!< WindowVersion
#include <utility>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct SystemVersion - Encapsulates operating system info
  //! 
  //! \tparam ENC - Character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct SystemVersion : protected choose_t<ENC,::OSVERSIONINFOA,::OSVERSIONINFOW>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  protected:
    //! \alias base - Define base type
    using base = choose_t<ENC,::OSVERSIONINFOA,::OSVERSIONINFOW>;

  public:    
    //! \alias type - Define own type
    using type = SystemVersion<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    WindowVersion       Ident;         //!< Windows version identifier
    CharArray<ENC,64>   LongName;      //!< Long name
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // SystemVersion::SystemVersion
    //! Create operating system data
    /////////////////////////////////////////////////////////////////////////////////////////
    SystemVersion() : Ident(WindowVersion::Future)
    {
      // Prepare
      this->dwOSVersionInfoSize = sizeof(base);

      // Query windows version
      if (WinAPI<ENC>::getVersion(this))
        Ident = identify(this->dwMajorVersion, this->dwMinorVersion);

      // Format long name
      LongName.format(L"%s %s (v%d.%d)", name(), this->szCSDVersion, this->dwMajorVersion, this->dwMinorVersion);
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(SystemVersion);       //!< Can be deep copied
    ENABLE_MOVE(SystemVersion);       //!< Can be moved
    ENABLE_POLY(SystemVersion);       //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // SystemVersion::identify
    //! Identifies the operating system 
    //! 
    //! \param[in] major - Major version number
    //! \param[in] minor - Minor version number
    //! \return WindowVersion - Windows version identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    static WindowVersion identify(long32_t major, long32_t minor)
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
        case 0:  return WindowVersion::Vista;   
        case 1:  return WindowVersion::Win7; 
        case 2:  return WindowVersion::Win8; 
        case 3:  return WindowVersion::Win81; 
        default: return WindowVersion::Future;     
        }
        break;
      }

      // [FUTURE] Newer
      return WindowVersion::Future;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:   
    /////////////////////////////////////////////////////////////////////////////////////////
    // SystemVersion::fullname const
    //! Get the full name
    //! 
    //! \return const char_t* - Operating system name
    /////////////////////////////////////////////////////////////////////////////////////////
    const char_t* fullname() const
    {
      // Return full name
      return LongName.c_str();
    }
     
    /////////////////////////////////////////////////////////////////////////////////////////
    // SystemVersion::ident const
    //! Get the version identifier
    //! 
    //! \return WindowVersion - Operating system version
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowVersion  ident() const
    {
      // Return ident
      return Ident;
    }
     
    /////////////////////////////////////////////////////////////////////////////////////////
    // SystemVersion::name const
    //! Get the short name
    //! 
    //! \return const char_t* - Operating system name
    //!
    //! \throw wtl::logic_error - Unrecognised window version
    /////////////////////////////////////////////////////////////////////////////////////////
    const char_t* name() const
    {
      // Return full name
      switch (Ident)
      {
      case WindowVersion::WinNT:   return L"Windows 2000";
      case WindowVersion::Win2000: return L"Windows 2000";
      case WindowVersion::WinXp:   return L"Windows XP";
      case WindowVersion::Win2003: return L"Windows Server 2003";
      case WindowVersion::Vista:   return L"Windows Vista";
      case WindowVersion::Win7:    return L"Windows 7";
      case WindowVersion::Win8:    return L"Windows 8";
      case WindowVersion::Win81:   return L"Windows 8.1";
      case WindowVersion::Future:  return L"Windows Future";
      }

      // Error: 
      throw logic_error(HERE, "Unrecognised window version");
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

}

#endif // WTL_SYSTEM_VERSION_HPP
