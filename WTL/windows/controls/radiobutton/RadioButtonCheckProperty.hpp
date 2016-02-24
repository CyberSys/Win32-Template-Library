//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\properties\RadioButtonCheckProperty.hpp
//! \brief Separate implementation for the RadioButton control 'Check' property (resolves circular dependency)
//! \date 6 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RADIOBUTTON_CHECKED_HPP
#define WTL_RADIOBUTTON_CHECKED_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                                         //!< enum_cast
#include <wtl/windows/controls/radiobutton/RadioButtonCheckProperty.h>    //!< RadioButtonCheckPropertyImpl
#include <wtl/windows/controls/radiobutton/RadioButton.hpp>               //!< RadioButton

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // RadioButtonCheckPropertyImpl::get const
  //! Get the button state
  //! 
  //! \return value_t - Current state if button exists, otherwise initial state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename RadioButtonCheckPropertyImpl<ENC>::value_t  RadioButtonCheckPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query check iff button exists
    if (this->Window.exists())
    {
      // Debug
      //cdebug << object_info(__func__, "ButtonMessage::GetCheck", enum_cast<value_t>(this->Window.send(ButtonMessage::GetCheck).Result),
      //                                "ButtonMessage::GetState", enum_cast<value_t>(this->Window.send(ButtonMessage::GetState).Result),
      //                                "IsDlgButtonChecked()", ::IsDlgButtonChecked(*this->Window.parent(), enum_cast(this->Window.Ident())) ) 
      //       << std::endl;

      //return this->Window.send(ButtonMessage::GetCheck).Result != 0; 
      //return ::IsDlgButtonChecked(*this->Window.parent(), enum_cast(this->Window.Ident())) != BST_UNCHECKED;
    }

    // Return initial value
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // RadioButtonCheckPropertyImpl::onCreate 
  //! Called during button creation to set the initial state
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Routing indicating message was handled
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  RadioButtonCheckPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // [CHECKED] Set only if this radio is checked
    if (base::get())
      ::CheckRadioButton(*this->Window.parent(), enum_cast(First), enum_cast(Last), enum_cast(this->Window.Ident()));
      //this->Window.send(ButtonMessage::SetCheck, BST_CHECKED); 
      //this->Window.send(ButtonMessage::SetCheck, base::get() ? BST_CHECKED : 0); 

    // Accept button creation
    return {MsgRoute::Handled, 0};
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // RadioButtonCheckPropertyImpl::set 
  //! Set the state iff button exists, otherwise sets the initial state
  //! 
  //! \param[in] state - RadioButton state
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  RadioButtonCheckPropertyImpl<ENC>::set(value_t state) 
  {
    // [EXISTS] Set state iff button exists
    if (this->Window.exists() && state)
      ::CheckRadioButton(*this->Window.parent(), enum_cast(First), enum_cast(Last), enum_cast(this->Window.Ident()));   //this->Window.send(ButtonMessage::SetCheck, state ? BST_CHECKED : 0); 
    
    // Save initial state
    base::set(state);
  }
  
      
} // namespace wtl

#endif // WTL_RADIOBUTTON_CHECKED_HPP
