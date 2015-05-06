//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\ResourceTraits.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RESOURCE_TRAITS_HPP
#define WTL_RESOURCE_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/ModuleTraits.hpp"
#include "wtl/platform/ResourceId.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HResource - Shared resource handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HResource = Handle<::HRSRC>;
  
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HRSRC> - Encapsulates allocating resource handles
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HRSRC>
  {
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HRSRC npos = default<::HRSRC>(); 

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HRSRC>::create
    //! Create resource handle
    //! 
    //! \tparam ENC - Resource name character encoding 
    //!
    //! \param[in] const& module - Module containing resource
    //! \param[in] name - Resource identifier
    //! \param[in] type - Resource type
    //! \param[in] lang - Resource language
    //! \return HAlloc<::HRSRC> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<::HRSRC> create(const HModule& module, ResourceId<ENC> name, ResourceType type, LanguageId language) 
    { 
      // Load resource handle
      if (::HRSRC res = getFunc<ENC>(::FindResourceExA,::FindResourceExW)(module, ResourceId(type), name, language))
        return { res, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to find resource");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HRSRC>::clone
    //! Clone handle
    //! 
    //! \param[in] addr - Handle
    //! \return HAlloc<::HRSRC> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HRSRC> clone(HAlloc<::HRSRC> addr);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HRSRC>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] addr - Handle
    //! \return bool - Always true
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HRSRC> addr) noexcept
    {
      // Resource handles are not released by module under Win32
      return true;
    }
  };
  
} //namespace wtl
#endif // WTL_RESOURCE_TRAITS_HPP

