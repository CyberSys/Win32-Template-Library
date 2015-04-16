////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\GuiCommand.hpp
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
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommand - Encapsulates a single application command. Base class for all commands.
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
    
    //! \alias command_t - Define command type
    using command_t = GuiCommand<ENC,CMD>;

    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
    //! \var ident - Define command id
    static constexpr CommandId  command = CMD;

    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::GuiCommand
    //! Create from a single id which is used for name/description string id and icon id
    //! 
    //! \param[in] id - Command identifier
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommand() : Ident(command),
                   Name(resource_id(command)),
                   Description(resource_id(command)),
                   Icon(resource_id(command))
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::GuiCommand
    //! Create command from individual properties
    //! 
    //! \param[in] id - Command identifier
    //! \param[in] name - Name/description string resource id
    //! \param[in] icon - Icon resource id
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommand(CommandId id, resource_t name, resource_t icon) : Ident(id),
                                                                 Name(name),
                                                                 Description(name),
                                                                 Icon(icon)
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
      return SystemIcon::Application;
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
    // GuiCommand::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool  permanent() const
    {
      return true;
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
    //! \param[in] src - Source of command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void execute(CommandSource src) 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::revert
    //! Reverts the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void revert() 
    {
      throw logic_error(HERE, "Command has not been implemented");
    }

    // ----------------------- REPRESENTATION ------------------------
  protected:
    CommandId    Ident;           //!< Command Id
    resource_t   Name,            //!< Command Name
                 Description,     //!< Command Description
                 Icon;            //!< Command Icon
  };
  

  

}

#endif // WTL_GUI_COMMAND_HPP
