//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\ActionGroupCollection.hpp
//! \brief Provides an 'ActionGroup' container
//! \date 7 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_GROUP_COLLECTION_HPP
#define WTL_ACTION_GROUP_COLLECTION_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"   //!< Encoding
#include "wtl/windows/ActionGroup.hpp"     //!< ActionGroupPtr
#include <map>                             //!< std::map

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ActionGroupCollection - Collection of Action groups, indexed by Id
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ActionGroupCollection : std::map<ActionGroupId,ActionGroupPtr<ENC>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = std::map<ActionGroupId,ActionGroupPtr<ENC>>;
      
    //! \alias type - Define own type
    using type = ActionGroupCollection;
      
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroupCollection::find const
    //! Find an action within the collection
    //! 
    //! \return ActionPtr<ENC> - Shared Action pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionPtr<encoding>  find(ActionId id) const 
    {
      // Lookup action
      for (const auto& group : *this)
        if (auto cmd = group.second->find(id))
          return cmd;

      // [NOT FOUND] Return empty pointer
      return ActionPtr<encoding>(nullptr);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
      
    /////////////////////////////////////////////////////////////////////////////////////////
    // ActionGroupCollection::operator +=
    //! Add a group to the collection
    //!
    //! \param[in] *group - Command group
    //! \return ActionGroupCollection& - Reference to self
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing group
    /////////////////////////////////////////////////////////////////////////////////////////
    ActionGroupCollection& operator += (ActionGroup<encoding>* group)
    {
      REQUIRED_PARAM(group);

      // Insert/overwrite
      emplace(group->ident(), ActionGroupPtr<encoding>(group));
      return *this;
    }
  };
    
} // namespace wtl

#endif // WTL_ACTION_GROUP_COLLECTION_HPP
