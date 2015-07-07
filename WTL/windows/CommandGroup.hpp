//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\CommandGroup.hpp
//! \brief Provides a collection of gui commands (for usage with menus/toolbars)
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_COMMAND_GROUP_HPP
#define WTL_COMMAND_GROUP_HPP

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
    
    //! \alias command_t - Define command pointer type
    using command_t = Command<ENC>;
    
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
    //! \param[in] cmds - List of commands
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroup(CommandGroupId id, std::initializer_list<command_t*>&& cmds) : CommandGroup(id)
    {
      // Populate
      for (command_t* c : cmds)
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
    //! Find an command within the group
    //! 
    //! \return CommandPtr<encoding> - Shared command pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandPtr<encoding>  find(CommandId id) const 
    {
      // Lookup command & return if found
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
    //! Add an command to the group
    //!
    //! \param[in] *cmd - Command
    //! \return CommandGroup& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroup& operator += (command_t* cmd)
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
        


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CommandGroupCollection - Provides a convenient container for groups of Gui Commands
  //! 
  //! \tparam ENC - Command character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct CommandGroupCollection : std::map<CommandGroupId,CommandGroupPtr<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = std::map<CommandGroupId,CommandGroupPtr<ENC>>;
      
    //! \alias type - Define own type
    using type = CommandGroupCollection;
      
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroupCollection::find const
    //! Recursively searches each group for a command
    //! 
    //! \param[in] id - Command identifier
    //! \return CommandPtr<ENC> - Shared Command pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandPtr<encoding>  find(CommandId id) const 
    {
      // Lookup command
      for (const auto& group : *this)
        if (auto cmd = group.second->find(id))
          return cmd;

      // [NOT FOUND] Return empty pointer
      return CommandPtr<encoding>(nullptr);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
      
    /////////////////////////////////////////////////////////////////////////////////////////
    // CommandGroupCollection::operator +=
    //! Add a group to the collection
    //!
    //! \param[in] *group - Command group
    //! \return CommandGroupCollection& - Reference to self
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing group
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandGroupCollection& operator += (CommandGroup<encoding>* group)
    {
      REQUIRED_PARAM(group);

      // Insert/overwrite
      emplace(group->ident(), CommandGroupPtr<encoding>(group));
      return *this;
    }
  };
} // namespace wtl

#endif // WTL_COMMAND_GROUP_HPP
