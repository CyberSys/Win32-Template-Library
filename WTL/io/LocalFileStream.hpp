//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\LocalFileStream.hpp
//! \brief Encapsulates file I/O for local files on disc
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LOCAL_FILE_STREAM_HPP
#define WTL_LOCAL_FILE_STREAM_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LocalFileStream - Encapsulates a handle to a local file on disc.
  //!
  //! This is used for for debug output streams whose object lifetimes must match 
  //! the module lifetime rather than an instance lifetime.
  //! 
  //! \tparam ELEMENT - Stream element type (Wide character types are supported)
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ELEMENT>
  struct LocalFileStream 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef distance_t - Stream distance type
    typedef ulong32  distance_t;
    
    //! \typedef element_t - Stream element type
    typedef ELEMENT  element_t;
    
    //! \typedef position_t - Stream position type
    typedef ulong32  position_t;

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::LocalFileStream
    //! Cannot default-construct file stream
    //////////////////////////////////////////////////////////////////////////////////////////
    LocalFileStream() = delete;

    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::LocalFileStream
    //! Create file stream from a local file
    //! 
    //! \tparam ENC - Path character encoding
    //! 
    //! \param[in] const& path - Full file path 
    //! \param[in] access - File access
    //!
    //! \throw wtl::domain_error - [Debug only] Unable to open file
    //////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    LocalFileStream(const Path<ENC>& path, FileAccess access) : Handle(openFile(path, access)), Access(access)
    {
      if (!Handle)
        throw wtl::domain_error(HERE, "Unable to open file handle");
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::LocalFileStream
    //! Cannot be duplicated
    //! 
    //! \param[in] const& - Ignored
    //////////////////////////////////////////////////////////////////////////////////////////
    LocalFileStream(const LocalFileStream&) = delete;
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::LocalFileStream
    //! Move-create file stream an rvalue, taking ownership of the handle from the input object
    //! 
    //! \param[in] &&r - Source stream, on return the handle is nullptr
    //////////////////////////////////////////////////////////////////////////////////////////
    LocalFileStream(LocalFileStream&& r) : Access(r.Access), Handle(r.Handle)
    {
      // Take ownership
      r.Handle = nullptr;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::~LocalFileStream
    //! Can be polymorphic, closes the file handle
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~LocalFileStream() 
    {
      // Close iff the owner
      if (Handle)
      {
        flush();
        close();
      }
    }
	  
    // Shallow copy semantics
    //ENABLE_COPY(LocalFileStream);
    //ENABLE_COPY_ASSIGN(LocalFileStream);

	  // ----------------------------------- STATIC METHODS -----------------------------------
  protected:
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::LocalFileStream
    //! Create file stream from a local file
    //! 
    //! \tparam ENC - Path character encoding
    //! 
    //! \param[in] const& path - Full file path 
    //! \param[in] access - File access
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Invalid/Missing argument
    //////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static FILE* openFile(const Path<ENC>& path, FileAccess access)
    {
      switch (access)
      {
      case FileAccess::Create:       return fopen(path.c_str(), "wb");
      case FileAccess::ReadOnly:     return fopen(path.c_str(), "rb");
      case FileAccess::OpenExisting: return fopen(path.c_str(), "ab");
      }

      throw wtl::invalid_argument(HERE, "Unrecognised file access type");
    }
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::length const
    //! Get the current file length
    //! 
    //! \return distance_t - File length (in elements) iff stream is open, otherwise zero
    //!
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t length() const
    {
      LOGIC_INVARIANT(Handle != nullptr);

      if (Handle)
      {
        position_t initial = position();   //!< original position

        // Determine file length
        const_cast<LocalFileStream&>(*this).seek(0, FileSeek::End);
        position_t len = position();

        // Restore position + return length
        const_cast<LocalFileStream&>(*this).seek(initial, FileSeek::Begin);
        return len;
      }
      
      // Error
      return default<distance_t>();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::position() const
    //! Get current position within the stream 
    //! 
    //! \return position_t - Zero-based position (in elements: 0 <= n <= length) iff stream is open, otherwise zero
    //! 
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    position_t position() const
    {
      LOGIC_INVARIANT(Handle != nullptr);
      
      // Query position, convert into elements
      return Handle ? static_cast<position_t>(ftell(Handle)) / sizeof(element_t) : default<position_t>();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::remaining() const
    //! Get the number of available elements remaining for input streams.
    //! For output streams this method always returns the maximum allowable distance_t value.
    //! 
    //! \return distance_t - Number of elements remaining for input streams, infinity for output streams
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t remaining() const
    {
      LOGIC_INVARIANT(Handle != nullptr);

      // Return remaining iff read-only, otherwise max allowable value
      return Access == FileAccess::ReadOnly ? length() - position() : std::numeric_limits<distance_t>::max();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::used() const
    //! Get the number of elements already written to the stream 
    //! 
    //! \return distance_t - Position of file pointer, in elements
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t used() const
    {
      LOGIC_INVARIANT(Handle != nullptr);

      return position();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::operator ::FILE* const
    //! Implicit user conversion to C file handle
    //! 
    //! \return ::FILE* - Mutable pointer to C file handle iff stream is open, otherwise nullptr
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    operator ::FILE*() const
    {
      LOGIC_INVARIANT(Handle != nullptr);

      return Handle;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::operator=
    //! Cannot be assigned once created
    //! 
    //! \param[in] &&r - Ignored
    //////////////////////////////////////////////////////////////////////////////////////////
    LocalFileStream& operator= (const LocalFileStream& r) = delete;
    LocalFileStream& operator= (LocalFileStream&& r) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::close
    //! Informs the stream the file handle has been closed.
    //! Responsibility for closing the handle lies with the FileSystem service
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    void close() 
    {
      LOGIC_INVARIANT(Handle != nullptr);

      // Clear handle
      fclose(Handle);
      Handle = nullptr;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::flush
    //! Flush any unwritten data in the stream to disc
    //! 
    //! \return int32 - Undocumented
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    int32 flush() 
    {
      LOGIC_INVARIANT(Handle != nullptr);

      return Handle ? fflush(Handle) : 0UL;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::read
    //! Read from the file stream into a statically allocated buffer. This provides no performance benefit
    //! over using a dynamic buffer.
    //! 
    //! \tparam LENGTH - Output buffer length, in elements
    //!
    //! \param[in,out] (&)[] buffer - Mutable reference to statically allocated output buffer
    //! \return distance_t - Number of elements read iff stream is open, otherwise zero
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    distance_t  read(std::remove_const_t<element_t> (&buffer)[LENGTH])
    {
      LOGIC_INVARIANT(Handle != nullptr);

      return read(buffer, LENGTH);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::read
    //! Read from the file stream into an output buffer of dynamic length
    //! 
    //! \param[in,out] *buffer - Mutable output buffer pointer
    //! \param[in] length - Buffer length, in elements
    //! \return distance_t - Number of elements read iff stream is open, otherwise zero
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t  read(std::remove_const_t<element_t>* buffer, distance_t length)
    {
      LOGIC_INVARIANT(Handle != nullptr);

      // Read data in elements
      return Handle ? fread(buffer, sizeof(element_t), length, Handle) : default<distance_t>();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::seek() 
    //! Provides random access to the file stream
    //! 
    //! \param[in] offset - Distance to move, in elements
    //! \param[in] origin - Seek origin: Start/Current/End
    //! \return int32 - Zero upon success, non-zero upon failure
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    int32 seek(int32 offset, FileSeek origin) 
    {
      LOGIC_INVARIANT(Handle != nullptr);

      switch (origin)
      {
      // Begin/Current/End:
      case FileSeek::Begin:   return fseek(Handle, offset*sizeof(element_t), SEEK_SET);
      case FileSeek::Current: return fseek(Handle, offset*sizeof(element_t), SEEK_CUR);
      case FileSeek::End:     return fseek(Handle, offset*sizeof(element_t), SEEK_END);
      }
      
      return 1;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::write
    //! Write a character to the file stream 
    //! 
    //! \param[in] chr - Character
    //! \return distance_t - Number of elements written iff stream is open, otherwise zero
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t  write(element_t chr)
    {
      LOGIC_INVARIANT(Handle != nullptr);

      return fputc(chr, Handle);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::write
    //! Write to the file stream from a statically allocated buffer. This provides no performance benefit
    //! over using a dynamic buffer.
    //! 
    //! \tparam LENGTH - Input buffer length, in elements
    //!
    //! \param[in] const (&)[] buffer - Immutable reference to statically allocated input buffer
    //! \return distance_t - Number of elements written iff stream is open, otherwise zero
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    distance_t  write(const element_t (&buffer)[LENGTH])
    {
      LOGIC_INVARIANT(Handle != nullptr);

      return write(buffer, LENGTH);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocalFileStream::write
    //! Write to the file stream from an input buffer of dynamic length
    //! 
    //! \param[in] const* buffer - Immutable input buffer pointer
    //! \param[in] length - Buffer length, in elements
    //! \return distance_t - Number of elements write iff stream is open, otherwise zero
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t  write(const element_t* buffer, distance_t length)
    {
      LOGIC_INVARIANT(Handle != nullptr);

      // Write data in elements
      return Handle ? fwrite(buffer, sizeof(element_t), length, Handle) : default<distance_t>();
    }
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    FileAccess  Access;    //!< Access privileges
    ::FILE*     Handle;    //!< File system handle
  };
  
    
} //namespace wtl
#endif // WTL_LOCAL_FILE_STREAM_HPP


