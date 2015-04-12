////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Window.hpp
//! \brief Main window class
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_HPP
#define WTL_WINDOW_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \struct Window - Window class
  template <Encoding ENC>
  struct Window 
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = WindowBase<ENC>;

    //! \alias wndclass_t - Define window class type
    using wndclass_t = typename base::wndclass_t;
    
    //! \alias encoding - Inherit window character encoding 
    static constexpr Encoding encoding = base::encoding;

    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // Window::Window
    //! Creates the window object (but not window handle) for an instance of a registered window class
    //! 
    //! \param[in] &cls - Registered window class 
    ///////////////////////////////////////////////////////////////////////////////
    Window(wndclass_t& cls) : base(cls)
    {
    }

    // ------------------------ STATIC -------------------------
  
    // ---------------------- ACCESSORS ------------------------			
  
    // ----------------------- MUTATORS ------------------------
  
    // -------------------- REPRESENTATION ---------------------
  protected:
  };

  

}

#endif // WTL_WINDOW_HPP
