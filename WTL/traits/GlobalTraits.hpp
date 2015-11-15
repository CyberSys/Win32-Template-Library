//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\GlobalTraits.hpp
//! \brief Defines global memory handle traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_GLOBAL_MEM_TRAITS_HPP
#define WTL_GLOBAL_MEM_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>
#include <wtl/utils/Default.hpp>
#include <wtl/traits/ModuleTraits.hpp>
#include <wtl/traits/ResourceTraits.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct hglobal_allocator - Encapsulates allocating global memory handles  (which are only typedefs for ::HANDLE)
  /////////////////////////////////////////////////////////////////////////////////////////
  struct hglobal_allocator
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HGLOBAL npos = defvalue<::HGLOBAL>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(hglobal_allocator);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(hglobal_allocator);     //!< Cannot instantiate
    DISABLE_MOVE(hglobal_allocator);     //!< Cannot instantiate
    DISABLE_DTOR(hglobal_allocator);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // hglobal_allocator::create
    //! Create global memory handle to resource
    //! 
    //! \param[in] const& module - Module containing resource
    //! \param[in] const& resource - Resource handle
    //! \return NativeHandle<::HGLOBAL> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HGLOBAL> create(const HModule& module, const HResource& resource) 
    { 
      // Load resource
      if (::HGLOBAL res = ::LoadResource(module, resource))
        return { res, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load resource");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // hglobal_allocator::clone
    //! Clone handle
    //! 
    //! \param[in] mem - Global memory handle
    //! \return NativeHandle<::HGLOBAL> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HGLOBAL> clone(NativeHandle<::HGLOBAL> mem);

    /////////////////////////////////////////////////////////////////////////////////////////
    // hglobal_allocator::destroy noexcept
    //! Release handle
    //! 
    //! \param[in] mem - Global memory handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HGLOBAL> mem) noexcept
    {
      // Delete without checking if handle is valid
      switch (mem.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return false;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HGlobal - Shared global memory handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HGlobal = Handle<::HGLOBAL, hglobal_allocator>;

} //namespace wtl
#endif // WTL_GLOBAL_MEM_TRAITS_HPP

