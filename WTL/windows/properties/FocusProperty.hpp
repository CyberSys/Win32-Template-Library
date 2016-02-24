//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\FocusProperty.hpp
//! \brief Separate implementation for 'Focus' window property (resolves circular dependency)
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_FOCUS_PROPERTY_HPP
#define WTL_WINDOW_FOCUS_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/properties/FocusProperty.h>        //!< FocusProperty
#include <wtl/windows/Window.hpp>                         //!< Window

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // FocusPropertyImpl::get const
  //! Query whether window has keyboard focus
  //! 
  //! \return value_t - True iff window has keyboard focus
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename FocusPropertyImpl<ENC>::value_t  FocusPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query whether window has keyboard focus
    if (this->Window.exists())
      return this->Window.handle() == ::GetFocus();
        
    // Return false
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // FocusPropertyImpl::onCreate 
  //! Called during window creation to set the initial font
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Routing indicating message was handled 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult FocusPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    static constexpr bool redraw = true;

    // Set keyboard focus
    if (base::get())
      this->Window.focus(); 
    
    // [Accept window creation]
    return {MsgRoute::Handled, 0};
  }
      
} // namespace wtl

#endif // WTL_WINDOW_FOCUS_PROPERTY_HPP

