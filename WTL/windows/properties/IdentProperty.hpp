//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\IdentProperty.hpp
//! \brief Separate implementation for 'Ident' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ID_PROPERTY_HPP
#define WTL_WINDOW_ID_PROPERTY_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                       //!< EnumCast
#include "wtl/windows/properties/IdentProperty.h"       //!< IdentPropertyImpl
#include "wtl/windows/WindowBase.hpp"                   //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // IdentPropertyImpl::get const
  //! Get the window id
  //! 
  //! \return value_t - Current window-id if window exists, otherwise 'initial' window-id
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename IdentPropertyImpl<ENC>::value_t  IdentPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window Id
    if (this->Window.exists())
      return static_cast<WindowId>( getFunc<base::encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_ID) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // IdentPropertyImpl::set 
  //! Set the current window id iff window exists, otherwise 'initial' window-id
  //! 
  //! \param[in] id - Window id
  //! 
  //! \throw wtl::platform_error - Unable to set window-id
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  IdentPropertyImpl<ENC>::set(value_t id) 
  {
    // [EXISTS] Set window Id
    if (this->Window.exists() && !getFunc<base::encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_ID, enum_cast(id)))
      throw platform_error(HERE, "Unable to set window Id");

    // Store value
    base::set(id);
  }

      
} // namespace wtl

#endif // WTL_WINDOW_ID_PROPERTY_HPP

