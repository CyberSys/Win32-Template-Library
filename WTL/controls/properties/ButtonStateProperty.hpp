//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\ButtonStateProperty.hpp
//! \brief Separate implementation for the Button control 'State' property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_STATE_PROPERTY_HPP
#define WTL_BUTTON_STATE_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/controls/properties/ButtonStateProperty.h>    //!< ButtonStatePropertyImpl
#include <wtl/controls/Button.hpp>                          //!< Button

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ButtonStatePropertyImpl::get const
  //! Get the button state
  //! 
  //! \return value_t - Current state if button exists, otherwise initial state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename ButtonStatePropertyImpl<ENC>::value_t  ButtonStatePropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query state iff button exists
    if (this->Window.exists())
    {
      int32_t s = this->Window.template send<ButtonMessage::GetState>().Result;
      value_t val = enum_cast<value_t>(s);
      cdebug << "Value=" << val << std::endl;
      return val;
      //return enum_cast<value_t>( this->Window.template send<ButtonMessage::GetState>().Result ); 
    }

    // Return initial value
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  
      
} // namespace wtl

#endif // WTL_BUTTON_STATE_PROPERTY_HPP
