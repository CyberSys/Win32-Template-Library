//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\TextWriter.hpp
//! \brief Provides text encoding for output streams
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TEXT_WRITER_HPP
#define WTL_TEXT_WRITER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct TextWriter - Writes formatted text to an output stream
  //! 
  //! \tparam STREAM - Output stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct TextWriter 
  {
    // ---------------------- TYPES & CONSTANTS -------------------
    
    //! \typedef delegate_t - Defines signature of output overload
    typedef TextWriter& (*delegate_t)(TextWriter&);
    
    //! \typedef stream_t - Stream type
    typedef std::remove_reference_t<STREAM> stream_t;
    
    //! \typedef distance_t - Stream distance type
    typedef typename stream_t::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename stream_t::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename stream_t::position_t position_t;
    
    //! \typedef literal_t - Stream element literal
    /*template <unsigned LENGTH>
    using literal_t = const element_t (&)[LENGTH];*/

    // ----------------------- CONSTRUCTION -----------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::TextWriter 
    //! Create a text writer and initialise the output stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit TextWriter(ARGS&&... args) : Stream(std::forward<ARGS>(args)...)
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::~TextWriter
    //! Virtual d-tor
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~TextWriter()
    {}

    // Copy semantics determined by stream type
    DEFAULT_COPY(TextWriter);
    DEFAULT_MOVE(TextWriter);
	
	  // ------------------------- STATIC ---------------------------

    // ------------------------ ACCESSORS -------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::used() const
    //! Get the number of elements already written to the stream 
    //! 
    //! \return distance_t - Number of elements already written to the stream buffer 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t used() const
    {
      return Stream.used();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::remaining() const
    //! Get the number of available elements remaining in the stream 
    //! 
    //! \return distance_t - Number of unused elements remaining in the stream buffer 
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    distance_t remaining() const
    {
      return Stream.remaining();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::str() const
    //! Get the entire output as a null terminated string
    //! 
    //! \return const element_t* - Immutable pointer to start of stream
    //! 
    //! \throw wtl::logic_error - [Debug only] Stream has been closed
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    const element_t* str() 
    {
      CHECKED_LENGTH(1, remaining());

      // Ensure null terminated
      /*Stream.put(default_t<element_t>::value);
      Stream.seek(-1, FileSeek::Current);*/

      // Return string
      return Stream.begin();
    }
    
    // ------------------------- MUTATORS -------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::flush
    //! Flushes the output stream
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    /////////////////////////////////////////////////////////////////////////////////////////
    void flush()
    {
      Stream.flush();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::write
    //! Writes a character to the output stream. No bounds checking is performed.
    //! 
    //! \param[in] c - Character 
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void write(element_t c)
    {
      CHECKED_LENGTH(2, remaining());

      // Write char 
      Stream.put(c);

      // Null terminate
      Stream.put(default_t<element_t>::value);
      Stream.seek(-1, FileSeek::Current);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::write
    //! Writes a string to the stream. No bounds checking is performed.
    //! 
    //! \param[in] const* str - Null terminated string
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void write(const element_t* str)
    {
      CHECKED_LENGTH(strlen(str)+1, remaining());

      // Write entire buffer to stream, then reverse over the null terminator
      Stream.write(str, strlen(str)+1);
      Stream.seek(-1, FileSeek::Current);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::write
    //! Writes a string to the stream. No bounds checking is performed.
    //! 
    //! \param[in] const* str - String, optionally null terminated
    //! \param[in] length - Number of elements to write
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void write(const element_t* str, uint32 length)
    {
      CHECKED_LENGTH(length, remaining());

      // Write to stream 
      Stream.write(str, length);
      
      // Null terminate then reverse over the null terminator
      Stream.put(default_t<element_t>::value);
      Stream.seek(-1, FileSeek::Current);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::write
    //! Writes a range of elements as a custom character delimited sequence
    //! 
    //! \param[in] input - First element in range
    //! \param[in] last - Position immediately beyond last element in range
    //! \param[in] delimiter - Delimiter character
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    //template <typename INPUT>
    //void write(INPUT first, INPUT last, char delimiter)
    //{
    //  // Cannot work upon empty range
    //  if (first != last)
    //  { 
    //    // Write first element
    //    *this << *first;

    //    // Write + delimit remaining N-1 elements
    //    for (auto it = first+1; it != last; ++it)
    //    {
    //      write(delimiter);
    //      *this << *it;
    //    }
    //  }
    //}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::write
    //! Optimized write for all elements of statically allocated array (eg. string literals)
    //! If remaining stream capacity is exceeded, the results are undefined.
    //! 
    //! \param[in] const (&)[] str - Statically allocated array of elements
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    void write(const element_t (&str)[LENGTH])
    {
      CHECKED_LENGTH(LENGTH, remaining());

      // Write to stream
      Stream.write(str);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::writef
    //! Writes a formatted string (with optional arguments) directly into the output stream.
    //!
    //! \remarks If the buffer would be exceeded by this operation an exception is thrown, the stream is not affected
    //! 
    //! \param[in] const* str - Formatting string
    //! \param[in] ... - Variadic arguments
    //!
    //! \throw wtl::length_error - Formatted string would exceed buffer capacity
    //! \throw wtl::out_of_range - [Debug only] Initial stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void writef(const element_t* format, ...)
    {
      va_list args;
      va_start(args, format);

      // Format directly into stream
      writevf(format, args);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::writevf
    //! Writes a formatted string (with argument list) directly into the output stream.
    //!
    //! \remarks If the buffer would be exceeded by this operation an exception is thrown, the stream is not affected
    //! 
    //! \param[in] const* str - Formatting string
    //! \param[in] args - Variadic arguments list
    //!
    //! \throw wtl::length_error - Formatted string would exceed buffer capacity
    //! \throw wtl::out_of_range - [Debug only] Initial stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    void writevf(const element_t* format, va_list args)
    {
      // Attempt to format directly into stream
      int32 capacity(Stream.remaining()),
            written(vsnprintf(Stream.buffer(), capacity, format, args));

      // Ensure successful
      if (written < 0 || written >= capacity)
        throw wtl::length_error(HERE, "Insufficient buffer space to format string");

      // Release stream
      Stream.release(written);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextWriter::operator=
    //! Copy-assign from a text writer of different type
    //! 
    //! \param[in,out] const& r - Text writer of different type
    //! \return TextWriter& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    TextWriter& operator=(const TextWriter<U>& r)
    {
      // Avoid self-assignment, and copy stream
      if (static_cast<void*>(this) != static_cast<const void*>(&r))
        Stream = r.Stream;

      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    STREAM   Stream;     //!< Output stream
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes the entire contents of a text writer stream to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in,out] &writer - Text writer
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  Console& operator<< (Console& c, TextWriter<STREAM>& writer)
  { 
    return c << writer.str();
  }



  // --------------------------- ARRAY --------------------------

  // --------------------------- DELIMITERS --------------------------
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes elements of an input range to a stream as text using a custom delimiter character 
  //!
  //! \tparam STREAM - Stream type
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] const& range - Delimited input range
  //! \return TextWriter<STREAM>& : Reference to writer 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename ITERATOR>
  TextWriter<STREAM>& operator<< (TextWriter<STREAM>& w, const delimited_range_t<ITERATOR>& range) 
  {
    delimit_range(w, range.First, range.Last, range.Delimiter);
    return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes pipe delimited array elements to a stream as text
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //! \tparam D - Whether array can be resized
  //! 
  //! \param[in,out] &w - Text writer
  //! \param[in] const &a - Array  
  //! \return TextWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L, bool D>
  TextWriter<STREAM>& operator<< (TextWriter<STREAM>& w, const Array<E,L,D>& a)
  {
    // Default to pipe separator
    return w << delimited_range(a, '|');
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Write character array to a stream as text
  //!
  //! \tparam STREAM - Stream type
  //! \tparam ENCODING - Character encoding
  //! \tparam LENGTH - Character array capacity
  //! 
  //! \param[in,out] &w - Text writer
  //! \param[in] const &p - Character array 
  //! \return TextWriter<STREAM>& : Reference to 'w'
  //! 
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, Encoding ENCODING, unsigned LENGTH>
  TextWriter<STREAM>& operator << (TextWriter<STREAM>& w, const CharArray<ENCODING,LENGTH>& p) 
  {
    w.write(p.c_str(), p.size());
    return w;
  }

  // --------------------------- COMMON --------------------------

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes all elements of a statically allocated array to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam LENGTH - Array length
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] const (&)[] str - String
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, unsigned LENGTH>
  TextWriter<STREAM>& operator << (TextWriter<STREAM>& w, const typename STREAM::element_t (&str)[LENGTH])
  {
     w.write(str);
     return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a null terminated string to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] const* str - Null terminated string
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  TextWriter<STREAM>& operator << (TextWriter<STREAM>& w, const typename STREAM::element_t* str)
  {
     w.write(str);
     return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a boolean to a stream as text using the character 'Y' or 'N'
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] val - Boolean
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  TextWriter<STREAM>& operator << (TextWriter<STREAM>& w, bool val)
  {
     w.write(val ? 'Y' : 'N');
     return w;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a character to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] ch - Character
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  TextWriter<STREAM>& operator << (TextWriter<STREAM>& w, char ch)
  {
     w.write(ch);
     return w;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes any integral or floating point type to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Integral or floating-point type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] val - Integral or floating point value 
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  std::enable_if_t<std::is_integral<U>::value 
                || std::is_floating_point<U>::value, TextWriter<STREAM>&>
  /*TextWriter<STREAM>&*/ operator << (TextWriter<STREAM>& w, U val)
  {
    w.writef(format_spec_t<U>::value, val);
    return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes the numeric representation of any enumeration type to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Enumeration type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] val - Enumeration value
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  enable_if_enum_t<U,TextWriter<STREAM>&> operator << (TextWriter<STREAM>& w, U val)
  {
    // Write as a numeral accoring to underlying type
    return w << static_cast<std::underlying_type_t<U>>(val);
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::endl
  //! Writes a carriage return & line feed (0x0d, 0x0a) to a stream
  //!
  //! \param[in,out] &w - Text writer
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
	TextWriter<STREAM>& endl(TextWriter<STREAM>& w)
	{	
    return w << "\r\n";
	}
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Executes a unary text stream operator overload
  //! 
  //! \param[in,out] &w - Text writer
  //! \param[in] *fn - Function overload
  //! \return TextWriter<STREAM>& - Reference to 'w'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  TextWriter<STREAM>& operator<< (TextWriter<STREAM>& w, typename TextWriter<STREAM>::delegate_t fn)  
  { 
    return fn(w); 
  }

} //namespace wtl
#endif // WTL_TEXT_WRITER_HPP
