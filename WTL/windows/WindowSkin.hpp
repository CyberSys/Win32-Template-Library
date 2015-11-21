//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowSkin.hpp
//! \brief 
//! \date 8 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_SKIN_HPP
#define WTL_WINDOW_SKIN_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/events/OwnerDrawCtrlEvent.hpp>              //!< OwnerDrawCtrlEvent
#include <wtl/windows/events/OwnerMeasureCtrlEvent.hpp>           //!< OwnerMeasureCtrlEventArgs

//! \namespace wtl - Windows template library
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC> struct Window;
  template <Encoding ENC> struct Button;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IWindowSkin - Interface for all window rendering visitors
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IWindowSkin
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = IWindowSkin<ENC>;
	
    //! \var encoding - Define character encoding
    static constexpr Encoding  encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    ENABLE_POLY(IWindowSkin);			//!< Abstract base class

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    //! Buttons
    virtual void draw(Button<encoding>& btn, OwnerDrawCtrlEventArgs<encoding>& args) const = 0;
    virtual void measure(Button<encoding>& btn, OwnerMeasureCtrlEventArgs<encoding>& args) const = 0;

    //! Fallback
    virtual void draw(Window<encoding>& wnd, OwnerDrawCtrlEventArgs<encoding>& args) const = 0;
    virtual void measure(Window<encoding>& wnd, OwnerMeasureCtrlEventArgs<encoding>& args) const = 0;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

} // namespace wtl


#endif // WTL_WINDOW_SKIN_HPP
