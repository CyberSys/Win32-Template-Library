//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\VisibilityPropertyImpl.hpp
//! \brief Separate implementation for 'Visibility' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_VISIBLE_PROPERTY_HPP
#define WTL_WINDOW_VISIBLE_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/BooleanCast.hpp>                            //!< BooleanCast
#include <wtl/casts/EnumCast.hpp>                               //!< EnumCast
#include <wtl/windows/properties/VisibilityProperty.h>          //!< VisibilityProperty
#include <wtl/windows/Window.hpp>                           //!< Window

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // VisibilityPropertyImpl::get const
  //! Get the window visibility
  //! 
  //! \return value_t - Current visibility if window exists, otherwise 'initial' visibility
  //! 
  //! \throw wtl::platform_error - Unable to query window visibility
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename VisibilityPropertyImpl<ENC>::value_t  VisibilityPropertyImpl<ENC>::get() const 
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
  // VisibilityPropertyImpl::set 
  //! Set the current window visibility iff window exists, otherwise 'initial' visibility
  //! 
  //! \param[in] visibility - Window visibility
  //! 
  //! \throw wtl::platform_error - Unable to set window visibility
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  VisibilityPropertyImpl<ENC>::set(value_t visibility) 
  {
    // Set window visibility
    if (this->Window.exists() && !::ShowWindow(this->Window, enum_cast(visibility)))
      throw platform_error(HERE, "Unable to set window visibility");

    // Update value
    base::set(visibility);
  }

      
} // namespace wtl

#endif // WTL_WINDOW_VISIBLE_PROPERTY_HPP

