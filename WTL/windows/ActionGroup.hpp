//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\ActionGroup.hpp
//! \brief Provides a collection of gui actions (for usage with menus/toolbars)
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_GROUP_HPP
#define WTL_ACTION_GROUP_HPP

#include "wtl/WTL.hpp"
#include "wtl/windows/Action.hpp"          //!< Action
#include <map>                             //!< std::map
#include <memory>                          //!< std::shared_ptr

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ActionGroup - Provides a collection of Gui Commands, indexed by Command Id
  //! 
  //! \tparam ENC - Command character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ActionGroup : std::map<ActionId,ActionPtr<ENC>>
  {
    //! \alias base - Define base type
    using base = std::map<ActionId,ActionPtr<ENC>>;

    //! \alias type - Define own type
    using type = ActionGroup<ENC>;
    
    //! \alias action_t - Define action pointer type
    using action_t = Action<ENC>;
    
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
    using decoder_t = typename Action<encoding>::NameStringResource;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ActionGroupId  Ident;           //!< Command Id
    decoder_t       NameString;      //!< Name + Description
    icon_t          Icon;            //!< Command Icon
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::ActionGroup
    //! Create empty collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionGroup(ActionGroupId id) : Ident(id),
                                     NameString(resource_id(id)),
                                     Icon(resource_id(id))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::ActionGroup
    //! Create populated collection
    //! 
    //! \param[in] id - Group id   (Defining name, description, and icon resource)
    //! \param[in] cmds - List of actions
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionGroup(ActionGroupId id, std::initializer_list<action_t*>&& cmds) : ActionGroup(id)
    {
      // Populate
      for (action_t* c : cmds)
        *this += c;
    }
    
	  // -------------------------------- COPYING & DESTRUCTION -------------------------------
  public:
    ENABLE_COPY(ActionGroup);      //!< Can be shallow copied
    ENABLE_MOVE(ActionGroup);      //!< Can be moved
    ENABLE_POLY(ActionGroup);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::description const
    //! Get the group description
    //! 
    //! \return const description_t::string_t& - Group description
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const typename decoder_t::description_t&  description() const 
    {
      return NameString.Description;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::find const
    //! Find an action within the group
    //! 
    //! \return ActionPtr<encoding> - Shared action pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionPtr<encoding>  find(ActionId id) const 
    {
      // Lookup action & return if found
      auto pos = base::find(id);
      if (pos != base::end())
        return pos->second; 
      
      // [NOT FOUND] Return nullptr sentinel
      return ActionPtr<encoding>(nullptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::icon const
    //! Get the group icon
    //! 
    //! \return HIcon - Shared icon handle
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual HIcon  icon() const 
    {
      return Icon.Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::ident const
    //! Get the group identifier
    //! 
    //! \return ActionGroupId - Group identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ActionGroupId  ident() const 
    {
      return Ident;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroup::name const
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
    // ActionGroup::operator +=
    //! Add an action to the group
    //!
    //! \param[in] *cmd - Action
    //! \return ActionGroup& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionGroup& operator += (action_t* cmd)
    {
      // Insert/overwrite
      this->emplace(cmd->ident(), ActionPtr<encoding>(cmd));
      return *this;
    }
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias ActionGroupPtr - Shared Action group pointer
  //! 
  //! \tparam ENC - Command character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  using ActionGroupPtr = std::shared_ptr<ActionGroup<ENC>>;
        
} // namespace wtl

#endif // WTL_ACTION_GROUP_HPP
