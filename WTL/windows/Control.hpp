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
//#include <wtl/platform/ControlStyles.hpp>         //!< (various)

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
      //this->Font  = StockFont::Window; 
      this->Ident = id;

      this->Created += new WindowCreatedEventHandler<encoding>(this, &Control::postCreate);
    }
  
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // Control::onDestroy
    //! Called during window destruction. Default implementation releases the window font.
    //! 
    //! \return wtl::LResult - Does not consume message
    ///////////////////////////////////////////////////////////////////////////////
    LResult  onDestroy() override
    { 
      // Release custom font, if any
      this->Font = StockFont::System;

      // [Unhandled] Pass to base
      return base::onDestroy();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Control::postCreate
    //! Called after window creation. Default implementation sets the window font.
    //! 
    //! \return wtl::LResult - Does not consume message
    ///////////////////////////////////////////////////////////////////////////////
    virtual LResult  postCreate()
    {
      //! Set default window font
      this->Font  = StockFont::Window; 

      //! Consume
      return {MsgRoute::Unhandled};
    }
  };

  

} // namespace wtl


#endif // WTL_CONTROL_HPP
