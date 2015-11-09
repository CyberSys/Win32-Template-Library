//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowSkin.hpp
//! \brief 
//! \date 8 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_SKIN_HPP
#define WTL_WINDOW_SKIN_HPP

#include "wtl/WTL.hpp"


//! \namespace wtl - Windows template library
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct WindowBase;

  //! Forward declaration
  template <Encoding ENC>
  struct Button;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IWindowSkin - 
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IWindowSkin
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = IWindowSkin<ENC>;
	
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    ENABLE_POLY(IWindowSkin);			//!< Abstract base class

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    virtual void draw(Button<ENC>& btn) const = 0;
    virtual void measure(Button<ENC>& btn) const = 0;

    //! Fallback
    virtual void draw(WindowBase<ENC>& wnd) const = 0;
    virtual void measure(WindowBase<ENC>& wnd) const = 0;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

} // namespace wtl


#endif // WTL_WINDOW_SKIN_HPP
