//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\properties\ButtonIconProperty.hpp
//! \brief Separate implementation for the Button control 'Icon' property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_ICON_PROPERTY_HPP
#define WTL_BUTTON_ICON_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/controls/properties/ButtonIconProperty.h>    //!< ButtonIconPropertyImpl
#include <wtl/windows/controls/Button.hpp>                         //!< Button

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ButtonIconPropertyImpl::get const
  //! Get the button icon
  //! 
  //! \return value_t - Current icon if button exists, otherwise initial icon
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename ButtonIconPropertyImpl<ENC>::value_t  ButtonIconPropertyImpl<ENC>::get() const 
  {
    // Return shared handle
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // ButtonIconPropertyImpl::onCreate 
  //! Called during button creation to set the initial icon
  //! 
  //! \param[in,out] &args - Message arguments 
  //! \return LResult - Returns 0 to accept button creation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  LResult  ButtonIconPropertyImpl<ENC>::onCreate(CreateWindowEventArgs<ENC>& args)
  {
    // [EXISTS] Set icon iff button exists
    if (this->exists() && this->Window.exists())
      this->Window.template send<ButtonMessage::SetImage>(IMAGE_ICON, opaque_cast(this->Value.get())); 

    // Accept button creation
    return 0;
  }
    
  /////////////////////////////////////////////////////////////////////////////////////////
  // ButtonIconPropertyImpl::set 
  //! Set the icon iff button exists, otherwise sets the initial icon
  //! 
  //! \param[in] icon - Button icon
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  ButtonIconPropertyImpl<ENC>::set(value_t icon) 
  {
    // [EXISTS] Set icon iff button exists
    if (this->Window.exists())
      this->Window.template send<ButtonMessage::SetImage>(IMAGE_ICON, opaque_cast(icon.get())); 
    
    // Updated ref-counted shared handle
    base::set(icon);

    // TODO: Clear the bitmap, if any
  }
  
    
} // namespace wtl

#endif // WTL_BUTTON_ICON_PROPERTY_HPP
