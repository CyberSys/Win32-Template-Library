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
  //! \interface IGuiCommand - Interface for all gui commands
  //! 
  //! \tparam ENC - Window character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IGuiCommand
  {
    // ------------------- TYPES & CONSTANTS -------------------

    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias interface_t - Define own type
    using interface_t = IGuiCommand<ENC>;

    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
    // -------------------- REPRESENTATION ---------------------

    // --------------------- CONSTRUCTION ----------------------

    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::~IGuiCommand
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~IGuiCommand() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::clone const
    //! Create a new instance of the command
    //! 
    //! \return interface_t* - New instance of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual interface_t*  clone() const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::ident const
    //! Get the command identifier
    //! 
    //! \return CommandId - Command identifier
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandId  ident() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  name() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool  permanent() const = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandState  state() const = 0;
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::execute 
    //! Executes the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void execute() = 0;
    
    ///////////////////////////////////////////////////////////////////////////////
    // IGuiCommand::revert
    //! Reverts the command
    //! 
    //! \throw logic_error - Command has not been implemented
    ///////////////////////////////////////////////////////////////////////////////
    virtual void revert() = 0;
  };
  

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct GuiCommand - Encapsulates a single gui command
  //! 
  //! \tparam ENC - Message character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct GuiCommand : IGuiCommand<ENC>
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \alias base - Define base type
    using base = IGuiCommand<ENC>;
    
    //! \alias char_t - Inherit character type
    using char_t = typename base::char_t;
    
    //! \alias interface_t - Inherit interface type
    using interface_t = typename base::interface_t;
    
    //! \alias resource_t - Inherit resource ident type
    using resource_t = typename base::resource_t;

    //! \alias name_t - Define name string resource type
    using name_t = StringResource<ENC,128>;

    //! \alias description_t - Define description string resource type
    using description_t = StringResource<ENC,1024>;

    //! \alias icon_t - Define icon resource type
    using icon_t = IconResource;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = base::encoding;
    
  protected:
    //! \alias execute_t - Define execute functor type
    using execute_t = std::function<void ()>;

    //! \alias revert_t - Define undo functor type
    using revert_t = std::function<void ()>;

    // ----------------------- REPRESENTATION ------------------------
  protected:
    CommandId      Ident;           //!< Command Id
    name_t         Name;            //!< Command Name
    description_t  Description;     //!< Command Description
    icon_t         Icon;            //!< Command Icon
    bool           Permanent;       //!< Whether command is permanent
    execute_t      Execute;         //!< Command execution functor
    revert_t       Revert;          //!< Command reversion functor

    // --------------------- CONSTRUCTION ----------------------
  
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::GuiCommand
    //! Create a permenant command
    //! 
    //! \param[in] id - Command identifier
    //! \param[in] exec - Callable target which implements executing command
    ///////////////////////////////////////////////////////////////////////////////
    template <typename T>
    GuiCommand(CommandId id, T exec) : Ident(id),
                                       Name(resource_id(id)),
                                       Description(resource_id(id)),
                                       Icon(resource_id(id)),
                                       Permanent(true),
                                       Execute(exec)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::GuiCommand
    //! Create a revertible command
    //! 
    //! \param[in] id - Command identifier
    //! \param[in] exec - Callable target which implements executing command
    //! \param[in] undo - Callable target which implements reverting command
    ///////////////////////////////////////////////////////////////////////////////
    template <typename T1, typename T2>
    GuiCommand(CommandId id, T1 exec, T2 undo) : Ident(id),
                                                 Name(resource_id(id)),
                                                 Description(resource_id(id)),
                                                 Icon(resource_id(id)),
                                                 Permanent(false),
                                                 Execute(exec),
                                                 Revert(undo)
    {}
    
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
    const char_t*  description() const override
    {
      return Description.Text;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    HIcon  icon() const override
    {
      return Icon.Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::ident const
    //! Get the command identifier
    //! 
    //! \return CommandId - Command identifier
    ///////////////////////////////////////////////////////////////////////////////
    CommandId  ident() const override
    {
      return Ident;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    const char_t*  name() const override
    {
      return Name.Text;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    ///////////////////////////////////////////////////////////////////////////////
    bool  permanent() const override
    {
      return Permanent;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::execute 
    //! Executes the command
    ///////////////////////////////////////////////////////////////////////////////
    void execute() override
    {
      // Execute
      Execute();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // GuiCommand::revert
    //! Reverts the command
    //! 
    //! \throw logic_error - Command cannot be reverted
    ///////////////////////////////////////////////////////////////////////////////
    void revert() override
    {
      // Verify
      if (permanent())
        throw logic_error(HERE, "Command cannot be reverted");

      // Revert
      Revert();
    }
  };
  

  

}

#endif // WTL_GUI_COMMAND_HPP
