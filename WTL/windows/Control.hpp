//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Control.hpp
//! \brief Base for all control types
//! \date 19 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_WINDOW_HPP
#define WTL_MESSAGE_WINDOW_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Window.hpp>                 //!< Window
#include <wtl/platform/ControlStyles.hpp>         //!< various enumerations

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Control - Base for all control types
  //! 
  //! \tparam ENC - Window charactrer encoding (UTF16 if unspecified)
  ///////////////////////////////////////////////////////////////////////////////
  template <wtl::Encoding ENC = wtl::Encoding::UTF16>
  struct Control : wtl::Window<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = wtl::Window<ENC>;

    //! \alias class_t - Inherit window class type
    using class_t = typename base::class_t;

    //! \var encoding - Inherit window character encoding
    static constexpr wtl::Encoding  encoding = base::encoding;

    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------- CONSTRUCTION -----------------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Control::Control
    //! Create child control
    //!
    //! \param[in] id - Control identifier
    ///////////////////////////////////////////////////////////////////////////////
    Control(WindowId id) 
    {
      this->Font  = StockFont::Window; 
      this->Ident = id;  
    }
  
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
  };

  

} // namespace wtl


#endif // WTL_MESSAGE_WINDOW_HPP
