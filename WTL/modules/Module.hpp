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
#include "wtl/traits/ModuleTraits.hpp"

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
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::~Module
    //! Virtual d-tor. Removes module from 'Loaded Modules' collection.
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Module();

    DISABLE_COPY(Module);           //!< Modules are unique
    DEFAULT_MOVE(Module);      //!< Modules can be moved
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			
    
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
    //! \return Resource - Resource, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    Resource  findResource(ResourceType type, ResourceId<ENC> name, LanguageId language = LanguageId::Neutral) const
    { 
      // Load resource handle
      if (::HRSRC res = getFunc<ENC>(::FindResourceExA,::FindResourceExW)(Handle, ResourceId<ENC>(type), name, language))
        return Resource(Handle, HResource(res, AllocType::Accquire));

      // [NOT FOUND] Return sentinel
      return default<Resource>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Module::findString
    //! Find a string resource 
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] id - String identifier
    //! \param[in] lang - Resource language
    //! \return Resource - Resource, possibly empty
    //!
    //! \throw wtl::invalid_argument - String ids must be numeric constants
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    Resource  findString(ResourceId<ENC> id, LanguageId language = LanguageId::Neutral) const
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

    // --------------------------------------- MUTATORS --------------------------------------

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

    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::findResource
    //! Find a resource from any module in the collection
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] type - Resource type
    //! \param[in] name - Resource identifier
    //! \param[in] lang - Resource language
    //! \return Resource - Resource, possibly empty
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    Resource  findResource(ResourceType type, ResourceId<ENC> name, LanguageId language = LanguageId::Neutral) const
    {
      Resource res;

      // Search all modules for resource
      for (const element_t& m : *this)
      {
        // [FOUND] Find & return resource
        if ((res = m.get().findResource(type, name, language)) != default<Resource>())
          return res;
      }
      
      // [NOT FOUND] Return default
      return default<Resource>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ModuleCollection::findString
    //! Find a string resource from any module in the collection
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] id - String identifier
    //! \param[in] lang - Resource language
    //! \return Resource - Resource, possibly empty
    //!
    //! \throw wtl::invalid_argument - String ids must be numeric constants
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    Resource  findString(ResourceId<ENC> id, LanguageId language = LanguageId::Neutral) const
    { 
      if (!id.isOrdinal())
        throw invalid_argument(HERE, "String ids must be numeric constants");

      // Load string table handle
      return findResource<ENC>(ResourceType::String, (id.Value.Numeral/16)+1, language);
    }
    
    // --------------------------------------- MUTATORS --------------------------------------
    
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
