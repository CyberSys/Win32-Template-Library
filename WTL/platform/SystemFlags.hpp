//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\DrawingFlags.hpp
//! \brief Defines flags for drawing related Win32 API functions
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DRAWING_FLAGS_HPP
#define WTL_DRAWING_FLAGS_HPP

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
  



}

#endif  // WTL_DRAWING_FLAGS_HPP
