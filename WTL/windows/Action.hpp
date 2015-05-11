//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Action.hpp
//! \brief Encapsulates commands in the Gui using the 'Action'/'Command' pattern
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_HPP
#define WTL_ACTION_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Action - Encapsulates a single gui command
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct Action 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Action<ENC>;
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias icon_t - Define icon resource type
    using icon_t = IconResource;
    
    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;

    //! \var encoding - Define encoding type
    static constexpr Encoding  encoding = ENC;
    
    //! \struct NameStringResource - Encapsulates decoding command name and description
    struct NameStringResource 
    { 
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
      
      //! \alias description_t - Define description string resource type
      using description_t = CharArray<encoding,1024>;

      //! \alias name_t - Define name string resource type
      using name_t = CharArray<encoding,128>;

      //! \var LineFeed - 
      static constexpr char_t  LineFeed = '\n';

      // ----------------------------------- REPRESENTATION -----------------------------------

      name_t         Name;            //!< Command Name
      description_t  Description;     //!< Command Description

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
      /////////////////////////////////////////////////////////////////////////////////////////
      // NameStringResource::NameStringResource
      //! Create decoder from string id
      //! 
      //! \param[in] id - Name/description resource id
      /////////////////////////////////////////////////////////////////////////////////////////
      NameStringResource(resource_t id)
      {
        auto text = StringResource(id).c_arr<encoding,1024>();
      
        // [NAME/DESCRIPTION] Extract name & description
        if (text.contains(LineFeed))
        {
          int32 sep = text.find(LineFeed);

          // Assign description and truncate name
          Name.assign<encoding>(text.begin(), text.begin()+(sep+1));
          Description.assign<encoding>(text.begin()+(sep+1), text.end());
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
    
    //! \using decoder_t - Name/description string type
    using decoder_t = NameStringResource;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    CommandId   Ident;           //!< Command Id
    icon_t      Icon;            //!< Command Icon
    bool        Permanent;       //!< Whether command is permanent
    decoder_t   NameString;      //!< Name + Description
    execute_t   ExecuteFn;       //!< Command execution functor
    revert_t    RevertFn;        //!< Command reversion functor

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::Action
    //! Create a permanent command
    //! 
    //! \param[in] id - Command identifier (Defining name, description, and icon resource)
    //! \param[in] exec - Callable target which implements executing command
    /////////////////////////////////////////////////////////////////////////////////////////
    Action(CommandId id, execute_t exec) : Ident(id),
                                           Icon(resource_id(id)),
                                           NameString(resource_id(id)),
                                           Permanent(true),
                                           ExecuteFn(exec)
    {}
    

    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::Action
    //! Create a revertible command
    //! 
    //! \param[in] id - Command identifier
    //! \param[in] exec - Callable target which implements executing command
    //! \param[in] undo - Callable target which implements reverting command
    /////////////////////////////////////////////////////////////////////////////////////////
    Action(CommandId id, execute_t exec, revert_t undo) : Ident(id),
                                                          Icon(resource_id(id)),
                                                          NameString(resource_id(id)),
                                                          Permanent(false),
                                                          ExecuteFn(exec),
                                                          RevertFn(undo)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::~Action
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Action() 
    {}
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::clone const
    //! Create a new instance of the command
    //! 
    //! \return type* - New instance of command
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual type*  clone() const = 0;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::description const
    //! Get the command description
    //! 
    //! \return const decoder_t::description_t& - Command description
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const typename decoder_t::description_t&  description() const 
    {
      return NameString.Description;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::icon const
    //! Get the command icon
    //! 
    //! \return HIcon - Shared icon handle
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const 
    {
      return Icon.Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::ident const
    //! Get the command identifier
    //! 
    //! \return CommandId - Command identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual CommandId  ident() const 
    {
      return Ident;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::name const
    //! Get the command name
    //! 
    //! \return const decoder_t::name_t& - Command name
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const typename decoder_t::name_t&  name() const 
    {
      return NameString.Name;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::permanent const
    //! Query the whether the command can be reverted
    //! 
    //! \return bool - True iff command is permanent (cannot be undone)
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual bool  permanent() const 
    {
      return Permanent;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::state const
    //! Query the current state of the command 
    //! 
    //! \return ActionState - Current state of command
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ActionState state() const 
    {
      // Always enabled by default
      return ActionState::Enabled;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::execute 
    //! Executes the command
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void execute() 
    {
      // Execute
      ExecuteFn();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Action::revert
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
  //! \alias ActionPtr - Shared Action/Command pointer
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionPtr = std::shared_ptr<Action<ENC>>;
  

}

#endif // WTL_ACTION_HPP
