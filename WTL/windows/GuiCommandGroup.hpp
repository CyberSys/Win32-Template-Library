////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\ActionGroup.hpp
//! \brief Provides a collection of gui actions (for usage with menus/toolbars)
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_GROUP_HPP
#define WTL_ACTION_GROUP_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct ActionGroup - Provides a collection of Gui Commands, indexed by Command Id
  //! 
  //! \tparam ENC - Command character encoding
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ActionGroup : std::map<CommandId,ActionPtr<ENC>>
  {
    //! \alias base - Define base type
    using base = std::map<CommandId,ActionPtr<ENC>>;

    //! \alias type - Define own type
    using type = ActionGroup<ENC>;
    
    //! \alias action_t - Define action pointer type
    using action_t = Action<ENC>;
    
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
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // -------------------- REPRESENTATION ---------------------
  protected:
    CommandGroupId  Ident;           //!< Command Id
    name_t          Name;            //!< Command Name
    description_t   Description;     //!< Command Description
    icon_t          Icon;            //!< Command Icon
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::ActionGroup
    //! Create empty collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    ///////////////////////////////////////////////////////////////////////////////
    ActionGroup(CommandGroupId id) : Ident(id),
                                     Name(resource_id(id)),
                                     Description(resource_id(id)),
                                     Icon(resource_id(id))
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::ActionGroup
    //! Create populated collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    //! \param[in] cmds - List of actions
    ///////////////////////////////////////////////////////////////////////////////
    ActionGroup(CommandGroupId id, std::initializer_list<action_t*>&& cmds) : ActionGroup(id)
    {
      // Populate
      for (action_t* c : cmds)
        *this += c;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::~ActionGroup
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~ActionGroup() 
    {}
    
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::description const
    //! Get the command description
    //! 
    //! \return char_t* - Command description
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  description() const 
    {
      return Description.Text;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::find const
    //! Find an action within the group
    //! 
    //! \return ActionPtr<encoding> - Shared action pointer, possibly empty
    ///////////////////////////////////////////////////////////////////////////////
    ActionPtr<encoding>  find(CommandId id) const 
    {
      // Lookup action & return if found
      auto pos = base::find(id);
      if (pos != base::end())
        return pos->second; 
      
      // [NOT FOUND] Return nullptr sentinel
      return ActionPtr<encoding>(nullptr);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::icon const
    //! Get the group icon
    //! 
    //! \return HIcon - Shared icon handle
    ///////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const 
    {
      return Icon.Handle;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::ident const
    //! Get the group identifier
    //! 
    //! \return CommandGroupId - Action group identifier
    ///////////////////////////////////////////////////////////////////////////////
    virtual CommandGroupId  ident() const 
    {
      return Ident;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::name const
    //! Get the group name
    //! 
    //! \return char_t* - Group name
    ///////////////////////////////////////////////////////////////////////////////
    virtual const char_t*  name() const 
    {
      return Name.Text;
    }
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // ActionGroup::operator +=
    //! Add an action to the group
    //!
    //! \param[in] *cmd - Action
    //! \return ActionGroup& - Reference to self
    ///////////////////////////////////////////////////////////////////////////////
    ActionGroup& operator += (action_t* cmd)
    {
      // Insert/overwrite
      this->emplace(cmd->ident(), ActionPtr<encoding>(cmd));
      return *this;
    }
  };
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias ActionGroupPtr - Shared Action group pointer
  //! 
  //! \tparam ENC - Command character encoding 
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionGroupPtr = std::shared_ptr<ActionGroup<ENC>>;


}

#endif // WTL_ACTION_GROUP_HPP
