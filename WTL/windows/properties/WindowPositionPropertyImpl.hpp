//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowPositionPropertyImpl.hpp
//! \brief Implementation for window position property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_POSITION_PROPERTY_IMPL_HPP
#define WTL_WINDOW_POSITION_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/properties/WindowPositionProperty.hpp"     //!< WindowPositionProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowPositionPropertyImpl::get const
  //! Get the window position
  //! 
  //! \return value_t - Current position if window exists, otherwise 'initial' position
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowPositionPropertyImpl<ENC>::value_t  WindowPositionPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Derive window position from window rectangle 
    if (this->Window.exists())
      return this->Window.WindowRect().topLeft();

    // [~EXISTS] Return cached position  (Offline window rectangle derived from position)
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowPositionPropertyImpl::set 
  //! Set the current window position iff window exists, otherwise 'initial' position
  //! 
  //! \param[in] position - Window position
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowPositionPropertyImpl<ENC>::set(value_t position) 
  {
    // [EXISTS] Resize current window rectangle   
    if (this->Window.exists())
      this->Window.WindowRect = RectL(position, this->Window.Size());
        
    // Store position
    base::set(position);
  }

      
} // namespace wtl

#endif // WTL_WINDOW_POSITION_PROPERTY_IMPL_HPP

