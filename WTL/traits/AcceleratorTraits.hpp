//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\AcceleratorTraits.hpp
//! \brief Defines accelerator traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ACCELERATOR_TRAITS_HPP
#define WTL_ACCELERATOR_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HAccelerator - Shared accelerator table handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HAccelerator = Handle<::HACCEL>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HACCEL> - Encapsulates accelerator handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HACCEL>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HACCEL npos = defvalue<::HACCEL>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HACCEL>::create
    //! Load accelerator table handle
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] instance - Instance containing cursor
    //! \param[in] ident - Accelerator identifier
    //! \return NativeHandle<HACCEL> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static NativeHandle<HACCEL> create(HINSTANCE instance, ResourceId<ENC> ident) 
    { 
      // Create solid colour brush handle
      if (HACCEL accel = WinAPI<ENC>::loadAccelerators(instance, ident))
        return { accel, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load accelerator table");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HACCEL>::clone
    //! Clone handle
    //! 
    //! \param[in] table - Handle
    //! \return NativeHandle<HACCEL> - Duplicate of handle
    //!
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<HACCEL> clone(NativeHandle<HACCEL> table);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HACCEL>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] table - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<HACCEL> table) noexcept
    {
      // no-op
      return true;
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
} //namespace wtl
#endif // WTL_ACCELERATOR_TRAITS_HPP

