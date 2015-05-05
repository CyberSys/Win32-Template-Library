////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Enumeration.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FILE_SEARCH_TRAITS_HPP
#define WTL_FILE_SEARCH_TRAITS_HPP

#include "wtl/WTL.hpp"

// Create distinct file handle
DECLARE_HANDLE(HFILESEARCH);

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<HFILESEARCH> - Encapsulates creating file system search handles
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HFILESEARCH>
  {
    //! \var npos - Invalid handle sentinel value
    static const HFILESEARCH npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFILESEARCH>::create
    //! Create file search handle
    //! 
    //! \tparam CHR - Character type
    //! 
    //! \param[in] const& query - Search query. Wildcards are permitted.
    //! \param[in,out] &results - Results storage, populated with first result if successful
    //! \return HFILESEARCH - File search handle
    ///////////////////////////////////////////////////////////////////////////////
    template <typename CHR>
    static HFILESEARCH create(const CHR* folder, getType<CHR,WIN32_FIND_DATAA,WIN32_FIND_DATAW>& results) 
    { 
      return reinterpret_cast<HFILESEARCH>( getFunc<CHR>(::FindFirstFileA,::FindFirstFileW)(folder.c_str(), &results) ); 
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFILESEARCH>::clone
    //! Clone handle
    //! 
    //! \param[in] search - Handle
    //! \return HFILESEARCH - Duplicate of handle
    ///////////////////////////////////////////////////////////////////////////////
    static HFILESEARCH  clone(HFILESEARCH search);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HFILESEARCH>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] search - Handle
    //! \return bool - True iff closed successfully
    //!
    //! \throw wtl::invalid_argument - [Debug only] Handle is invalid
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HFILESEARCH search) noexcept
    {
      //PARAM_INVARIANT(search, search != npos);

      // Close search query
      return ::FindClose(search) != FALSE;
    }
  };


} //namespace wtl
#endif // WTL_FILE_SEARCH_TRAITS_HPP

