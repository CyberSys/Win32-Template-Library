//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\FileSearch.hpp
//! \brief Provides a file system search query iterator
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_FILE_SEARCH_HPP
#define WTL_FILE_SEARCH_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/EnumTraits.hpp"
#include "wtl/traits/EncodingTraits.hpp"
#include "wtl/traits/FileSearchTraits.hpp"
#include "wtl/utils/Default.hpp"
#include "wtl/utils/Handle.hpp"
#include "wtl/platform/Path.hpp"
  
//! \namespace wtl - Windows template library
namespace wtl
{

  ////////////////////////////////////////////////////////////////////////////////////////// 
  //! \struct FileSearch - Provides a file system search query iterator
  //! 
  //! \tparam ENC - Defines path character encoding 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct FileSearch
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias encoding - Encoding type
    static constexpr Encoding encoding = ENC;  
    
    //! \alias type - Define own type
    using type = FileSearch<encoding>;
  
    //! \alias char_t - Encoding character type
    using char_t = encoding_char_t<encoding>;

    //! \alias path_t - Path type
    using path_t = Path<encoding>;

    //! \alias result_t - Results data type
    using result_t = choose_t<encoding,::WIN32_FIND_DATAA,::WIN32_FIND_DATAW>;

    //! \alias handle_t - Search handle type
    using handle_t = Handle<::HFILESEARCH>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    result_t   Result;      //!< Storage for current result
    handle_t   Handle;      //!< Search handle
    path_t     Folder;      //!< Absolute path of target folder (with trailing backslash)

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::FileSearch
    //! Create and execute a file system query
    //!
    //! \param[in] const& folder - Target folder
    //! \param[in] const& query - Search query  (Wildcards allowed)
    //! 
    //! \throw wtl::logic_error - Folder does not exist
    //////////////////////////////////////////////////////////////////////////////////////////
    FileSearch(const path_t& folder, const path_t& query) : Handle((folder+query).c_str(), Result),
                                                            Folder(folder)
    {
      // Verify folder exists
      if (!folder.exists())
        throw logic_error(HERE, "Search folder '", folder, "' does not exist");

      // Skip undesireable first result
      if (exists() && !valid())
        next();
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(FileSearch);      //!< Cannot be copied
    ENABLE_MOVE(FileSearch);       //!< Can be moved
    ENABLE_POLY(FileSearch);       //!< Can be polymorphic  [Closes the query handle]

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::exists const
    //! Query whether current result exists
    //!
    //! \return bool - True if valid, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return Handle.exists();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::isFolder const
    //! Query whether current item is a folder
    //!
    //! \return bool - True for folders, False for files
    //!
    //! \throw wtl::logic_error - [Debug Only] Current result is invalid
    //////////////////////////////////////////////////////////////////////////////////////////
    bool isFolder() const
    {
      LOGIC_INVARIANT(exists());

      // Query current result attributes
      return (FileAttribute::Directory & Result.dwFileAttributes) != defvalue<FileAttribute>();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::fileName const
    //! Get filename of current item
    //!
    //! \return path_t - Filename of current item
    //!
    //! \throw wtl::logic_error - [Debug Only] Current result is invalid
    //////////////////////////////////////////////////////////////////////////////////////////
    path_t fileName() const
    {
      LOGIC_INVARIANT(exists());

      // Return current result filename
      return Result.cFileName;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::fullPath const
    //! Get full path of current item
    //!
    //! \return path_t - Full path of current item
    //! 
    //! \throw wtl::logic_error - [Debug Only] Current result is invalid
    //////////////////////////////////////////////////////////////////////////////////////////
    path_t fullPath() const
    {
      LOGIC_INVARIANT(exists());

      // Prepend folder to current result filename
      return Folder + Result.cFileName;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::operator bool() const
    //! Query whether current result exists
    //!
    //! \return bool - True if valid, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    operator bool() const
    {
      return Handle.exists();
    }

  protected:
    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::valid const
    //! Query whether current result is desired
    //!
    //! \return bool - True if valid, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    bool valid() const
    {
      return strcmp(Result.cFileName, ".") && strcmp(Result.cFileName, "..");
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::next
    //! Advances to the next result
    //!
    //! \return bool - True if valid, otherwise False
    //! 
    //! \throw wtl::logic_error - [Debug Only] Current position is invalid
    //////////////////////////////////////////////////////////////////////////////////////////
    bool next()
    {
      LOGIC_INVARIANT(exists());

      do
      {
        // Advance iff results remain, otherwise close handle
        if (!choose<encoding>(::FindNextFileA,::FindNextFileW)(Handle, Result))
          Handle.release();
      } 
      // Skip relative paths, abort if complete
      while (exists() && !valid());

      // Query state
      return exists();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::operator++ 
    //! Advances to the next result
    //!
    //! \return FileSearch& - Reference to next result
    //! 
    //! \throw wtl::logic_error - [Debug Only] Current position is invalid
    //////////////////////////////////////////////////////////////////////////////////////////
    FileSearch& operator++()
    {
      next();
      return *this;
    }
  };


}  // namespace wtl
#endif // WTL_FILE_SEARCH_HPP








