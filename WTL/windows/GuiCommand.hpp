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
  //! \enum CommandState - Define states of GUI Commands
  enum class CommandState
  {
    Disabled = 0,     //!< Command should be disabled
    Enabled = 0,      //!< Command should be enabled
    Hidden = 0,       //!< Command should be hidden
  };

  //! \enum CommandState - Defines GUI Command Ids
  enum class CommandId : int32
  {
    // TODO: Import ID_FILE_OPEN, ... etc. from Win32 definitions if possible, otherwise MFC or WTL
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! wtl::command_id
  //! Creates a strongly typed command id from any integral or enumeration type
  //!
  //! \tparam TYPE - Integral or enumeration type
  //! 
  //! \param[in] id - Value representing command id
  //! \return CommandId - CommandId representation of 'id'
  ///////////////////////////////////////////////////////////////////////////////
  template <typename VALUE, typename = std::enable_if_t<std::is_integral<VALUE>::value || std::is_enum<VALUE>::value>>
  CommandId  command_id(VALUE id)
  {
    // Convert into underlying type then cast to enumeration
    return enum_cast<CommandId>( static_cast<std::underlying_type_t<CommandId>>(id) );
  }
  

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
    
    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------
  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::GuiCommand
    //! Create from a single id which is used for name/description string id and icon id
    //! 
    //! \param[in] id - Command identifier
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommand(CommandId id) : Ident(id),
                               Name(resource_id(id)),
                               Description(resource_id(id)),
                               Icon(resource_id(id)),
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
  };
  


  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommandGroup - Base for all Gui commands
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct GuiCommandGroup
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding encoding = ENC;
    
    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::~GuiCommandGroup
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~GuiCommandGroup() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::canUndo const
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
    // GuiCommandGroup::operator +=
    //! Add a command to the group
    //! 
    //! \param[in] const& cmd - Command 
    //! \return GuiCommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommandGroup& operator += (const GuiCommand<ENC>& cmd)
    {
      return *this;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommandGroup::operator -=
    //! Remove a command from the group
    //! 
    //! \param[in] const& cmd - Command 
    //! \return GuiCommandGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    GuiCommandGroup& operator -= (const GuiCommand<ENC>& cmd)
    {
      return *this;
    }
    
    // ----------------------- REPRESENTATION ------------------------
  };



  

}

#endif // WTL_GUI_COMMAND_HPP
