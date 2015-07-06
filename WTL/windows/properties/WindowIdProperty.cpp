//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowIdProperty.cpp
//! \brief Implementation for 'WindowId' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                            //!< EnumCast
#include "wtl/windows/properties/WindowIdProperty.hpp"       //!< WindowIdProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowIdPropertyImpl::get const
  //! Get the window id
  //! 
  //! \return value_t - Current window-id if window exists, otherwise 'initial' window-id
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowIdPropertyImpl<ENC>::value_t  WindowIdPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window Id
    if (this->Window.exists())
      return static_cast<WindowId>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_ID) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowIdPropertyImpl::set 
  //! Set the current window id iff window exists, otherwise 'initial' window-id
  //! 
  //! \param[in] id - Window id
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowIdPropertyImpl<ENC>::set(value_t id) 
  {
    // [EXISTS] Set window Id
    if (this->Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_ID, enum_cast(id)))
      throw platform_error(HERE, "Unable to set window Id");

    // Store value
    base::set(id);
  }

      
} // namespace wtl

