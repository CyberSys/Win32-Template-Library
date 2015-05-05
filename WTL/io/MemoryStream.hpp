//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\MemoryStream.hpp
//! \brief Provides stream semantics for in-memory buffers
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STREAM_WRITER_HPP
#define WTL_STREAM_WRITER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //! \enum FileAccess - file access modes for the GLS file system (Formerly GLS_FILE_ACCESS)
  //! \ingroup dox_GLSSPMGroup
  enum class FileAccess : int32 
  {
    Create,         //!< Create a new file. If it exists, overwrite by truncating to zero. (Equivalent to "wb" in fopen) (Formerly GLS_FILE_CREATE)
    OpenExisting,   //!< Open existing file for append mode. If it does not exist, will be created. (Equivalent to "ab" in fopen) (Formerly GLS_FILE_OPEN_EXISTING)
    ReadOnly,       //!< Open the file read only. If it does not exist, this will fail. (Equivalent to "rb" in fopen) (Formerly GLS_FILE_READ_ONLY)
  };


  //! \enum FileSeek - File seek type
  enum class FileSeek : int32
  {
    Begin,       //!< Seek from beginning of stream
    Current,     //!< Seek from current stream position
    End,         //!< Seek from end of stream
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MemoryStream - Provides stream semantics upon an in-memory buffer
  //! 
  //! \tparam ELEMENT - Element type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ELEMENT>
  struct MemoryStream
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \typedef distance_t - Stream distance type
    typedef uint32  distance_t;
    
    //! \typedef element_t - Stream element type
    typedef ELEMENT  element_t;
    
    //! \typedef position_t - Stream position type
    typedef uint32  position_t;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::MemoryStream
    //! Cannot default-construct memory stream 
    //////////////////////////////////////////////////////////////////////////////////////////
    MemoryStream() = delete;
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::MemoryStream
    //! Create memory stream for a buffer of dynamic length
    //!
    //! \param[in] *buf - Input buffer
    //! \param[in] len - Input buffer Length
    //!
    //! \throw wtl::invalid_argument - [Debug only] Missing buffer
    //! \throw wtl::length_error - [Debug only] Length is zero
    //////////////////////////////////////////////////////////////////////////////////////////
   /* template <unsigned LENGTH>
    MemoryStream(const CharArray<LENGTH,element_t>& r) 
    {
      
    }*/
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::MemoryStream
    //! Create memory stream for a buffer of dynamic length
    //!
    //! \param[in] *buf - Input buffer
    //! \param[in] len - Input buffer Length
    //!
    //! \throw wtl::invalid_argument - [Debug only] Missing buffer
    //! \throw wtl::length_error - [Debug only] Length is zero
    //////////////////////////////////////////////////////////////////////////////////////////
    MemoryStream(element_t* buf, distance_t len) : Position(buf), 
                                                   Start(Position),
                                                   End(Start+len)
    {
      REQUIRED_PARAM(buf);
      LENGTH_INVARIANT(len > 0);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::MemoryStream 
    //! Create memory stream for a statically allocated buffer 
    //! 
    //! \tparam LENGTH - Length of buffer  
    //!
    //! \param[in] (&)[LENGTH] buf - Statically allocated input buffer
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    explicit MemoryStream(element_t (&buf)[LENGTH]) : Position(static_cast<element_t*>(buf)), 
                                                      Start(Position),
                                                      End(Start+LENGTH)
    {
      static_assert(LENGTH > 0, "Invalid input buffer length");
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::~MemoryStream
    //! Virtual d-tor
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~MemoryStream()
    {
    }

    // Shallow copy semantics
    DEFAULT_COPY(MemoryStream);
    
    // -------------------------------------- ACCESSORS --------------------------------------			
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::begin() const
    //! Get a pointer to the first element 
    //! 
    //! \return const element_t* - Immutable pointer to start of stream
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //////////////////////////////////////////////////////////////////////////////////////////
    const element_t* begin() const
    {
      CHECKED_INDEX(Position, Start, End+1);

      return Start;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::length() const
    //! Get length of stream
    //! 
    //! \return distance_t - Length of stream, in elements
    //!
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t length() const
    {
      CHECKED_INDEX(Position, Start, End+1);

      return End - Start;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::position() const
    //! Get current position within the stream 
    //! 
    //! \return position_t - Zero-based position: 0 <= n <= length
    //! 
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    position_t position() const
    {
      CHECKED_INDEX(Position, Start, End+1);

      return Position;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::remaining() const
    //! Get the number of available elements remaining in the stream 
    //! 
    //! \return distance_t - Number of unused elements remaining in the stream buffer 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t remaining() const
    {
      CHECKED_INDEX(Position, Start, End+1);

      return End - Position;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::used() const
    //! Get the number of elements already written to the stream 
    //! 
    //! \return distance_t - Number of elements already written to the stream buffer 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t used() const
    {
      CHECKED_INDEX(Position, Start, End+1);

      return Position - Start;
    }
    
    // --------------------------------------- MUTATORS --------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::close() 
    //! No-op
    //////////////////////////////////////////////////////////////////////////////////////////
    void close() 
    {
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::flush() 
    //! No-op
    //////////////////////////////////////////////////////////////////////////////////////////
    void flush() 
    {
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::buffer()
    //! Returns direct access to the current position in the buffer.
    //! 
    //! \return element_t* - Mutable pointer to current position in stream buffer
    //!
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //!
    //! \remarks This method clearly leaks representation and is inheriently unsafe, but necessary
    //! for high performance operation.
    //////////////////////////////////////////////////////////////////////////////////////////
    element_t* buffer() 
    {
      CHECKED_INDEX(Position, Start, End+1);

      return Position;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::get() 
    //! Reads a single character from the stream. No bounds checking is performed.
    //! If there is no remaining stream capacity, the results are undefined.
    //! 
    //! \return element_t - Element
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    element_t get() 
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_LENGTH(1, remaining());

      // Copy stream -> character, and advance position
      element_t chr(*Position);
      return (++Position, chr);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::put() 
    //! Writes a single character from the stream. No bounds checking is performed.
    //! If there is no remaining stream capacity, the results are undefined.
    //! 
    //! \param[in] chr - Element
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void put(element_t chr) 
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_LENGTH(1, remaining());

      // Copy character -> stream, and advance position
      *Position = chr;
      ++Position;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::read() 
    //! Optimized read for statically allocated output buffers. No bounds checking is performed.
    //! If output buffer size exceeds remaining stream capacity, the results are undefined.
    //! 
    //! \tparam LENGTH - Length of output buffer, in elements
    //! 
    //! \param[in,out] (&)[LENGTH] buffer - Reference to statically allocated output buffer
    //! \return uin32 - Number of elements read (Always return 'length')
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    uint32 read(std::remove_const_t<element_t> (&buffer)[LENGTH]) 
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_LENGTH(LENGTH, remaining());

      // Copy stream -> buffer, and advance position
      memcpy(buffer, Position, LENGTH*sizeof(element_t));
      Position += LENGTH;

      return LENGTH;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::read() 
    //! Reads the stream into an output buffer, without performing any bounds checking.
    //! If input buffer size exceeds remaining stream capacity, the results are undefined.
    //! 
    //! \param[in,out] *buffer - Output buffer
    //! \param[in] length - Length of output buffer
    //! \return uin32 - Number of elements read (Always return 'length')
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    uint32 read(std::remove_const_t<element_t>* buffer, uint32 length) 
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_LENGTH(length, remaining());

      // Copy stream -> buffer, and advance position
      memcpy(buffer, Position, length*sizeof(element_t));
      Position += length;

      return length;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::release()
    //! Release direct stream access
    //! 
    //! \param[in] count - Number of characters written to the buffer
    //!
    //! \throw wtl::out_of_range - [Debug only] Initial or final stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void release(uint32 count)
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_INDEX(Position+count, Start, End+1);

      // Advance position
      Position += count;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::resize() 
    //! Resizes the stream 
    //! 
    //! \param[in] length - New length
    //! 
    //! \throw wtl::length_error - [Debug only] Length is zero
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void resize(distance_t length)
    {
      CHECKED_INDEX(Position, Start, End+1);
      LENGTH_INVARIANT(length > 0);

      // Set length, adjust position to EOF if necessary
      End = length;
      Position = (Position < End ? Position : End);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::seek() 
    //! Seek the stream
    //! 
    //! \param[in] offset - Distance to move, in elements
    //! \param[in] origin - Seek origin
    //! \return int32 - Number of unused elements remaining in the stream buffer 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Initial position or destination position is out-of-bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    int32 seek(int32 offset, FileSeek origin) 
    {
      CHECKED_INDEX(Position, Start, End+1);

      switch (origin)
      {
      // Begin:
      case FileSeek::Begin:  
        CHECKED_INDEX(Start+offset, Start, End+1);
        Position = Start+offset;  
        break;

      // Current:
      case FileSeek::Current:
        CHECKED_INDEX(Position+offset, Start, End+1);
        Position += offset;  
        break;

      // End:
      case FileSeek::End:  
        CHECKED_INDEX(End-offset, Start, End+1);
        Position = End-offset;  
        break;
      }
      
      return 1;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::write() 
    //! Optimized write for statically allocated input buffers. No bounds checking is performed.
    //! If input buffer size exceeds remaining stream capacity, the results are undefined.
    //! 
    //! \tparam LENGTH - Length of input buffer, in elements
    //! 
    //! \param[in,out] const (&)[LENGTH] buffer - Reference to statically allocated input buffer
    //! \return uin32 - Number of elements read (Always return 'length')
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    uint32 write(const element_t (&buffer)[LENGTH]) 
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_LENGTH(LENGTH * sizeof(element_t), remaining());

      // Copy buffer -> stream, and advance position
      memcpy(Position, buffer, LENGTH*sizeof(element_t));
      Position += LENGTH;

      return LENGTH;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::write() 
    //! Write from an input buffer into the stream, without performing any bounds checking.
    //! If input buffer size exceeds remaining stream capacity, the results are undefined.
    //! 
    //! \param[in] const* buffer - Input buffer
    //! \param[in] length - Length of input buffer
    //! \return uin32 - Number of elements written (Always return 'length')
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    uint32 write(const element_t* buffer, uint32 length) 
    {
      CHECKED_INDEX(Position, Start, End+1);
      CHECKED_LENGTH(length * sizeof(element_t), remaining());

      // Copy buffer -> stream, and advance position
      memcpy(Position, buffer, length*sizeof(element_t));
      Position += length;

      return length;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    element_t *Position,  //!< Position
              *Start,     //!< Start-of-stream position
              *End;       //!< End-of-stream position
  };
  

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct BufferStream - Provides a stream with an internal buffer
  //! 
  //! \tparam ELEMENT - Element type
  //! \tparam CAPACITY - Internal buffer capacity
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ELEMENT, unsigned CAPACITY>
  struct BufferStream : MemoryStream<ELEMENT>
  {
    static_assert(CAPACITY > 0, "Cannot create a buffer of zero length");

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \typedef base - Base class alias
    typedef MemoryStream<ELEMENT> base;
    
    //! \typedef distance_t - Stream distance type
    typedef typename base::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename base::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename base::position_t position_t;

    //! \enum - Stream capacity
    enum { capacity = CAPACITY };   

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BufferStream::BufferStream 
    //! Creates a buffered stream of length 'capacity'
    //////////////////////////////////////////////////////////////////////////////////////////
    BufferStream() : base(Storage, capacity)
    {
      Storage[0] = default_t<element_t>::value;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // BufferStream::BufferStream 
    //! Creates a buffered stream of length 'len' and copies into it from an existing buffer
    //! 
    //! \param[in] const* buf - Source buffer (If nullptr the results are undefined)
    //! \param[in] len - Length of buffer
    //! 
    //! \throw wtl::invalid_argument - [Debug only] Missing buffer
    //! \throw wtl::length_error - [Debug only] Length exceeds capacity (or is zero)
    //////////////////////////////////////////////////////////////////////////////////////////
    BufferStream(const element_t* buf, distance_t len) : base(const_cast<element_t*>(Storage), len)
    {
      LENGTH_INVARIANT(len > 0 && len <= capacity);
      REQUIRED_PARAM(buf);

      // Copy from input buffer
      memcpy(Storage, buf, len*sizeof(element_t));
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BufferStream::BufferStream 
    //! Creates a stream buffer by copying an existing buffer
    //! 
    //! \tparam LENGTH - Length of buffer
    //!
    //! \param[in] (&)[LENGTH] buf - Statically allocated input buffer
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    BufferStream(element_t (&buf)[LENGTH]) : base(Storage, LENGTH)
    {
      static_assert(LENGTH > 0 && LENGTH <= capacity, "Invalid input buffer length");

      // Unsafe copy
      memcpy(Storage, &buf, LENGTH*sizeof(element_t));
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // BufferStream::~BufferStream
    //! Nothing
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~BufferStream()
    {
    }

    
    DEFAULT_COPY(BufferStream);   //!< Deep copy semantics
    
    // -------------------------------------- ACCESSORS --------------------------------------			

    // --------------------------------------- MUTATORS --------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // MemoryStream::resize() 
    //! Resizes the stream 
    //! 
    //! \param[in] length - New length
    //! 
    //! \throw wtl::length_error - [Debug only] Length is zero
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    //void resize(distance_t length)
    //{
    //  CHECKED_INDEX(Position, Start, End+1);
    //  LENGTH_INVARIANT(length > 0 && length < capacity);

    //  // Set length, adjust position to EOF if necessary
    //  length < capacity;

    //  if ()
    //  {
    //    End = Storage+length;
    //    Position = (Position < Storage+capacity ? Position : Storage+capacity);
    //  }
    //}
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    std::remove_const_t<element_t>  Storage[capacity];    //!< Buffer storage
  };

  
} //namespace wtl
#endif // WTL_STREAM_WRITER_HPP
