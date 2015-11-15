//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\StyleProperty.hpp
//! \brief Separate implementation for 'Style' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_STYLE_PROPERTY_HPP
#define WTL_WINDOW_STYLE_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                       //!< EnumCast
#include <wtl/windows/properties/StyleProperty.h>       //!< StyleProperty
#include <wtl/windows/WindowBase.hpp>                   //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // StylePropertyImpl::get const
  //! Get the window style
  //! 
  //! \return value_t - Current style if window exists, otherwise 'initial' style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename StylePropertyImpl<ENC>::value_t  StylePropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window Style
    if (this->Window.exists())
      return enum_cast<WindowStyle>( choose<base::encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_STYLE) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // StylePropertyImpl::set 
  //! Set the current window style iff window exists, otherwise 'initial' style
  //! 
  //! \param[in] style - Window style
  //! 
  //! \throw wtl::platform_error - Unable to set window style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  StylePropertyImpl<ENC>::set(value_t style) 
  {
    // [EXISTS] Set window Style
    if (this->Window.exists() && !choose<base::encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_STYLE, enum_cast(style)))
      throw platform_error(HERE, "Unable to set window style");

    // Store value
    base::set(style);
  }

      
} // namespace wtl

#endif // WTL_WINDOW_STYLE_PROPERTY_HPP

