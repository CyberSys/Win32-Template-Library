//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Command.hpp
//! \brief Encapsulates commands in the Gui using the 'Command'/'Command' pattern
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_HPP
#define WTL_COMMAND_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>          //!< Encoding
#include <wtl/utils/CharArray.hpp>                //!< CharArray
#include <wtl/windows/CommandId.hpp>             //!< CommandId
#include <wtl/resources/ResourceId.hpp>            //!< ResourceId
#include <wtl/resources/StringResource.hpp>       //!< StringResource
#include <wtl/resources/IconResource.hpp>         //!< IconResource
#include <memory>                                 //!< std::shared_ptr
#include <functional>                             //!< std::function

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum CommandSource - Defines how a Gui command was raised
  enum class CommandSource
  {
    MenuItem = 0,     //!< Command raised via menu item
    Accelerator = 1,  //!< Command raised via accelerator
  };

  //! \enum CommandState - Define states of GUI Commands
  enum class CommandState
  {
    Disabled = 0,     //!< Command should be disabled
    Enabled = 1,      //!< Command should be enabled
    Hidden = 2,       //!< Command should be hidden
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Command - Encapsulates a single gui command
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Command 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Command<ENC>;
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias icon_t - Define icon resource type
    using icon_t = IconResource;
    
    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
    //! \struct NameDecoder - Encapsulates decoding command name and description
    struct NameDecoder 
    { 
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
      
      //! \var LineFeed - Separator character
      static constexpr char_t  LineFeed = '\n';

      // ----------------------------------- REPRESENTATION -----------------------------------

      String<encoding>  Name,            //!< Command Name
                        Description;     //!< Command Description

      // ------------------------------------ CONSTRUCTION ------------------------------------
    
      /////////////////////////////////////////////////////////////////////////////////////////
      // NameDecoder::NameDecoder
      //! Create decoder from string id
      //! 
      //! \param[in] id - Name/description resource id
      /////////////////////////////////////////////////////////////////////////////////////////
      NameDecoder(resource_t id)
      {
        auto text = StringResource(id).c_str<encoding>();
      
        // [NAME/DESCRIPTION] Extract name & description
        if (text.contains(LineFeed))
        {
          int32_t sep = text.find(LineFeed);

          // Assign description and truncate name
          Name.assign(text.begin(), text.begin()+(sep+1));
          Description.assign(text.begin()+(sep+1), text.end());
        }
        // [NAME] Leave description blank
        else
          Name = text;
      }
    };

  protected:
    //! \alias execute_t - Define execute functor type
    using execute_t = std::function<void ()>;

    //! \alias revert_t - Define undo functor type
    using revert_t = std::function<void ()>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    CommandId   Ident;           //!< Command Id
    icon_t      Icon;            //!< Command Icon
    bool        Permanent;       //!< Whether command is permanent
    NameDecoder Decoder;         //!< Name + Description
    execute_t   ExecuteFn;       //!< Command execution functor
    revert_t    RevertFn;        //!< Command reversion functor
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::Command
    //! Create a permanent command
    //! 
    //! \param[in] id - Command identifier (Defining name, description, and icon resource)
    //! \param[in] exec - Callable target which implements executing command
    /////////////////////////////////////////////////////////////////////////////////////////
    Command(CommandId id, execute_t exec) : Ident(id),
                                            Icon(resource_id(id)),
                                            Decoder(resource_id(id)),
                                            Permanent(true),
                                            ExecuteFn(exec)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::Command
    //! Create a revertible command
    //! 
    //! \param[in] id - Command identifier
    //! \param[in] exec - Callable target which implements executing command
    //! \param[in] undo - Callable target which implements reverting command
    /////////////////////////////////////////////////////////////////////////////////////////
    Command(CommandId id, execute_t exec, revert_t undo) : Ident(id),
                                                           Icon(resource_id(id)),
                                                           Decoder(resource_id(id)),
                                                           Permanent(false),
                                                           ExecuteFn(exec),
                                                           RevertFn(undo)
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(Command);      //!< Can be shallow copied
    ENABLE_MOVE(Command);      //!< Can be moved 
    ENABLE_POLY(Command);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual type*  clone() const = 0;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::description const
    //! Get the command description
    //! 
    //! \return const String<encoding>& - Command description
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const String<encoding>&  description() const 
    {
      return Decoder.Description;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const 
    {
      return Icon.Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::ident const
    //! Get the command identifier
    //! 
    //! \return CommandId - Command identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual CommandId  ident() const 
    {
      return Ident;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::name const
    //! Get the command name
    //! 
    //! \return const String<encoding>& - Command name
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const String<encoding>&  name() const 
    {
      return Decoder.Name;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual bool  permanent() const 
    {
      return Permanent;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::state const
    //! Query the current state of the command 
    //! 
    //! \return CommandState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual CommandState state() const 
    {
      // Always enabled by default
      return CommandState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::execute 
    //! Executes the command
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void execute() 
    {
      // Execute
      ExecuteFn();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Command::revert
    //! Reverts the command
    //! 
    //! \throw logic_error - Command cannot be reverted
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void revert() 
    {
      // Verify
      if (permanent())
        throw logic_error(HERE, "Command cannot be reverted");

      // Revert
      RevertFn();
    }
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandPtr - Shared Command/Command pointer
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandPtr = std::shared_ptr<Command<ENC>>;
  
} // namespace wtl

#endif // WTL_COMMAND_HPP
