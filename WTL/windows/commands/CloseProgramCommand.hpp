////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\commands\ExitProgramCommand.hpp
//! \brief Encapsulates the 'Exit program' command
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CLOSE_PROGRAM_HPP
#define WTL_CLOSE_PROGRAM_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ExitProgramCommand - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ExitProgramCommand : GuiCommand<ENC,CommandId::FILE_EXIT>
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \typedef base - Define base type
    using base = GuiCommand<ENC,CommandId::FILE_EXIT>;

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::ExitProgramCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    ///////////////////////////////////////////////////////////////////////////////
    ExitProgramCommand(WindowBase<ENC>& appWnd) : AppWindow(appWnd)
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    ///////////////////////////////////////////////////////////////////////////////
    bool  permanent() const override
    {
      return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandState state() const
    {
      return CommandState::Enabled;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::execute 
    //! Executes the command
    //!
    //! \param[in] src - Source of command
    ///////////////////////////////////////////////////////////////////////////////
    void execute(CommandSource src) override
    {
      AppWindow.post(WindowMessage::CLOSE);
    }

    // ----------------------- REPRESENTATION ------------------------
  protected:
    WindowBase<ENC>&  AppWindow;     //!< Main program window
  };
  
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ExitProgramCommandHandler - Handler for gui command 'FILE_EXIT'
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ExitProgramCommandHandler = GuiCommandHandler<ENC,ExitProgramCommand<ENC>>;

  
}

#endif // WTL_CLOSE_PROGRAM_HPP
