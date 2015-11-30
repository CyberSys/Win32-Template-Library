//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\ModuleTraits.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MODULE_TRAITS_HPP
#define WTL_MODULE_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>
#include <wtl/utils/Default.hpp>
#include <wtl/utils/Path.hpp>

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
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HMODULE npos = defvalue<::HMODULE>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMODULE>::create
    //! Load an external module
    //! 
    //! \param[in] const& fullPath - Absolute path of module
    //! \return NativeHandle<HMENU> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to load module
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static NativeHandle<::HMODULE> create(const Path<ENC>& fullPath) 
    { 
      // Load external library
      if (::HMODULE module = WinAPI<ENC>::loadLibrary(fullPath))
        return { module, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load external module");
    }
    

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMODULE>::clone
    //! Clone handle
    //! 
    //! \param[in] module - Handle
    //! \return NativeHandle<::HMODULE> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HMODULE> clone(NativeHandle<::HMODULE> module);


    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HMODULE>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] module - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HMODULE> module) noexcept
    {
      // Delete without checking if handle is valid
      switch (module.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return ::FreeLibrary(module.Handle) != False;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
} //namespace wtl
#endif // WTL_MODULE_TRAITS_HPP

