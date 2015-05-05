//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Path.hpp
//! \brief Provides file path handling 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PATH_HPP
#define WTL_PATH_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum FileAttribute - Defines file attributes
  enum class FileAttribute : ulong32
  {
    ReadOnly = 0x00000001,  
    Hidden = 0x00000002,  
    System = 0x00000004,  
    Directory = 0x00000010,  
    Archive = 0x00000020,  
    Device = 0x00000040,  
    Normal = 0x00000080,  
    Temporary = 0x00000100,  
    SparseFile = 0x00000200,  
    ReparsePoint = 0x00000400,  
    Compressed = 0x00000800,  
    Offline = 0x00001000,  
    NotContentIndexed = 0x00002000,  
    Encrypted = 0x00004000,  
    Virtual = 0x00010000, 
    Invalid = INVALID_FILE_ATTRIBUTES,
  };
  
  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<FileAttribute>  : std::true_type  {};
  template <> struct is_contiguous<FileAttribute> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<FileAttribute>     : std::integral_constant<FileAttribute,FileAttribute::ReadOnly>     {};
  template <> struct min_value<FileAttribute>     : std::integral_constant<FileAttribute,FileAttribute::Invalid>      {};

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Path - Provides OS independent handling of file paths
  //! 
  //! \tparam ENCODING - Encoding type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENCODING = Encoding::ANSI>
  struct Path : CharArray<ENCODING,MAX_PATH>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
     
    //! \alias base - Define base type
    using base = CharArray<ENCODING,MAX_PATH>;

    //! \alias char_t - Character type
    using char_t = typename base::char_t;

    //! \alias const_pointer - String type
    using const_pointer = typename base::const_pointer;

    // ------------------------------------ CONSTRUCTION ------------------------------------
 
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::Path
    //! Create an initially empty path
    //////////////////////////////////////////////////////////////////////////////////////////
    Path() 
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::Path
    //! Create from null-terminated string
    //!
    //! \param[in] const* path - Null-terminated string containing absolute path
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing path argument
    //////////////////////////////////////////////////////////////////////////////////////////
    Path(const char_t* path) : base(path)
    {
      REQUIRED_PARAM(path);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::Path
    //! Create from a std::string
    //! 
    //! \param[in] const& path - std::string containing an absolute path
    //////////////////////////////////////////////////////////////////////////////////////////
    Path(const std::string& path) : base(path.c_str())
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::Path
    //! Copy-create from another path
    //! 
    //! \param[in] const& r - Another path
    //////////////////////////////////////////////////////////////////////////////////////////
    Path(const Path& r) : base(r)
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::Path
    //! Move-create from another path
    //! 
    //! \param[in] && r - Another path
    //////////////////////////////////////////////////////////////////////////////////////////
    Path(Path&& r) : base(r)
    {
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::~Path
    //! Virtual d-tor
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Path()
    {}

    // --------------------------------------- STATIC ---------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::combine
    //! Combines two relative or absolute paths, handling trailing backslashes as necessary
    //! 
    //! \param[in] const* a - Relative or absolute path
    //! \param[in] const* b - Relative or absolute path
    //! \return Path - New instance containing combination of both paths
    //! 
    //! \throw wtl::platform_error - Unable to combine paths
    //////////////////////////////////////////////////////////////////////////////////////////
    static Path  combine(const char_t* a, const char_t* b) 
    {
      Path path(a);

      // Combine and return paths
      path.combine(b);
      return path;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::getModulePath
    //! Get absolute path of the executing module
    //! 
    //! \return Path - New instance containing absolute path of the executing module
    //! 
    //! \throw wtl::platform_error - Unable to query module path
    //////////////////////////////////////////////////////////////////////////////////////////
    static Path  getModulePath()
    {
      Path::array_t tmp;     //!< Path storage

      // Get absolute module path
      if (!getFunc<char_t>(::GetModuleFileNameA,::GetModuleFileNameW)(nullptr, tmp, size_of(tmp)))
        throw wtl::platform_error(HERE, "Unable to query module path");

      // Return path
      return tmp;
    }

    // -------------------------------------- ACCESSORS --------------------------------------			
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::attributes const
    //! Query file/folder attributes
    //! 
    //! \return FileAttribute - Combination of file system attributes
    //! 
    //! \throw wtl::platform_error - Unable to query path attributes
    //////////////////////////////////////////////////////////////////////////////////////////
    FileAttribute  attributes() const
    {
      // Query attributes
      auto attr = static_cast<FileAttribute>( getFunc<char_t>(::GetFileAttributesA,::GetFileAttributesW)(Data) );

      // Ensure valid
      if (attr == FileAttribute::Invalid)
        throw wtl::platform_error(HERE, "Unable to query file attributes");

      return attr;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::exists
    //! Query whether path exists
    //! 
    //! \return bool - True iff exists
    //////////////////////////////////////////////////////////////////////////////////////////
    bool  exists() const
    {
      return getFunc<char_t>(::PathFileExistsA,::PathFileExistsW)(Data) != FALSE;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::extension const
    //! Get the file extension, if any
    //! 
    //! \return const char_t* - File Extension if found (with leading dot), otherwise empty string
    //////////////////////////////////////////////////////////////////////////////////////////
    const char_t*  extension() const
    {
      return getFunc<char_t>(::PathFindExtensionA,::PathFindExtensionW)(Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::fileName const
    //! Get the filename, if any
    //! 
    //! \return const char_t* - Filename if found, otherwise whole path
    //////////////////////////////////////////////////////////////////////////////////////////
    const char_t*  fileName() const
    {
      return getFunc<char_t>(::PathFindFileNameA,::PathFindFileNameW)(Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::folder const
    //! Gets the folder portion of the path only
    //! 
    //! \return Path - Absolute folder path (including a trailing backslash)
    //////////////////////////////////////////////////////////////////////////////////////////
    Path  folder() const
    {
      Path tmp(*this);

      // Remove filename, add backslash
      if (!getFunc<char_t>(::PathRemoveFileSpecA,::PathRemoveFileSpecW)(tmp)
       || !getFunc<char_t>(::PathAddBackslashA,::PathAddBackslashW)(tmp))

       throw platform_error(HERE, "Unable to remove path filename");

      // Return copy
      return tmp.c_str();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::resolve const
    //! Full resolves a relative path or symbolic link 
    //! 
    //! \return Path - Fully resolved path
    //!
    //! \throw wtl::domain_error - Path not found or Unable to query properties
    //////////////////////////////////////////////////////////////////////////////////////////
    //Path  resolve() const
    //{
    //  Path resolved;

    //  // Resolve path
    //  if (!GetFullPathName(c_str(), MAX_PATH, resolved.c_arr(), nullptr))
    //    throw wtl::domain_error(HERE, "Unable to resolve path");

    //  // Ensure exists
    //  if (!resolved.exists())
    //    throw wtl::domain_error(HERE, "File not found");

    //  // Get info
    //  SHFILEINFO info;
    //  if (!SHGetFileInfo(resolved.c_arr(), 0, &info, sizeof(info), SHGFI_ATTRIBUTES))
    //    throw wtl::domain_error(HERE, "Unable to retrieve file info");

    //  // Ensure is link
    //  if (info.dwAttributes & SFGAO_LINK)
    //  {
    //    IShellLinkPtr   shell(CLSID_ShellLink);
    //    IPersistFilePtr file(shell);
    //    HRESULT         hr;

    //    // Load file, resolve link, extract resolved path
    //    if (FAILED(hr = file->Load(c_str(), STGM_READ))
    //      || FAILED(hr = shell->Resolve(nullptr, SLR_NO_UI | SLR_ANY_MATCH))
    //      || FAILED(hr = shell->GetPath((wchar_t*)resolved, MAX_PATH, nullptr, 0)))
    //      throw wtl::domain_error(HERE, "Unable to resolve shell link");
    //  }

    //  // Return resolved path
    //  return resolved;
    //}

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::hasExtension const
    //! Query whether path has a given extension (case insensitive)
    //! 
    //! \param[in] ext - File extention including the leading dot
    //! \return bool - True iff exists
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing file extension
    //////////////////////////////////////////////////////////////////////////////////////////
    bool  hasExtension(const char_t* ext) const
    {
      REQUIRED_PARAM(ext);

      // Compare extension, case insenstive
      return getFunc<char_t>(::StrCmpIA,::StrCmpIW)(extension(), ext) == 0;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::isFolder const
    //! Query whether path points to a folder
    //! 
    //! \return bool - True iff path is folder
    //! 
    //! \throw wtl::domain_error - Unable to query path attributes
    //////////////////////////////////////////////////////////////////////////////////////////
    bool  isFolder() const
    {
      // Query folder flag
      return (attributes & FileAttribute::Directory) != default_t<FileAttribute>();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Path::operator == const
    //! Performs a case insensitive lexiographic comparison against a string
    //! 
    //! \param[in] const *ptr - Null-terminated string
    //! \return bool - True iff equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator== (const char_t* ptr) const
    {
      // Compare full path, case insenstive
      return getFunc<char_t>(::StrCmpIA,::StrCmpIW)(Data, ptr) == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Path::operator == const
    //! Performs a case insensitive lexiographic comparison against another path
    //! 
    //! \param[in] const& p - Another path
    //! \return bool - True iff equal
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator== (const Path& p) const
    {
      // Compare full path, case insenstive
      return getFunc<char_t>(::StrCmpIA,::StrCmpIW)(Data, p.c_str()) == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Path::operator+ const
    //! Creates an independant combination of two paths
    //! 
    //! \param[in] const& p - Relative or absolute path
    //! \return Path - New instance combining both paths
    /////////////////////////////////////////////////////////////////////////////////////////
    Path operator+ (const Path& p) const
    {
      return Path::combine(p);
    }
    
    // --------------------------------------- MUTATORS --------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::append 
    //! Appends an absolute path, including a separating backslash if necessary
    //! 
    //! \param[in] const* path - Null-terminated absolute path
    //! 
    //! \throw wtl::platform_error - Unable to append path
    //////////////////////////////////////////////////////////////////////////////////////////
    void append(const char_t* path) 
    {
      // Append path to self
      if (!getFunc<char_t>(::PathAppendA,::PathAppendW)(Data, path))
        throw wtl::platform_error(HERE, "Unable to append path");

      // Update length
      Count = strlen(Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::appendBackslash 
    //! Appends a trailing backslash if not already present
    //! 
    //! \throw wtl::platform_error - Unable to append path
    //////////////////////////////////////////////////////////////////////////////////////////
    void appendBackslash() 
    {
      // Append backslash and update length
      if (const_pointer chr = getFunc<char_t>(::PathAddBackslashA,::PathAddBackslashW)(Data))
        Count += (chr[0] != null_t);
      else
        throw wtl::platform_error(HERE, "Insufficient buffer to append backslash to path");
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::combine 
    //! Combines with an absolute or relative path, adding and/or removing intermediate paths where necessary
    //! 
    //! \param[in] const* path - Null-terminated absolute or relative path
    //! 
    //! \throw wtl::platform_error - Unable to combine paths
    //////////////////////////////////////////////////////////////////////////////////////////
    void combine(const char_t* path) 
    {
      // Append path to self
      if (!getFunc<char_t>(::PathCombineA,::PathCombineW)(Data, path))
        throw wtl::platform_error(HERE, "Unable to combine path");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::removeBackslash 
    //! Removes the trailing backslash, if any
    //////////////////////////////////////////////////////////////////////////////////////////
    void removeBackslash() 
    {
      // Remove backslash and update length
      const_pointer chr = getFunc<char_t>(::PathRemoveBackslashA,::PathRemoveBackslashW)(Data);
      Count -= (chr[0] != null_t);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::removeExtension 
    //! Removes the file extension, if any
    //////////////////////////////////////////////////////////////////////////////////////////
    void  removeExtension() 
    {
      // Remove extension + update length
      getFunc<char_t>(::PathRemoveExtensionA,::PathRemoveExtensionW)(Data);
      Count = strlen(Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::removeFileName
    //! Removes the filename, if any
    //////////////////////////////////////////////////////////////////////////////////////////
    void  removeFileName()
    {
      // Remove filename + update length
      if (const_pointer fn = getFunc<char_t>(::PathFindFileNameA,::PathFindFileNameW)(tmp))
      {
        fn[0] = null_t;
        Count = (fn - Data);
      }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::renameExtension 
    //! Renames the file extension (if none is present then it is appended)
    //! 
    //! \param[in] ext - New file extension including the dot
    //! 
    //! \throw wtl::platform_error - Unable to rename extension
    //////////////////////////////////////////////////////////////////////////////////////////
    void  renameExtension(const char_t* ext) 
    {
      // Rename extension 
      if (!getFunc<char_t>(::PathRenameExtensionA,::PathRenameExtensionW)(Data, ext))
        throw wtl::platform_error(HERE, "Unable to rename extension");

      // Update length
      Count = strlen(Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::renameFileName 
    //! Changes the filename only
    //! 
    //! \param[in] name - Null-terminated filename
    //! 
    //! \throw wtl::platform_error - Unable to change filename
    //////////////////////////////////////////////////////////////////////////////////////////
    void  renameFileName(const char_t* name) const
    {
      // Find filename
      if (const_pointer fn = getFunc<char_t>(::PathFindFileNameA,::PathFindFileNameW)(Data))
      {
        // Verify new length
        int32 remaining = (length-1) - (fn-Data);
        if (strlen(name) > remaining)
          throw wtl::platform_error(HERE, "Insufficient buffer to change filename");

        // Erase filename & replace
        fn[0] = null_t;
        base::append(name);
      }
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::operator=
    //! Overwrite contents with that of another path
    //! 
    //! \param[in] const& r - Another path
    //! \return Path& - Reference to self, containing new path
    //////////////////////////////////////////////////////////////////////////////////////////
    Path& operator=(const Path& r)
    {
      CharArray::assign(r.c_str());
      return *this;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Path::operator +=
    //! Appends a relative or absolute path
    //! 
    //! \param[in] const& p - Relative or absolute path
    //! \return Path& - Reference to self
    /////////////////////////////////////////////////////////////////////////////////////////
    Path& operator += (const Path& p) 
    {
      combine(p);
      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct AppPath - Represents the path of a file/folder in the application folder
  //! 
  //! \tparam ENCODING - Encoding type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENCODING = Encoding::ANSI>
  struct AppPath : Path<ENCODING>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias base - Define base type
    using base = Path<ENCODING>;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    //! AppPath::AppPath
    //! Populates with absolute folder path of executing module
    //! 
    //! \throw wtl::platform_error - Unable to query module path
    //////////////////////////////////////////////////////////////////////////////////////////
    AppPath() : base(Path::getModulePath())
    {
      // Remove filename
      removeFileName();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! AppPath::AppPath
    //! Create absolute path within executing module folder
    //! 
    //! \param[in] const* path - Absolute or relative path to combine 
    //! 
    //! \throw wtl::platform_error - Unable to query module path
    //////////////////////////////////////////////////////////////////////////////////////////
    AppPath(const char_t* path) : AppPath()
    {
      combine(path);
    }

    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			

    // --------------------------------------- MUTATORS --------------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  };


  


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct TempPath - Represents the path of a temporary file
  //! 
  //! \tparam ENCODING - Encoding type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENCODING = Encoding::ANSI>
  struct TempPath : Path<ENCODING>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = Path<ENCODING>;
    
    // ------------------------------------ CONSTRUCTION ------------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    //! TempPath::TempPath
    //! Create temporary file path
    //! 
    //! \param[in] prefix - three letter filename prefix
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing filename prefix
    //! \throw wtl::platform_error - Unable to retrieve temporary folder
    //////////////////////////////////////////////////////////////////////////////////////////
    TempPath(const char_t* prefix = "tmp")
    {
      REQUIRED_PARAM(prefix);

      Path<ENCODING>::array_t tmp;     //!< Absolute path of user temp folder

      // Get temp folder
      if (!getFunc<char_t>(GetTempPathA,GetTempPathW)(MAX_PATH, tmp))
        throw wtl::platform_error(HERE, "Unable to get temp folder");

      // Combine with random filename   (TODO: See L_tmpnam constant and tmpnam() func)
      if (!getFunc<char_t>(GetTempFileNameA,GetTempFileNameW)(tmp, prefix, NULL, Data))
        throw wtl::platform_error(HERE, "Unable to generate temporary filename");

      // Update length
      Count = strlen(Data);
    }
    
    // --------------------------------------- STATIC ---------------------------------------

    // -------------------------------------- ACCESSORS --------------------------------------			

    // --------------------------------------- MUTATORS --------------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  };


} //namespace wtl
#endif // WTL_PATH_HPP

