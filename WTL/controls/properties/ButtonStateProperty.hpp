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
      enum_cast<value_t>( this->Window.template send<ButtonMessage::GetState>().Result ); 

    // Return initial value
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // ButtonStatePropertyImpl::onCreate 
  //! Called during button creation to set the initial state
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Returns 0 to accept button creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  ButtonStatePropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // [EXISTS] Set state iff button exists
    if (this->Window.exists())
      this->Window.template send<ButtonMessage::SetState>(); 

    // Accept button creation
    return 0;
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // ButtonStatePropertyImpl::set 
  //! Set the state iff button exists, otherwise sets the initial state
  //! 
  //! \param[in] state - Button state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  ButtonStatePropertyImpl<ENC>::set(value_t state) 
  {
    // [EXISTS] Set state iff button exists
    if (this->Window.exists())
      this->Window.template send<ButtonMessage::SetImage>(); 
    
    // Save initial state
    base::set(state);
  }
  
      
} // namespace wtl

#endif // WTL_BUTTON_STATE_PROPERTY_HPP
