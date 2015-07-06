//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowStyleProperty.cpp
//! \brief Implementation for 'WindowStyle' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                            //!< EnumCast
#include "wtl/windows/properties/WindowStyleProperty.hpp"    //!< WindowStyleProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowStylePropertyImpl::get const
  //! Get the window style
  //! 
  //! \return value_t - Current style if window exists, otherwise 'initial' style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowStylePropertyImpl<ENC>::value_t  WindowStylePropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window Style
    if (this->Window.exists())
      return enum_cast<WindowStyle>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_STYLE) );
        
    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowStylePropertyImpl::set 
  //! Set the current window style iff window exists, otherwise 'initial' style
  //! 
  //! \param[in] style - Window style
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowStylePropertyImpl<ENC>::set(value_t style) 
  {
    // [EXISTS] Set window Style
    if (this->Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_STYLE, enum_cast(style)))
      throw platform_error(HERE, "Unable to set window style");

    // Store value
    base::set(style);
  }

      
} // namespace wtl

