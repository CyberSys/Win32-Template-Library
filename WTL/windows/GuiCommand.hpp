////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Action.hpp
//! \brief Encapsulates commands in the Gui using the 'Action'/'Command' pattern
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_HPP
#define WTL_ACTION_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Action - Encapsulates a single gui command
  //! 
  //! \tparam ENC - Command character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Action 
  {
    // ------------------- TYPES & CONSTANTS -------------------
    
    //! \alias type - Define own type
    using type = Action<ENC>;
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias description_t - Define description string resource type
    using description_t = StringResource<ENC,1024>;

    //! \alias icon_t - Define icon resource type
    using icon_t = IconResource;
    
    //! \alias name_t - Define name string resource type
    using name_t = StringResource<ENC,128>;
    
    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
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
    execute_t      ExecuteFn;       //!< Command execution functor
    revert_t       RevertFn;        //!< Command reversion functor

    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Action::Action
    //! Create a permenant command
    //! 
    //! \param[in] id - Command identifier (Defining name, description, and icon resource)
    //! \param[in] exec - Callable target which implements executing command
    ///////////////////////////////////////////////////////////////////////////////
    Action(CommandId id, execute_t exec) : Ident(id),
                                           Name(resource_id(id)),
                                           Description(resource_id(id)),
                                           Icon(resource_id(id)),
                                           Permanent(true),
                                           ExecuteFn(exec)
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Action::Action
    //! Create a revertible command
    //! 
    //! \param[in] id - Command identifier
    //! \param[in] exec - Callable target which implements executing command
    //! \param[in] undo - Callable target which implements reverting command
    ///////////////////////////////////////////////////////////////////////////////
    Action(CommandId id, execute_t exec, revert_t undo) : Ident(id),
                                                          Name(resource_id(id)),
                                                          Description(resource_id(id)),
                                                          Icon(resource_id(id)),
                                                          Permanent(false),
                                                          ExecuteFn(exec),
                                                          RevertFn(undo)
    {}
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::~Action
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~Action() 
    {}
    
    // ---------------------- ACCESSORS ------------------------			
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual type*  clone() const = 0;

    ///////////////////////////////////////////////////////////////////////////////
    // Action::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const 
    {
      return Description.Text;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Action::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const 
    {
      return Icon.Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::ident const
    //! Get the command identifier
    //! 
    //! \return CommandId - Command identifier
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandId  ident() const 
    {
      return Ident;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::name const
    //! Get the command name
    //! 
    //! \return char_t* - Command name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  name() const 
    {
      return Name.Text;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    ///////////////////////////////////////////////////////////////////////////////
    virtual bool  permanent() const 
    {
      return Permanent;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::state const
    //! Query the current state of the command 
    //! 
    //! \return ActionState - Current state of command
    ///////////////////////////////////////////////////////////////////////////////
    virtual ActionState state() const 
    {
      // Always enabled by default
      return ActionState::Enabled;
    }
    
    // ----------------------- MUTATORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::execute 
    //! Executes the command
    ///////////////////////////////////////////////////////////////////////////////
    virtual void execute() 
    {
      // Execute
      ExecuteFn();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Action::revert
    //! Reverts the command
    //! 
    //! \throw logic_error - Command cannot be reverted
    ///////////////////////////////////////////////////////////////////////////////
    virtual void revert() 
    {
      // Verify
      if (permanent())
        throw logic_error(HERE, "Command cannot be reverted");

      // Revert
      RevertFn();
    }
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ActionPtr - Shared Action/Command pointer
  //! 
  //! \tparam ENC - Command character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionPtr = std::shared_ptr<Action<ENC>>;
  

}

#endif // WTL_ACTION_HPP
