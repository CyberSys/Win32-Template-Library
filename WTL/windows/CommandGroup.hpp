//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\CommandGroup.hpp
//! \brief Provides a collection of gui actions (for usage with menus/toolbars)
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_GROUP_HPP
#define WTL_ACTION_GROUP_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Command.hpp"          //!< Command
#include <map>                             //!< std::map
#include <memory>                          //!< std::shared_ptr

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CommandGroup - Provides a collection of Gui Commands, indexed by Command Id
  //! 
  //! \tparam ENC - Command character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CommandGroup : std::map<CommandId,CommandPtr<ENC>>
  {
    //! \alias base - Define base type
    using base = std::map<CommandId,CommandPtr<ENC>>;

    //! \alias type - Define own type
    using type = CommandGroup<ENC>;
    
    //! \alias action_t - Define action pointer type
    using action_t = Command<ENC>;
    
    //! \alias char_t - Define character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias icon_t - Define icon resource type
    using icon_t = IconResource;

    //! \alias resource_t - Define resource ident type
    using resource_t = ResourceId<ENC>;
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;
  
  protected:
    //! \using decoder_t - Name/description string type
    using decoder_t = typename Command<encoding>::NameStringResource;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    CommandGroupId  Ident;           //!< Command Id
    decoder_t       NameString;      //!< Name + Description
    icon_t          Icon;            //!< Command Icon
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::CommandGroup
    //! Create empty collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroup(CommandGroupId id) : Ident(id),
                                     NameString(resource_id(id)),
                                     Icon(resource_id(id))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::CommandGroup
    //! Create populated collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    //! \param[in] cmds - List of actions
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroup(CommandGroupId id, std::initializer_list<action_t*>&& cmds) : CommandGroup(id)
    {
      // Populate
      for (action_t* c : cmds)
        *this += c;
    }
    
	  // -------------------------------- COPYING & DESTRUCTION -------------------------------
  public:
    ENABLE_COPY(CommandGroup);      //!< Can be shallow copied
    ENABLE_MOVE(CommandGroup);      //!< Can be moved
    ENABLE_POLY(CommandGroup);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::description const
    //! Get the group description
    //! 
    //! \return const description_t::string_t& - Group description
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const typename decoder_t::description_t&  description() const 
    {
      return NameString.Description;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::find const
    //! Find an action within the group
    //! 
    //! \return CommandPtr<encoding> - Shared action pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandPtr<encoding>  find(CommandId id) const 
    {
      // Lookup action & return if found
      auto pos = base::find(id);
      if (pos != base::end())
        return pos->second; 
      
      // [NOT FOUND] Return nullptr sentinel
      return CommandPtr<encoding>(nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::icon const
    //! Get the group icon
    //! 
    //! \return HIcon - Shared icon handle
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const 
    {
      return Icon.Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::ident const
    //! Get the group identifier
    //! 
    //! \return CommandGroupId - Group identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual CommandGroupId  ident() const 
    {
      return Ident;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::name const
    //! Get the group name
    //! 
    //! \return const decoder_t::name_t& - Group name
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const typename decoder_t::name_t&  name() const 
    {
      return NameString.Name;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroup::operator +=
    //! Add an action to the group
    //!
    //! \param[in] *cmd - Command
    //! \return CommandGroup& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroup& operator += (action_t* cmd)
    {
      // Insert/overwrite
      this->emplace(cmd->ident(), CommandPtr<encoding>(cmd));
      return *this;
    }
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias CommandGroupPtr - Shared Command group pointer
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using CommandGroupPtr = std::shared_ptr<CommandGroup<ENC>>;
        
} // namespace wtl

#endif // WTL_ACTION_GROUP_HPP
