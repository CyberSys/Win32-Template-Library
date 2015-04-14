////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\events\GuiCommand.hpp
//! \brief Encapsulates commands in the Gui using the 'Command' pattern
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_GUI_COMMAND_HPP
#define WTL_GUI_COMMAND_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum CommandState - Define states of GUI Commands
  enum class CommandState
  {
    Disabled = 0,     //!< Command should be disabled
    Enabled = 0,      //!< Command should be enabled
    Hidden = 0,       //!< Command should be hidden
  };


  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommand - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  //! \tparam CMD - Command Id
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, CommandId CMD>
  struct GuiCommand
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::GuiCommand
    //! Derived c-tor
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommand(CommandId id) : Ident(id),
                               Name(wtl::StringResource(id)),
                               Description(id)
    {}
    
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::~GuiCommand
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~GuiCommand() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::canUndo const
    //! Query the whether the command can be undone
    //! 
    //! \return bool - True iff command can be undone
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool  canUndo() const
    {
      return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const
    {
      return nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const
    {
      return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  name() const
    {
      return nullptr;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandState  state() const
    {
      return CommandState::Enabled;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::execute 
    //! Executes the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void execute() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::undo
    //! Undo the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void undo() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }

    // ----------------------- REPRESENTATION ------------------------
  };
  

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct CommandGroup - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CommandGroup
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // CommandGroup::~CommandGroup
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~CommandGroup() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // CommandGroup::canUndo const
    //! Query the whether the command can be undone
    //! 
    //! \return bool - True iff command can be undone
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool canUndo() const
    {
      return true;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // CommandGroup::operator +=
    //! Add a command to the group
    //! 
    //! \param[in] const& cmd - Command 
    //! \return CommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    CommandGroup& operator += (const GuiCommand<ENC>& cmd)
    {
      return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // CommandGroup::operator -=
    //! Remove a command from the group
    //! 
    //! \param[in] const& cmd - Command 
    //! \return CommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    CommandGroup& operator -= (const GuiCommand<ENC>& cmd)
    {
      return *this;
    }
    
    // ----------------------- REPRESENTATION ------------------------
  };



  

}

#endif // WTL_GUI_COMMAND_HPP
