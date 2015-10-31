//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\OpenDocumentCommand.hpp
//! \brief Encapsulates the 'Open Document' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_OPEN_DOCUMENT_HPP
#define WTL_OPEN_DOCUMENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Command.hpp"             //!< Command
#include "wtl/windows/WindowBase.hpp"         //!< WindowBase
#include "wtl/windows/MessageBox.hpp"         //!< errorBox

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct OpenDocumentCommand - Encapsulates displaying the 'Open Document' dialog
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct OpenDocumentCommand : Command<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = OpenDocumentCommand<ENC>;
  
    //! \alias base - Define base type
    using base = Command<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------- CONSTRUCTION -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // OpenDocumentCommand::OpenDocumentCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    /////////////////////////////////////////////////////////////////////////////////////////
    OpenDocumentCommand(window_t& appWnd)  
      : base(CommandId::File_Open, [&appWnd] () { errorBox<encoding>(appWnd, c_str("Program Error"), c_str("Command not implemented")); })
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(OpenDocumentCommand);      //!< Can be shallow copied
    ENABLE_MOVE(OpenDocumentCommand);      //!< Can be moved 
    ENABLE_POLY(OpenDocumentCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // OpenDocumentCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    type*  clone() const 
    {
      return new OpenDocumentCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // OpenDocumentCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandState state() const override
    {
      // Always enabled
      return CommandState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };
  

}

#endif // WTL_OPEN_DOCUMENT_HPP
