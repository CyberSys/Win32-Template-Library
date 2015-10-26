//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\DrawingFlags.hpp
//! \brief Defines flags for drawing related Win32 API functions
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SYSTEM_FLAGS_HPP
#define WTL_SYSTEM_FLAGS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"                  //!< is_attribute, is_contiguous
#include "wtl/utils/Default.hpp"                      //!< default_t
#include <utility>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // ----------------------------------- COMMON CONTROL VERSION ----------------------------------

  
  //! \enum ControlVersion - Defines common control library (ComCtl32) versions
  enum class ControlVersion : ulong32_t
  { 
    Explorer30 = 0x0400,        //!< Internet Explorer 3.00  (Windows 95)
    Explorer31 = 0x0470,        //!< Internet Explorer 3.01  (Windows NT 4, Windows 95 OSR2)

    Explorer40 = 0x0471,        //!< Internet Explorer 4.00 
    Explorer41 = 0x0472,        //!< Internet Explorer 4.01  (Windows 98)

    Explorer50 = 0x0580,        //!< Internet Explorer 5.00  (Windows 98 SE)
    Explorer51 = 0x0581,        //!< Internet Explorer 5.01  (Windows 2000)

    Explorer60 = 0x0582,        //!< Internet Explorer 6.00  (Windows XP)
    
    WinXp      = 0x0600,        //!< <Not packaged with OS> (Windows XP)
    Vista      = 0x0610,        //!< <Not packaged with OS> (Windows Vista)
  };

  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<ControlVersion>  : std::false_type  {};
  template <> struct is_contiguous<ControlVersion> : std::false_type  {};
  template <> struct default_t<ControlVersion>     : std::integral_constant<ControlVersion,ControlVersion::Explorer30>   {};
  
  // ----------------------------------- DATE FORMAT FLAGS ----------------------------------

  //! \enum DateFlags - Define GetDateFormat() flags
  enum class DateFlags : long32_t
  {
    ShortDate = 0x00000001,			      //!< Use short date picture
    LongDate = 0x00000002,			      //!< Use long date picture
    AltCalendar = 0x00000004,		      //!< Use alternate calendar (if any)
    YearMonth = 0x00000008,			      //!< [Windows 5.00] Use year month picture
    LtrReading = 0x00000010,			    //!< [Windows 5.00] Add marks for left to right reading order layout
    RtlReading = 0x00000020,			    //!< [Windows 5.00] Add marks for right to left reading order layout
    AutoLayout = 0x00000040,			    //!< [Windows 6.01] add appropriate marks for left-to-right or right-to-left reading order layout
  };

  //! Define traits: Non-Contiguous attribute
  template <> struct is_attribute<DateFlags>  : std::true_type  {};
  template <> struct is_contiguous<DateFlags> : std::false_type {};
  template <> struct default_t<DateFlags>     : std::integral_constant<DateFlags,DateFlags::ShortDate>   {};

  // ----------------------------------- RESOURCE TYPES ----------------------------------
  
  
  //! \enum ResourceType - Defines resource types
  enum class ResourceType : uint32_t
  {
    Cursor = 1,           //!< 
    Bitmap = 2,           //!< 
    Icon = 3,             //!< 
    Menu = 4,             //!< 
    Dialog = 5,           //!< 
    String = 6,           //!< 
    FontDir = 7,          //!< 
    Font = 8,             //!< 
    Accelerator = 9,      //!< 
    RcData = 10,          //!< 
    MessageTable = 11,    //!< 
    GroupCursor = 12,     //!< 
    GroupIcon = 14,       //!< 
    Version = 16,         //!< 
    DlgInclude = 17,      //!< 
    PlugPlay = 19,        //!< [Windows 4.00] 
    Vxd = 20,             //!< [Windows 4.00] 
    AniCursor = 21,       //!< [Windows 4.00] 
    AniIcon = 22,         //!< [Windows 4.00] 
    Html = 23,            //!< 
    Manifest = 24,        //!< 
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<ResourceType>  : std::false_type {};
  template <> struct is_contiguous<ResourceType> : std::false_type {};
  template <> struct default_t<ResourceType>     : std::integral_constant<ResourceType,ResourceType::Cursor>   {};
  
  
  // ----------------------------------- SHELL VERSION ----------------------------------


  //! \enum ShellVersion - Defines shell library (Shell32) versions
  enum class ShellVersion : ulong32_t
  { 
    Win95      = 0x0400,        //!< Windows 95 / NT4 (Internet Explorer 3.0)
    Explorer40 = 0x0471,        //!< Windows 95 / NT4 (Internet Explorer 4.0)
    Win98      = 0x0472,        //!< Windows 98 / 98SE
    Win2000    = 0x0500,        //!< Windows 2000
    WinXp      = 0x0600,        //!< Windows XP / Vista
    Win7       = 0x0610,        //!< Windows 7
    Future,                     //!< Future
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<ShellVersion>  : std::false_type  {};
  template <> struct is_contiguous<ShellVersion> : std::false_type  {};
  template <> struct default_t<ShellVersion>     : std::integral_constant<ShellVersion,ShellVersion::Win95>   {};

  // ----------------------------------- SYSTEM CURSORS ----------------------------------
  

  //! \enum SystemCursor - System cursor Ids
  enum class SystemCursor
  {
    Arrow = 32512,			  //!< 
    IBeam = 32513,			  //!< 
    Wait = 32514,			    //!< 
    Cross = 32515,			  //!< 
    Uparrow = 32516,			//!< 
    Size = 32640,			    //!< obsolete: use SizeAll
    Icon = 32641,			    //!< obsolete: use Arrow
    SizeNWSE = 32642,			//!< 
    SizeNESW = 32643,			//!< 
    SizeWE = 32644,			  //!< 
    SizeNS = 32645,			  //!< 
    SizeAll = 32646,			//!< 
    No = 32648,			      //!< 
    Hand = 32649,			    //!< [Windows 5.00]
    AppStarting = 32650,	//!< 
    Help = 32651,			    //!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemCursor>  : std::false_type  {};
  template <> struct is_contiguous<SystemCursor> : std::false_type  {};
  template <> struct default_t<SystemCursor>     : std::integral_constant<SystemCursor,SystemCursor::Arrow>   {};


  // ----------------------------------- SYSTEM ICONS ----------------------------------
  

  //! \enum SystemIcon - System icon Ids
  enum class SystemIcon
  {
    Application = 32512,			    //!< 
    Hand = 32513,			            //!< 
    Question = 32514,			        //!< 
    Exclamation = 32515,			    //!< 
    Asterisk = 32516,			        //!< 
    WinLogo = 32517,			        //!< 
    Shield = 32518,               //!< [Windows 6.00]
    Warning = Exclamation,			  //!< 
    Error = Hand,			            //!< 
    Information = Asterisk,			  //!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemIcon>  : std::false_type  {};
  template <> struct is_contiguous<SystemIcon> : std::false_type  {};
  template <> struct default_t<SystemIcon>     : std::integral_constant<SystemIcon,SystemIcon::Application>   {};

  
  // ----------------------------------- WINDOWS OS VERSION ----------------------------------
  

  //! \enum WindowVersion - Defines operating system (Kernel32) versions
  enum class WindowVersion : ulong32_t
  { 
    WinNT    = 0x0400,      //!< Windows 9x / NT4
    Win2000  = 0x0500,      //!< Windows 2000
    WinXp    = 0x0501,      //!< Windows XP 
    Win2003  = 0x0502,      //!< Windows Server 2003
    Vista    = 0x0600,      //!< Windows Vista
    Win7     = 0x0601,      //!< Windows 7
    Win8     = 0x0602,      //!< Windows 8
    Win81    = 0x0603,      //!< Windows 8.1
    Future,                 //!< Future
  };
  
  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<WindowVersion>  : std::false_type  {};
  template <> struct is_contiguous<WindowVersion> : std::false_type  {};
  template <> struct default_t<WindowVersion>     : std::integral_constant<WindowVersion,WindowVersion::WinNT>   {};


  

}

#endif  // WTL_SYSTEM_FLAGS_HPP
