//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowRectPropertyImpl.hpp
//! \brief Implementation for window rect property accessors/mutators (resolves circular dependency)
//! \remarks Poor naming scheme not to be confused with the PIMPL pattern used by Property templates! 
//! \date 5 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_RECT_PROPERTY_IMPL_HPP
#define WTL_WINDOW_RECT_PROPERTY_IMPL_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/properties/WindowRectProperty.hpp"     //!< WindowRectProperty
#include "wtl/windows/WindowBase.hpp"                        //!< WindowBase

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  
  // ---------------------------------- ACCESSOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowRectPropertyImpl::get const
  //! Get the window rectangle
  //! 
  //! \return value_t - Current rectangle if window exists, otherwise 'initial' rectangle
  //!
  //! \throw wtl::logic_error - Window is using default size or location
  //! \throw wtl::platform_error - Unable to query window rectangle
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  typename WindowRectPropertyImpl<ENC>::value_t  WindowRectPropertyImpl<ENC>::get() const 
  {
    // [EXISTS] Return current window rectangle
    if (this->Window.exists())
    {
      value_t wnd;    //!< Window rectangle
        
      // Query & return window rectangle
      if (!::GetWindowRect(this->Window, &native_cast(wnd)))
        throw platform_error(HERE, "Unable to query window rectangle");
      return wnd;
    }

    // [DEFAULT] Error: Cannot generate a window rectangle from default co-ordinates
    if (this->Window.Size == window_t::DefaultSize || this->Window.Position == window_t::DefaultPosition)
      throw logic_error(HERE, "Cannot generate a window rectangle from default co-ordinates");

    // [~EXISTS] Generate from cached size & position
    return { this->Window.Position(), this->Window.Size() };
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowRectPropertyImpl::set 
  //! Set the current window rectangle iff window exists, otherwise 'initial' rectangle
  //! 
  //! \param[in] rc - Window rectangle
  //! 
  //! \throw wtl::platform_error - Unable to set window position
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowRectPropertyImpl<ENC>::set(value_t rc) 
  {
    bool resized = this->Value.width() == rc.width() && this->Value.height() == rc.height(),    //!< Whether resized
           moved = this->Value.left == rc.left && this->Value.top == rc.top;                    //!< Whethe rmoved

    // [EXISTS] Resize window
    if (this->Window.exists())
    {
      MoveWindowFlags flags = MoveWindowFlags::NoZOrder;
          
      // [¬RESIZED] Add appropriate flag
      if (!resized)
        flags |= MoveWindowFlags::NoSize;

      // [¬MOVED] Add appropriate flag
      if (!moved)
        flags |= MoveWindowFlags::NoMove;

      // Resize/reposition window
      if (!::SetWindowPos(this->Window, default<::HWND>(), rc.left, rc.top, rc.width(), rc.height(), enum_cast(flags)))
        throw platform_error(HERE, "Unable to set window position");
    }

    // [¬EXISTS] Set size/position
    if (!this->Window.exists())
    {
      this->Window.Size = rc.size();
      this->Window.Position = rc.topLeft();
    }
  }

      
} // namespace wtl

#endif // WTL_WINDOW_RECT_PROPERTY_IMPL_HPP

