//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\StyleExProperty.hpp
//! \brief Separate implementation for 'StyleEx' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EX_WINDOW_STYLE_PROPERTY_HPP
#define WTL_EX_WINDOW_STYLE_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>                       //!< EnumCast
#include <wtl/windows/properties/StyleExProperty.h>     //!< StyleExPropertyImpl
#include <wtl/windows/Window.hpp>                   //!< Window

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // StyleExPropertyImpl::get const
  //! Get the extended window style
  //! 
  //! \return value_t - Current style if window exists, otherwise 'initial' style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename StyleExPropertyImpl<ENC>::value_t  StyleExPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window StyleEx
    if (this->Window.exists())
      return enum_cast<WindowStyleEx>( choose<base::encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_EXSTYLE) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // StyleExPropertyImpl::set 
  //! Set the current extended window style iff window exists, otherwise 'initial' style
  //! 
  //! \param[in] style - Extended window style
  //! 
  //! \throw wtl::platform_error - Unable to set window style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  StyleExPropertyImpl<ENC>::set(value_t style) 
  {
    // [EXISTS] Set window StyleEx
    if (this->Window.exists() && !choose<base::encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_EXSTYLE, enum_cast(style)))
      throw platform_error(HERE, "Unable to set extended window style");

    // Store value
    base::set(style);
  }

      
} // namespace wtl

#endif // WTL_EX_WINDOW_STYLE_PROPERTY_HPP

