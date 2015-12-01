//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\properties\CheckBoxCheckProperty.hpp
//! \brief Separate implementation for the CheckBox control 'Check' property (resolves circular dependency)
//! \date 22 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CHECKBOX_CHECKED_PROPERTY_HPP
#define WTL_CHECKBOX_CHECKED_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/controls/properties/CheckBoxCheckProperty.h>    //!< CheckBoxCheckPropertyImpl
#include <wtl/windows/controls/CheckBox.hpp>                          //!< CheckBox

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // CheckBoxCheckPropertyImpl::get const
  //! Get the button state
  //! 
  //! \return value_t - Current state if button exists, otherwise initial state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename CheckBoxCheckPropertyImpl<ENC>::value_t  CheckBoxCheckPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query check iff button exists
    if (this->Window.exists())
      return enum_cast<value_t>( this->Window.template send<ButtonMessage::GetCheck>().Result ); 

    // Return initial value
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // CheckBoxCheckPropertyImpl::onCreate 
  //! Called during button creation to set the initial state
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Routing indicating message was handled
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  CheckBoxCheckPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // [EXISTS] Set check iff button exists
    if (this->Window.exists())
      this->Window.template send<ButtonMessage::SetCheck>(enum_cast(base::get())); 

    // Accept button creation
    return {MsgRoute::Handled, 0};
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // CheckBoxCheckPropertyImpl::set 
  //! Set the state iff button exists, otherwise sets the initial state
  //! 
  //! \param[in] state - CheckBox state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  CheckBoxCheckPropertyImpl<ENC>::set(value_t state) 
  {
    // [EXISTS] Set state iff button exists
    if (this->Window.exists())
      this->Window.template send<ButtonMessage::SetCheck>(enum_cast(state)); 
    
    // Save initial state
    base::set(state);
  }
  
      
} // namespace wtl

#endif // WTL_CHECKBOX_CHECKED_PROPERTY_HPP
