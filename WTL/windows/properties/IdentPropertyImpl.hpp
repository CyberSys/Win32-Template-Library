//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\IdentPropertyImpl.hpp
//! \brief Implementation for window id property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_ID_PROPERTY_IMPL_HPP
#define WTL_WINDOW_ID_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                            //!< EnumCast
#include "wtl/windows/properties/IdentProperty.hpp"       //!< IdentProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

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
      return static_cast<WindowId>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_ID) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // IdentPropertyImpl::set 
  //! Set the current window id iff window exists, otherwise 'initial' window-id
  //! 
  //! \param[in] id - Window id
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  IdentPropertyImpl<ENC>::set(value_t id) 
  {
    // [EXISTS] Set window Id
    if (this->Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_ID, enum_cast(id)))
      throw platform_error(HERE, "Unable to set window Id");

    // Store value
    base::set(id);
  }

      
} // namespace wtl

#endif // WTL_WINDOW_ID_PROPERTY_IMPL_HPP

