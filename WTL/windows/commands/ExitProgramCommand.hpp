//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\ExitProgramCommand.hpp
//! \brief Encapsulates the 'Exit program' command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLOSE_PROGRAM_HPP
#define WTL_CLOSE_PROGRAM_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ExitProgramCommand - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ExitProgramCommand : Action<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = Action<ENC>;

    //! \alias window_t - Define window base type
    using window_t = WindowBase<ENC>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    window_t&  AppWnd;        //!< Application window

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::ExitProgramCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    /////////////////////////////////////////////////////////////////////////////////////////
    ExitProgramCommand(window_t& appWnd) 
      : base(CommandId::App_Exit, [&appWnd] () { appWnd.post<WindowMessage::CLOSE>(); } ),
        AppWnd(appWnd)
    {}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    typename base::type*  clone() const 
    {
      return new ExitProgramCommand(AppWnd);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::state const
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

#endif // WTL_CLOSE_PROGRAM_HPP
