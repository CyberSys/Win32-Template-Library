//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\CommandGroupCollection.hpp
//! \brief Provides an 'CommandGroup' container
//! \date 7 July 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACTION_GROUP_COLLECTION_HPP
#define WTL_ACTION_GROUP_COLLECTION_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EncodingTraits.hpp"   //!< Encoding
#include "wtl/windows/CommandGroup.hpp"     //!< CommandGroupPtr
#include <map>                             //!< std::map

//! \namespace wtl - Windows template library
namespace wtl 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct CommandGroupCollection - Collection of Command groups, indexed by Id
  //! 
  //! \tparam ENC - Window character encoding
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
    //! Find an action within the collection
    //! 
    //! \return CommandPtr<ENC> - Shared Command pointer, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    CommandPtr<encoding>  find(CommandId id) const 
    {
      // Lookup action
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

#endif // WTL_ACTION_GROUP_COLLECTION_HPP
