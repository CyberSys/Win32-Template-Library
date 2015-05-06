//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\ModuleTraits.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MODULE_TRAITS_HPP
#define WTL_MODULE_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HModule - Shared module handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HModule = Handle<::HMODULE>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HMODULE> - Encapsulates allocating module handles
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HMODULE>
  {
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HMODULE npos = default<::HMODULE>(); 

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMODULE>::create
    //! Load an external module
    //! 
    //! \param[in] const& fullPath - Absolute path of module
    //! \return HAlloc<HMENU> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to load module
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<::HMODULE> create(const Path<ENC>& fullPath) 
    { 
      // Load external library
      if (::HMODULE module = getFunc<ENC>(::LoadLibraryA,::LoadLibraryW)(fullPath))
        return { module, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load external module");
    }
    

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMODULE>::clone
    //! Clone handle
    //! 
    //! \param[in] module - Handle
    //! \return HAlloc<::HMODULE> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HMODULE> clone(HAlloc<::HMODULE> module);


    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMODULE>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] module - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HMODULE> module) noexcept
    {
      // Delete without checking if handle is valid
      switch (module.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return ::FreeLibrary(module.Handle) != FALSE;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
  };

  
} //namespace wtl
#endif // WTL_MODULE_TRAITS_HPP

