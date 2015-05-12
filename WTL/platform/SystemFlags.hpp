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
#include "wtl/traits/EnumTraits.hpp"
#include <utility>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // ----------------------------------- RESOURCE TYPES ----------------------------------
  
  
  //! \enum ResourceType - Defines resource types
  enum class ResourceType : uint32
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

  //! Define limits traits
  template <> struct max_value<ResourceType>     : std::integral_constant<ResourceType,ResourceType::Cursor>     {};
  template <> struct min_value<ResourceType>     : std::integral_constant<ResourceType,ResourceType::Manifest>   {};
  

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
  
  //! Define limits traits
  template <> struct max_value<SystemCursor>     : std::integral_constant<SystemCursor,SystemCursor::Help>  {};
  template <> struct min_value<SystemCursor>     : std::integral_constant<SystemCursor,SystemCursor::Arrow> {};


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
  
  //! Define limits traits
  template <> struct max_value<SystemIcon>     : std::integral_constant<SystemIcon,SystemIcon::Shield>      {};
  template <> struct min_value<SystemIcon>     : std::integral_constant<SystemIcon,SystemIcon::Application> {};
  
  // ----------------------------------- WINDOWS OS VERSION ----------------------------------
  
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


  

}

#endif  // WTL_SYSTEM_FLAGS_HPP
