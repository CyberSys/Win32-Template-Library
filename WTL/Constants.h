//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\Constants.h
//! \brief Defines wtl constants
//! \date 6 March 2015
//! \author Nick Crowley
//! Copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSTANTS_HPP
#define WTL_CONSTANTS_HPP

#include "WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var wtl::True - Win32 boolean TRUE
  constexpr ::BOOL  True = TRUE;      

  //! \var wtl::False - Win32 boolean FALSE
  constexpr ::BOOL  False = FALSE;    
}

#endif // WTL_CONSTANTS_HPP
