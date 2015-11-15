//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\properties\WindowRectPropertyImpl.hpp
//! \brief Separate implementation for 'WindowRect' window property (resolves circular dependency)
//! \date 29 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_RECT_PROPERTY_HPP
#define WTL_WINDOW_RECT_PROPERTY_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/properties/WindowRectProperty.h>      //!< WindowRectProperty
#include <wtl/windows/WindowBase.hpp>                       //!< WindowBase

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
      if (!::GetWindowRect(this->Window, wnd))
        throw platform_error(HERE, "Unable to query window rectangle");
      return wnd;
    }
    // [~EXISTS] Calculate from size & position
    else
    {
      // [DEFAULT] Sentinel values are invalid by definition
      if (this->Window.Size == window_t::DefaultSize || this->Window.Position == window_t::DefaultPosition)
        throw logic_error(HERE, "Cannot generate a window rectangle from default co-ordinates");

      // [~DEFAULT] Generate rectangle from offline size & position
      return { this->Window.Position(), this->Window.Size() };
    }
  }

  // ----------------------------------- MUTATOR METHODS ----------------------------------

  /////////////////////////////////////////////////////////////////////////////////////////
  // WindowRectPropertyImpl::set 
  //! Set the current window rectangle iff window exists, otherwise 'initial' rectangle
  //! 
  //! \param[in] rc - Window rectangle
  //! 
  //! \throw wtl::platform_error - Unable to set window rectangle
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  void  WindowRectPropertyImpl<ENC>::set(value_t rc) 
  {
    bool resized = this->Value.width() == rc.width() && this->Value.height() == rc.height(),    //!< Whether resized
           moved = this->Value.Left == rc.Left && this->Value.Top == rc.Top;                    //!< Whether moved

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
      if (!::SetWindowPos(this->Window, defvalue<::HWND>(), rc.Left, rc.Top, rc.width(), rc.height(), enum_cast(flags)))
        throw platform_error(HERE, "Unable to set window position");
    }
    // [¬EXISTS] Set size/position
    else
    {
      this->Window.Size = rc.size();
      this->Window.Position = rc.topLeft();
    }
  }

      
} // namespace wtl

#endif // WTL_WINDOW_RECT_PROPERTY_HPP

