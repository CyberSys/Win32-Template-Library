//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\SizePropertyImpl.hpp
//! \brief Implementation for window size property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_SIZE_PROPERTY_IMPL_HPP
#define WTL_WINDOW_SIZE_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/properties/SizeProperty.hpp"     //!< SizeProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // SizePropertyImpl::get const
  //! Get the window size
  //! 
  //! \return value_t - Current size if window exists, otherwise 'initial' size
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename SizePropertyImpl<ENC>::value_t  SizePropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Derive window size from window rectangle 
    if (this->Window.exists())
      return this->Window.WindowRect().size();

    // [~EXISTS] Return cached size  (Offline window rectangle derived from size)
    return base::get();
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // SizePropertyImpl::set 
  //! Set the current window size iff window exists, otherwise 'initial' size
  //! 
  //! \param[in] size - Window size
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  SizePropertyImpl<ENC>::set(value_t size) 
  {
    // [EXISTS] Resize current window rectangle 
    if (this->Window.exists())
      this->Window.WindowRect = RectL(this->Window.Position(), size);
        
    // Store size
    base::set(size);
  }

      
} // namespace wtl

#endif // WTL_WINDOW_SIZE_PROPERTY_IMPL_HPP

