//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\SaveDocumentCommand.hpp
//! \brief Encapsulates the 'Save Document' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SAVE_DOCUMENT_HPP
#define WTL_SAVE_DOCUMENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Action.hpp"             //!< Action
#include "wtl/windows/WindowBase.hpp"         //!< WindowBase
#include "wtl/windows/MessageBox.hpp"         //!< errorBox

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct SaveDocumentCommand - Encapsulates saving the active document
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct SaveDocumentCommand : Action<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = SaveDocumentCommand<ENC>;
  
    //! \alias base - Define base type
    using base = Action<ENC>;

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
    // SaveDocumentCommand::SaveDocumentCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    /////////////////////////////////////////////////////////////////////////////////////////
    SaveDocumentCommand(window_t& appWnd)  
      : base(ActionId::File_Save, [&appWnd] () { errorBox(appWnd, c_arr("Error"), c_arr("Command not implemented")); })
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(SaveDocumentCommand);      //!< Can be shallow copied
    ENABLE_MOVE(SaveDocumentCommand);      //!< Can be moved 
    ENABLE_POLY(SaveDocumentCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // SaveDocumentCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::type*  clone() const 
    {
      return new SaveDocumentCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // SaveDocumentCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return ActionState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionState state() const override
    {
      // Always enabled
      return ActionState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };
  

}

#endif // WTL_SAVE_DOCUMENT_HPP
