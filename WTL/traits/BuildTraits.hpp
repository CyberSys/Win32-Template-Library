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
#include "wtl/platform/SystemFlags.hpp"   //!< WindowVersion
#include <type_traits>                    //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct build_traits - Defines project build target traits 
  //! 
  //! \tparam T - Type used to specialize traits
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T> 
  struct build_traits   /* Undefined */
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var target - Target operating system
    static constexpr WindowVersion  target = static_cast<WindowVersion>(_WIN32_WINNT);
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(build_traits);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(build_traits);     //!< Cannot instantiate
    DISABLE_MOVE(build_traits);     //!< Cannot instantiate
    DISABLE_DTOR(build_traits);     //!< Cannot instantiate
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

