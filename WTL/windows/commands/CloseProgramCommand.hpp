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

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias resource_t - Define resource id type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::ExitProgramCommand
    //! Derived c-tor
    ///////////////////////////////////////////////////////////////////////////////
    ExitProgramCommand() : base(ID_FILE_EXIT)
    {}
    
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::~ExitProgramCommand
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~ExitProgramCommand() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::canUndo const
    //! Query the whether the command can be undone
    //! 
    //! \return bool - True iff command can be undone
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool canUndo() const
    {
      return false;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const
    {
      return "Exit the program";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon icon() const
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
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t* name() const
    {
      return "Exit";
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::execute 
    //! Executes the command
    //! 
    //! \param[in] &&... args - [optional] Strongly typed variadic arguments
    ///////////////////////////////////////////////////////////////////////////////
    /*template <typename... ARGS>
    virtual void execute(ARGS&&...) */

    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::execute 
    //! Executes the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void execute() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::undo
    //! Undo the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void undo() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ExitProgramCommand::undo
    //! Undo the command
    //! 
    //! \param[in] &&... args - [optional] Strongly typed variadic arguments
    ///////////////////////////////////////////////////////////////////////////////
    /*template <typename... ARGS>
    virtual void undo(ARGS&&...) 
    {
    }*/
    
  };
  

}

#endif // WTL_CLOSE_PROGRAM_HPP
