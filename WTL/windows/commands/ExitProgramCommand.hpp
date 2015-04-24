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
  struct ExitProgramCommand : GuiCommand<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \typedef base - Define base type
    using base = GuiCommand<ENC>;

    // -------------------- REPRESENTATION ---------------------
  protected:
    WindowBase<ENC>&  AppWnd;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::ExitProgramCommand
    //! Create command
    //! 
    //! \param[in] appWnd - Main application window
    ///////////////////////////////////////////////////////////////////////////////
    ExitProgramCommand(WindowBase<ENC>& appWnd) 
      : base(CommandId::FILE_EXIT, [&appWnd] () { appWnd.post(WindowMessage::CLOSE); } ),
        AppWnd(appWnd)
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return interface_t* - New instance of command
    ///////////////////////////////////////////////////////////////////////////////
    typename base::interface_t*  clone() const 
    {
      return new ExitProgramCommand(AppWnd);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::state const
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

#endif // WTL_CLOSE_PROGRAM_HPP
