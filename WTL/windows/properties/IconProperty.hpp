//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\IconProperty.hpp
//! \brief Separate implementation for the Button control 'Icon' property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ICON_PROPERTY_HPP
#define WTL_WINDOW_ICON_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/properties/IconProperty.h>     //!< IconPropertyImpl
#include <wtl/windows/controls/Button.hpp>           //!< Button

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // IconPropertyImpl::get const
  //! Get the button icon
  //! 
  //! \return value_t - Current icon if button exists, otherwise initial icon
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename IconPropertyImpl<ENC>::value_t  IconPropertyImpl<ENC>::get() const 
  {
    // Return shared handle
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // IconPropertyImpl::set 
  //! Set the icon iff button exists, otherwise sets the initial icon
  //! 
  //! \param[in] icon - Button icon
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  IconPropertyImpl<ENC>::set(value_t icon) 
  {
    // [EXISTS] Set icon iff button exists
    if (this->Window.exists())
      this->Window.template send<ButtonMessage::SetImage>(IMAGE_ICON, opaque_cast(icon.get())); 
    
    // Updated ref-counted shared handle
    base::set(icon);

    // TODO: Clear the bitmap, if any
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // IconPropertyImpl::set 
  //! Set the previously assigned icon
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  IconPropertyImpl<ENC>::set() 
  {
    // [EXISTS] Set icon iff button exists
    if (this->exists() && this->Window.exists())
      this->Window.template send<ButtonMessage::SetImage>(IMAGE_ICON, opaque_cast(this->Value.get())); 
  }
      
} // namespace wtl

#endif // WTL_WINDOW_ICON_PROPERTY_HPP
