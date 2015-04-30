////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\OpenDocumentCommand.hpp
//! \brief Encapsulates the 'Open Document' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OPEN_DOCUMENT_HPP
#define WTL_OPEN_DOCUMENT_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct OpenDocumentCommand - Encapsulates displaying the 'Open Document' dialog
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct OpenDocumentCommand : GuiCommand<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \alias base - Define base type
    using base = GuiCommand<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // -------------------- REPRESENTATION ---------------------
  
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // OpenDocumentCommand::OpenDocumentCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    ///////////////////////////////////////////////////////////////////////////////
    OpenDocumentCommand(window_t& appWnd)  
      : base(CommandId::File_Open, [&appWnd] () { errorBox(appWnd, c_arr("Error"), c_arr("Command not implemented")); })
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // OpenDocumentCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return interface_t* - New instance of command
    ///////////////////////////////////////////////////////////////////////////////
    typename base::interface_t*  clone() const 
    {
      return new OpenDocumentCommand(*this);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // OpenDocumentCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    ///////////////////////////////////////////////////////////////////////////////
    CommandState state() const override
    {
      // Always enabled
      return CommandState::Enabled;
    }
    
    // ----------------------- MUTATORS ------------------------
    
  };
  

}

#endif // WTL_OPEN_DOCUMENT_HPP
