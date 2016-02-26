//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Control.hpp
//! \brief Base for all control types
//! \date 19 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONTROL_HPP
#define WTL_CONTROL_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/Window.hpp>                 //!< Window
#include <wtl/platform/ControlStyles.hpp>         //!< ()

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
  
    //! \alias type - Define own type
    using type = wtl::Control<ENC>;

    //! \alias base - Define base type
    using base = wtl::Window<ENC>;

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
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Control::onCreate
    //! Called during window creation to modify window parameters and create child windows
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Routing indicating message was not handled
    /////////////////////////////////////////////////////////////////////////////////////////
    LResult  onCreate(CreateWindowEventArgs<encoding>& args) override
    { 
      // [Unhandled] Default action is to route message to subclass
      return {MsgRoute::Unhandled, 0};
    }
    
  };

  

} // namespace wtl


#endif // WTL_CONTROL_HPP
