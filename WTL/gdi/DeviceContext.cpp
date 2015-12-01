//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\DeviceContext.hpp
//! \brief Provides storage for static device context fields
//! \date 7 March 2015
//! \author Nick Crowley
//! \copyright © Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "DeviceContext.hpp"
#include <wtl/utils/Default.hpp>
#include <functional>

//////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
//////////////////////////////////////////////////////////////////////////////////////////
namespace wtl
{
  // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
  // ----------------------------------- REPRESENTATION -----------------------------------
  
  //! \var DeviceContext::ScreenDC - Screen device context
  DeviceContext  DeviceContext::ScreenDC = defvalue<HDeviceContext>();
  
  // ------------------------------------ CONSTRUCTION ------------------------------------
	
  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

  // ----------------------------------- STATIC METHODS -----------------------------------

  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  // ----------------------------------- MUTATOR METHODS ----------------------------------

}
