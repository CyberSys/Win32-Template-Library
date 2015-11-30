//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Module.h
//! \brief Encapsulates a module instance
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MODULE_HPP
#define WTL_MODULE_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>             //!< Handle
#include <wtl/utils/List.hpp>               //!< List
#include <wtl/utils/Default.hpp>            //!< Default
#include <wtl/traits/EncodingTraits.hpp>    //!< Encoding
#include <wtl/traits/ModuleTraits.hpp>      //!< HModule
#include <wtl/traits/ResourceTraits.hpp>    //!< HResource
#include <wtl/resources/ResourceBlob.hpp>   //!< ResourceBlob
#include <wtl/resources/ResourceId.hpp>      //!< ResourceId
#include <wtl/utils/Path.hpp>            //!< Path
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
    //! Removes module from 'Loaded Modules' collection.
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Module();
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // TODO: Execute exported function 
    
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
      if (::HRSRC res = WinAPI<ENC>::findResourceEx(Handle, ResourceId<ENC>(type), name, language))
        return ResourceBlob(Handle, HResource(res, AllocType::Accquire));

      // [NOT FOUND] Return sentinel
      return defvalue<ResourceBlob>();
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
    // Module::path const
    //! Get the full path of the module
    //! 
    //! \tparam ENC - Path character encoding
    //! 
    //! \return Path<ENC> - Full path of module
    //! 
    //! \throw wtl::platform_error - Unable to query path
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    Path<ENC> path() const
    {
      Path<ENC> p;

      // Query module path
      if (!WinAPI<ENC>::getModuleFileName(Handle, p.buffer(), Path<ENC>::length))
        throw platform_error(HERE, "Unable to query module path");
      return p;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };


}

#endif // WTL_MODULE_HPP
