////////////////////////////////////////////////////////////////////////////////
//! \file wtl\controls\Button.hpp
//! \brief Encapsulates button controls
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BUTTON_HPP
#define WTL_BUTTON_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Button - Encapsulates a button control
  //! 
  //! \tparam ENC - Character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Button : WindowBase<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = WindowBase<ENC>;

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates a standard button control
    //! 
    //! \param[in] instance - Owning instance
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    ///////////////////////////////////////////////////////////////////////////////
    Button(HINSTANCE instance) : base(getClass(instance))
    {
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::Button
    //! Creates a custom button control
    //! 
    //! \param[in] &custom - Custom window class
    //! 
    //! \throw wtl::platform_error - Unrecognised system window class
    ///////////////////////////////////////////////////////////////////////////////
    Button(wndclass_t& custom) : base(custom)
    {
    }

    // ------------------------ STATIC -------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button::getClass 
    //! Get the window class
    //! 
    //! \param[in] instance - Module handle
    //! \return wndclass_t& - Window class 
    ///////////////////////////////////////////////////////////////////////////////
    static wndclass_t& getClass(HINSTANCE instance) 
    {
      static SystemWindowClass<encoding,SystemClass::Button>  wc;    // wc( resource_name(system_class_t<SystemClass::Button>::name) );      //!< Standard system window class
      
      // Return singleton
      return wc;
    }

    // ---------------------- ACCESSORS ------------------------			
    
    // ----------------------- MUTATORS ------------------------
    
    // -------------------- REPRESENTATION ---------------------
    
  };

}

#endif // WTL_BUTTON_HPP
