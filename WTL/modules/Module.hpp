//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Module.hpp
//! \brief Encapsulates a module instance
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MODULE_HPP
#define WTL_MODULE_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"             //!< Handle
#include "wtl/utils/List.hpp"               //!< List
#include "wtl/utils/Default.hpp"            //!< Default
#include "wtl/traits/EncodingTraits.hpp"    //!< Encoding
#include "wtl/traits/ModuleTraits.hpp"      //!< HModule
#include "wtl/traits/ResourceTraits.hpp"    //!< HResource
#include "wtl/resources/ResourceBlob.hpp"   //!< ResourceBlob
#include "wtl/platform/ResourceId.hpp"      //!< ResourceId
#include <functional>                       //!< std::forward

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Module - Encapsulates a module (.dll, .exe)
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Module
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HModule   Handle;       //!< Module handle

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::Module
    //! Create from native module handle. Adds module to 'Loaded Modules' collection.
    //!
    //! \param[in] m - Native module handle
    /////////////////////////////////////////////////////////////////////////////////////////
    Module(::HMODULE m);
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Module);       //!< Cannot be copied
    ENABLE_MOVE(Module);        //!< Can be moved
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::~Module
    //! Can be polymorphic
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Module();
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // TODO: Execute exported function 
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::contains const
    //! Query whether a resource is present
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] type - Resource type
    //! \param[in] name - Resource identifier
    //! \param[in] lang - Resource language
    //! \return HAlloc<::HRSRC> - Accquired handle if found, otherwise 'npos'
    /////////////////////////////////////////////////////////////////////////////////////////
    //template <Encoding ENC>
    //Resource  findResource(ResourceType type, ResourceId<ENC> name, LanguageId language = LanguageId::Neutral) const
    //{ 
    //  // Load resource handle
    //  if (::HRSRC res = getFunc<ENC>(::FindResourceExA,::FindResourceExW)(Handle, ResourceId<ENC>(type), name, language))
    //    return Resource(Handle, HResource(res, AllocType::Accquire));

    //  // [NOT FOUND] Return sentinel
    //  return Resource::npos;
    //}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::findResource
    //! Find a resource 
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
      // Load resource handle
      if (::HRSRC res = getFunc<ENC>(::FindResourceExA,::FindResourceExW)(Handle, ResourceId<ENC>(type), name, language))
        return ResourceBlob(Handle, HResource(res, AllocType::Accquire));

      // [NOT FOUND] Return sentinel
      return default<ResourceBlob>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::findString
    //! Find a string resource 
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
      return findResource(ResourceType::String, (id.Value.Numeral/16)+1, language);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::load
    //! Loads a string resource from the module
    //! 
    //! \tparam LEN - Output buffer capacity
    //! \tparam ENC - Output string character encoding 
    //! 
    //! \param[in] id - Resource id
    //! \param[in,out] &str - Output buffer
    //! \param[in] lang - [optional] String language
    //! 
    //! \throw wtl::domain_error - Missing string -or- Insufficient buffer capacity to store string
    //! \throw wtl::platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    //template <unsigned LEN, Encoding ENC = Encoding::UTF16>
    //void load(ResourceId<ENC> id, CharArray<ENC,LEN>& str, LanguageId lang = LanguageId::Neutral) const
    //{
    //  // Load string from module
    //  str = StringResource(Handle, id, lang);
    //}

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };


  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ModuleCollection - Hosts all loaded modules
  /////////////////////////////////////////////////////////////////////////////////////////
  struct ModuleCollection : protected List<std::reference_wrapper<const Module>>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = List<std::reference_wrapper<const Module>>;

    //! \alias element_t - Define collection element type
    using element_t = std::reference_wrapper<const Module>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

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
      for (const element_t& m : *this)
      {
        // [FOUND] Find & return resource
        if ((res = m.get().findResource(type, name, language)) != default<ResourceBlob>())
          return res;
      }
      
      // [NOT FOUND] Return default
      return default<ResourceBlob>();
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
      base::emplace_back(m);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::remove
    //! Remove a module to the collection
    //!
    //! \param[in] const& m - Module
    /////////////////////////////////////////////////////////////////////////////////////////
    void  remove(const Module& m)
    {
      base::remove_if( [&m] (const element_t& w) { return &w.get() == &m; } );
    }
  };

  //! \var LoadedModules - Loaded modules collection
  extern ModuleCollection  LoadedModules;

}

#endif // WTL_MODULE_HPP
