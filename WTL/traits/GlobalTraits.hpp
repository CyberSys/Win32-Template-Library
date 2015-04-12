////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\GlobalTraits.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_GLOBAL_MEM_TRAITS_HPP
#define WTL_GLOBAL_MEM_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<HGLOBAL> - Encapsulates allocating global memory handles
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<HGLOBAL>
  {
    //! \var npos - Invalid handle sentinel value
    static const HGLOBAL npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HGLOBAL>::create
    //! Create global memory handle to resource
    //! 
    //! \param[in] module - Module containing resource
    //! \param[in] resource - Resource handle
    ///////////////////////////////////////////////////////////////////////////////
    static HGLOBAL create(HMODULE module, HResource resource) 
    { 
      return ::LoadResource(module, resource.get());
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HGLOBAL>::clone
    //! Clone handle
    //! 
    //! \param[in] addr - Handle
    //! \return HGLOBAL - Duplicate of handle
    ///////////////////////////////////////////////////////////////////////////////
    static HGLOBAL clone(HGLOBAL addr);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HGLOBAL>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] addr - Handle
    //! \return bool - True iff closed successfully
    //!
    //! \throw wtl::invalid_argument - [Debug only] Handle is invalid
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HGLOBAL addr) noexcept
    {
      
    }
  };

  
  //! \alias HGlobal - Global memory handle
  using HGlobal = Handle<HGLOBAL>;

} //namespace wtl
#endif // WTL_GLOBAL_MEM_TRAITS_HPP

