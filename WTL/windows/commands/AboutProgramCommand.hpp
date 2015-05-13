//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\AboutProgramCommand.hpp
//! \brief Encapsulates the 'About dialog' Gui Command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ABOUT_PROGRAM_HPP
#define WTL_ABOUT_PROGRAM_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Action.hpp"             //!< Action
#include "wtl/windows/WindowBase.hpp"         //!< WindowBase
#include "wtl/windows/MessageBox.hpp"         //!< errorBox

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct AboutProgramCommand - Encapsulates displaying the 'About' dialog
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct AboutProgramCommand : Action<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = AboutProgramCommand<ENC>;
  
    //! \alias base - Define base type
    using base = Action<ENC>;

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // AboutProgramCommand::AboutProgramCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    /////////////////////////////////////////////////////////////////////////////////////////
    AboutProgramCommand(window_t& appWnd)  
      : base(CommandId::App_About, [&appWnd] () { errorBox(appWnd, c_arr("Error"), c_arr("Command not implemented")); })
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(AboutProgramCommand);      //!< Can be shallow copied
    ENABLE_MOVE(AboutProgramCommand);      //!< Can be moved 
    ENABLE_POLY(AboutProgramCommand);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // AboutProgramCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::type*  clone() const 
    {
      return new AboutProgramCommand(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // AboutProgramCommand::state const
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

#endif // WTL_ABOUT_PROGRAM_HPP
