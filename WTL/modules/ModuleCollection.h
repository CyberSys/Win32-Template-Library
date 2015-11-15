//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\ModuleCollection.h
//! \brief Provides a collection of currently loaded modules
//! \date 14 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MODULE_COLLECTION_HPP
#define WTL_MODULE_COLLECTION_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/List.hpp>               //!< List
#include <wtl/utils/Default.hpp>            //!< defvalue
#include <wtl/traits/EncodingTraits.hpp>    //!< Encoding
#include <wtl/resources/ResourceBlob.hpp>   //!< ResourceBlob
#include <wtl/platform/ResourceId.hpp>      //!< ResourceId
#include <wtl/platform/SystemFlags.hpp>     //!< ResourceType
#include <wtl/modules/Module.h>             //!< Module
#include <functional>                       //!< std::forward, std::reference_wrapper

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ModuleCollection - Hosts all loaded modules
  /////////////////////////////////////////////////////////////////////////////////////////
  struct ModuleCollection 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias element_t - Define collection element type
    using element_t = std::reference_wrapper<const Module>;
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    List<element_t>   Items;       //!< Module storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::ModuleCollection
    //! Create empty collection
    /////////////////////////////////////////////////////////////////////////////////////////
    ModuleCollection() = default;

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::findResource
    //! Find a resource from any module in the collection
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] type - Resource type
    //! \param[in] name - Resource identifier
    //! \param[in] lang - Resource language
    //! \return ResourceBlob - Resource, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    ResourceBlob  findResource(ResourceType type, ResourceId<ENC> name, LanguageId language = LanguageId::Neutral) const
    {
      ResourceBlob res;

      // Search all modules for resource
      for (const element_t& m : Items)
      {
        // [FOUND] Find & return resource
        if ((res = m.get().findResource(type, name, language)) != defvalue<ResourceBlob>())
          return res;
      }
      
      // [NOT FOUND] Return default
      return defvalue<ResourceBlob>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::findString
    //! Find a string resource from any module in the collection
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] id - String identifier
    //! \param[in] lang - Resource language
    //! \return ResourceBlob - Resource, possibly empty
    //!
    //! \throw wtl::invalid_argument - String ids must be numeric constants
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    ResourceBlob  findString(ResourceId<ENC> id, LanguageId language = LanguageId::Neutral) const
    { 
      if (!id.isOrdinal())
        throw invalid_argument(HERE, "String ids must be numeric constants");

      // Load string table handle
      return findResource<ENC>(ResourceType::String, (id.Value.Numeral/16)+1, language);
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::add
    //! Add a module to the collection
    //!
    //! \param[in] const& m - Module
    /////////////////////////////////////////////////////////////////////////////////////////
    void  add(const Module& m)
    {
      Items.emplace_back(m);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::remove
    //! Remove a module to the collection
    //!
    //! \param[in] const& m - Module
    /////////////////////////////////////////////////////////////////////////////////////////
    void  remove(const Module& m)
    {
      Items.remove_if( [&m] (const element_t& w) { return &w.get() == &m; } );
    }
  };

  //! \var LoadedModules - Loaded modules collection
  extern ModuleCollection  LoadedModules;

}

#endif // WTL_MODULE_COLLECTION_HPP
