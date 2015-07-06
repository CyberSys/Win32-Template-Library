//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowEnabledProperty.cpp
//! \brief Implementation for 'WindowEnabled' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/casts/BooleanCast.hpp"                              //!< BooleanCast
#include "wtl/windows/properties/WindowEnabledProperty.hpp"       //!< WindowEnabledProperty
#include "wtl/windows/WindowBase.hpp"                             //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowEnabledPropertyImpl::get const
  //! Get the window state
  //! 
  //! \return value_t - Current state if window exists, otherwise 'initial' state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowEnabledPropertyImpl<ENC>::value_t  WindowEnabledPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window state
    if (this->Window.exists())
      return boolean_cast(::IsWindowVisible(this->Window));

    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowEnabledPropertyImpl::set 
  //! Set the current window state iff window exists, otherwise 'initial' state
  //! 
  //! \param[in] state - Window state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowEnabledPropertyImpl<ENC>::set(value_t state) 
  {
    // Set window state
    if (this->Window.exists() && !::EnableWindow(Window, boolean_cast(state)))
      throw platform_error(HERE, "Unable to set window state");

    // Update value
    base::set(state);
  }

      
} // namespace wtl

