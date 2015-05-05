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
#include "wtl/traits/FileSearchTraits.hpp"
  
//! \namespace wtl - Windows template library
namespace wtl
{

  ////////////////////////////////////////////////////////////////////////////////////////// 
  //! \struct FileSearch - Provides a file system search query iterator
  //! 
  //! \tparam ENCODING - Defines path character encoding 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENCODING>
  struct FileSearch
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias encoding - Encoding type
    static constexpr Encoding encoding = ENCODING;  

    //! \alias char_t - Encoding character type
    using char_t = encoding_char_t<encoding>;

    //! \alias path_t - Path type
    using path_t = Path<encoding>;

    //! \alias result_t - Results data type
    using result_t = getType<char_t,::WIN32_FIND_DATAA,::WIN32_FIND_DATAW>;

    //! \alias handle_t - Search handle type
    using handle_t = Handle<::HFILESEARCH>;

    // ------------------------------------ CONSTRUCTION ------------------------------------
  
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
        throw wtl::logic_error(HERE, "Search folder '%s' does not exist", folder.c_str());

      // Skip undesireable first result
      if (exists() && !valid())
        next();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // FileSearch::~FileSearch
    //! Closes the query handle
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~FileSearch()
    {
    }

    // --------------------------------------- STATIC ---------------------------------------
  

    // -------------------------------------- ACCESSORS --------------------------------------			
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

      return (FileAttribute::Directory & Result.dwFileAttributes) != zero<FileAttribute>::value;
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

    // --------------------------------------- MUTATORS --------------------------------------
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
        if (!getFunc<char_t>(::FindNextFileA,::FindNextFileW)(Handle, Result))
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

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    result_t   Result;      //!< Storage for current result
    handle_t   Handle;      //!< Search handle
    path_t     Folder;      //!< Absolute path of target folder (with trailing backslash)
  };


}  // namespace wtl
#endif // WTL_FILE_SEARCH_HPP








