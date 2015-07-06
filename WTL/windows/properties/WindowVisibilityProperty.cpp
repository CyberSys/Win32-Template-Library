//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowVisibilityProperty.cpp
//! \brief Implementation for 'WindowVisibility' property (avoids circular reference regarding WindowBase template)
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include "wtl/WTL.hpp"
#include "wtl/casts/BooleanCast.hpp"                              //!< BooleanCast
#include "wtl/casts/EnumCast.hpp"                                 //!< EnumCast
#include "wtl/windows/properties/WindowVisibilityProperty.hpp"       //!< WindowVisibilityProperty
#include "wtl/windows/WindowBase.hpp"                             //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowVisibilityPropertyImpl::get const
  //! Get the window visibility
  //! 
  //! \return value_t - Current visibility if window exists, otherwise 'initial' visibility
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowVisibilityPropertyImpl<ENC>::value_t  WindowVisibilityPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Query window visibility
    if (this->Window.exists())
    {
      WindowPlacement info;

      // Query window placement
      if (!::GetWindowPlacement(this->Window, &info))
        throw platform_error(HERE, "Unable to query window visibility");

      // Extract visibility
      return enum_cast<value_t>(info.flags);
    }

    // Return cached
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowVisibilityPropertyImpl::set 
  //! Set the current window visibility iff window exists, otherwise 'initial' visibility
  //! 
  //! \param[in] visibility - Window visibility
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowVisibilityPropertyImpl<ENC>::set(value_t visibility) 
  {
    // Set window visibility
    if (this->Window.exists() && !::ShowWindow(Window, enum_cast(visibility)))
      throw platform_error(HERE, "Unable to set window visibility");

    // Update value
    base::set(visibility);
  }

      
} // namespace wtl

