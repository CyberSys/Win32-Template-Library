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
#include "wtl/traits/EnumTraits.hpp"              //!< is_attribute, is_contiguous
#include "wtl/traits/EncodingTraits.hpp"          //!< Encoding
#include "wtl/utils/CharArray.hpp"                //!< CharArray
#include "wtl/utils/Default.hpp"                  //!< default_t
//#include "wtl/io/Console.hpp"                   //!< Console
#include <string>

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum FileAttribute - Defines file attributes
  enum class FileAttribute : ulong32_t
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
  template <> struct default_t<FileAttribute>     : std::integral_constant<FileAttribute,FileAttribute::ReadOnly>   {};


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Path - Provides OS independent handling of file paths
  //!
  //! \tparam ENC - Encoding type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::ANSI>
  struct Path : CharArray<ENC,MAX_PATH>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = Path<ENC>;

    //! \alias base - Define base type
    using base = CharArray<ENC,MAX_PATH>;

    //! \alias char_t - Inherit character type
    using char_t = typename base::char_t;

    //! \alias const_pointer - Define null-terminated string type
    using const_pointer = typename base::const_pointer;
    
    //! \var encoding - Inherit window character encoding
    static constexpr Encoding encoding = base::encoding;

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

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY_CTOR(Path);      //!< Can be deep copied
    ENABLE_MOVE_CTOR(Path);      //!< Can be moved
    ENABLE_POLY(Path);           //!< Can be polymorphic

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::operator=
    //! Overwrite contents with that of another path
    //!
    //! \param[in] const& r - Another path
    //! \return Path& - Reference to self, containing new path
    //////////////////////////////////////////////////////////////////////////////////////////
    Path& operator=(const Path& r)
    {
      base::assign(r.c_str());
      return *this;
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

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
      typename base::array_t tmp;     //!< Path storage

      // Get absolute module path
      if (!choose<encoding>(::GetModuleFileNameA,::GetModuleFileNameW)(nullptr, tmp, size_of(tmp)))
        throw wtl::platform_error(HERE, "Unable to query module path");

      // Return path
      return tmp;
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

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
      auto attr = static_cast<FileAttribute>( choose<encoding>(::GetFileAttributesA,::GetFileAttributesW)(this->Data) );

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
      return choose<encoding>(::PathFileExistsA,::PathFileExistsW)(this->Data) != False;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::extension const
    //! Get the file extension, if any
    //!
    //! \return const char_t* - File Extension if found (with leading dot), otherwise empty string
    //////////////////////////////////////////////////////////////////////////////////////////
    const char_t*  extension() const
    {
      return choose<encoding>(::PathFindExtensionA,::PathFindExtensionW)(this->Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::fileName const
    //! Get the filename, if any
    //!
    //! \return const char_t* - Filename if found, otherwise whole path
    //////////////////////////////////////////////////////////////////////////////////////////
    const char_t*  fileName() const
    {
      return choose<encoding>(::PathFindFileNameA,::PathFindFileNameW)(this->Data);
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
      if (!choose<encoding>(::PathRemoveFileSpecA,::PathRemoveFileSpecW)(tmp)
       || !choose<encoding>(::PathAddBackslashA,::PathAddBackslashW)(tmp))

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
      return choose<encoding>(::StrCmpIA,::StrCmpIW)(extension(), ext) == 0;
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
      return (attributes() & FileAttribute::Directory) != defvalue<FileAttribute>();
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
      return choose<encoding>(::StrCmpIA,::StrCmpIW)(this->Data, ptr) == 0;
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
      return choose<encoding>(::StrCmpIA,::StrCmpIW)(this->Data, p.c_str()) == 0;
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

    // ----------------------------------- MUTATOR METHODS ----------------------------------

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
      if (!choose<encoding>(::PathAppendA,::PathAppendW)(this->Data, path))
        throw wtl::platform_error(HERE, "Unable to append path");

      // Update length
      this->Count = strlen(this->Data);
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
      if (const_pointer chr = choose<encoding>(::PathAddBackslashA,::PathAddBackslashW)(this->Data))
        this->Count += (chr[0] != this->null_t);
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
      if (!choose<encoding>(::PathCombineA,::PathCombineW)(this->Data, path))
        throw wtl::platform_error(HERE, "Unable to combine path");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::removeBackslash
    //! Removes the trailing backslash, if any
    //////////////////////////////////////////////////////////////////////////////////////////
    void removeBackslash()
    {
      // Remove backslash and update length
      const_pointer chr = choose<encoding>(::PathRemoveBackslashA,::PathRemoveBackslashW)(this->Data);
      this->Count -= (chr[0] != this->null_t);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::removeExtension
    //! Removes the file extension, if any
    //////////////////////////////////////////////////////////////////////////////////////////
    void  removeExtension()
    {
      // Remove extension + update length
      choose<encoding>(::PathRemoveExtensionA,::PathRemoveExtensionW)(this->Data);
      this->Count = strlen(this->Data);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! Path::removeFileName
    //! Removes the filename, if any
    //////////////////////////////////////////////////////////////////////////////////////////
    void  removeFileName()
    {
      // Remove filename + update length
      if (const_pointer fn = choose<encoding>(::PathFindFileNameA,::PathFindFileNameW)(this->Data))
      {
        fn[0] = this->null_t;
        this->Count = (fn - this->Data);
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
      if (!choose<encoding>(::PathRenameExtensionA,::PathRenameExtensionW)(this->Data, ext))
        throw wtl::platform_error(HERE, "Unable to rename extension");

      // Update length
      this->Count = strlen(this->Data);
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
      if (const_pointer fn = choose<encoding>(::PathFindFileNameA,::PathFindFileNameW)(this->Data))
      {
        // Verify new length
        int32_t remaining = (this->length-1) - (fn-this->Data);
        if (strlen(name) > remaining)
          throw wtl::platform_error(HERE, "Insufficient buffer to change filename");

        // Erase filename & replace
        fn[0] = this->null_t;
        base::append(name);
      }
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
  // wtl::operator<<
  //! Writes a path to the debug console in yellow, then resets the output colour
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] const& path - Path
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <Encoding ENC>
  inline Console& operator<< (Console& c, const Path<ENC>& path)
  {
    return c << Cons::Yellow << path.c_str() << Cons::Reset;
  }*/


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct AppPath - Represents the path of a file/folder in the application folder
  //!
  //! \tparam ENC - Encoding type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::ANSI>
  struct AppPath : Path<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = AppPath<ENC>;

    //! \alias base - Define base type
    using base = Path<ENC>;
    
    //! \var encoding - Inherit window character encoding
    static constexpr Encoding encoding = base::encoding;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    //! AppPath::AppPath
    //! Populates with absolute folder path of executing module
    //!
    //! \throw wtl::platform_error - Unable to query module path
    //////////////////////////////////////////////////////////////////////////////////////////
    AppPath() : base(base::getModulePath())
    {
      // Remove filename
      this->removeFileName();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    //! AppPath::AppPath
    //! Create absolute path within executing module folder
    //!
    //! \param[in] const* path - Absolute or relative path to combine
    //!
    //! \throw wtl::platform_error - Unable to query module path
    //////////////////////////////////////////////////////////////////////////////////////////
    AppPath(const typename base::char_t* path) : AppPath()
    {
      this->combine(path);
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  };





  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct TempPath - Represents the path of a temporary file
  //!
  //! \tparam ENC - Encoding type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::ANSI>
  struct TempPath : Path<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = TempPath<ENC>;

    //! \alias base - Define base type
    using base = Path<ENC>;
    
    //! \var encoding - Inherit window character encoding
    static constexpr Encoding encoding = base::encoding;

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
    TempPath(const typename base::char_t* prefix = "tmp")
    {
      REQUIRED_PARAM(prefix);

      typename base::array_t tmp;     //!< Absolute path of user temp folder

      // Get temp folder
      if (!choose<encoding>(::GetTempPathA,::GetTempPathW)(MAX_PATH, tmp))
        throw wtl::platform_error(HERE, "Unable to get temp folder");

      // Combine with random filename   (TODO: See L_tmpnam constant and tmpnam() func)
      if (!choose<encoding>(::GetTempFileNameA,::GetTempFileNameW)(tmp, prefix, NULL, this->Data))
        throw wtl::platform_error(HERE, "Unable to generate temporary filename");

      // Update length
      this->Count = strlen(this->Data);
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  };


} //namespace wtl
#endif // WTL_PATH_HPP

