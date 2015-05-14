//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Enumeration.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FILE_SEARCH_TRAITS_HPP
#define WTL_FILE_SEARCH_TRAITS_HPP

#include "wtl/WTL.hpp"

// Create distinct file handle
DECLARE_HANDLE(HFILESEARCH);

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HFileSearch - Shared file-system search handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HFileSearch = Handle<::HFILESEARCH>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HFILESEARCH> - Encapsulates creating file-system search handles
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HFILESEARCH>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HFILESEARCH npos = (::HFILESEARCH)INVALID_HANDLE_VALUE;    //default<::HFILESEARCH>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFILESEARCH>::create
    //! Create file search handle
    //! 
    //! \tparam ENC - Character encoding
    //! 
    //! \param[in] const& query - Search query. Wildcards are permitted.
    //! \param[in,out] &results - Results storage, populated with first result if successful
    //! \return ::HFILESEARCH - File search handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static ::HFILESEARCH create(const encoding_char_t<ENC>* folder, getType_t<ENC,::WIN32_FIND_DATAA,::WIN32_FIND_DATAW>& results) 
    { 
      return reinterpret_cast<::HFILESEARCH>( getFunc<ENC>(::FindFirstFileA,::FindFirstFileW)(folder.c_str(), &results) ); 
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFILESEARCH>::clone
    //! Clone handle
    //! 
    //! \param[in] search - Handle
    //! \return ::HFILESEARCH - Duplicate of handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static ::HFILESEARCH  clone(::HFILESEARCH search);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HFILESEARCH>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] search - Handle
    //! \return bool - True iff closed successfully
    //!
    //! \throw wtl::invalid_argument - [Debug only] Handle is invalid
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(::HFILESEARCH search) noexcept
    {
      //PARAM_INVARIANT(search, search != npos);

      // Close search query
      return ::FindClose(search) != False;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_FILE_SEARCH_TRAITS_HPP

