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
    static constexpr ::HACCEL npos = default<::HACCEL>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
	
    // -------------------------------- COPY & MOVE SEMANTICS -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HACCEL>::create
    //! Load accelerator table handle
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] instance - Instance containing cursor
    //! \param[in] ident - Accelerator identifier
    //! \return HAlloc<HACCEL> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<HACCEL> create(HINSTANCE instance, ResourceId<ENC> ident) 
    { 
      //! \alias char_t - Character encoding type
      using char_t = encoding_char_t<ENC>;    

      // Create solid colour brush handle
      if (HACCEL accel = getFunc<char_t>(::LoadAcceleratorsA,::LoadAcceleratorsW)(instance, ident))
        return { accel, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load accelerator table");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HACCEL>::clone
    //! Clone handle
    //! 
    //! \param[in] table - Handle
    //! \return HAlloc<HACCEL> - Duplicate of handle
    //!
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<HACCEL> clone(HAlloc<HACCEL> table);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HACCEL>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] table - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<HACCEL> table) noexcept
    {
      // no-op
      return true;
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
} //namespace wtl
#endif // WTL_ACCELERATOR_TRAITS_HPP

