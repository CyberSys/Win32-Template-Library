//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\StyleExPropertyImpl.hpp
//! \brief Implementation for extended window style property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EX_WINDOW_STYLE_PROPERTY_IMPL_HPP
#define WTL_EX_WINDOW_STYLE_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                            //!< EnumCast
#include "wtl/windows/properties/StyleExProperty.hpp"    //!< StyleExProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

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
      return enum_cast<WindowStyleEx>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_EXSTYLE) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // StyleExPropertyImpl::set 
  //! Set the current extended window style iff window exists, otherwise 'initial' style
  //! 
  //! \param[in] style - Extended window style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  StyleExPropertyImpl<ENC>::set(value_t style) 
  {
    // [EXISTS] Set window StyleEx
    if (this->Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_EXSTYLE, enum_cast(style)))
      throw platform_error(HERE, "Unable to set extended window style");

    // Store value
    base::set(style);
  }

      
} // namespace wtl

#endif // WTL_EX_WINDOW_STYLE_PROPERTY_IMPL_HPP

