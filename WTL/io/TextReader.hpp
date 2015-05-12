//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\TextReader.hpp
//! \brief Provides text decoding for input streams
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_TEXT_READER_HPP
#define WTL_TEXT_READER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct TextReader - Reads formatted text from an input stream
  //! 
  //! \tparam STREAM - Input stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct TextReader 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \typedef delegate_t - Defines signature of input overload
    typedef TextReader& (*delegate_t)(TextReader&);
    
    //! \typedef distance_t - Stream distance type
    typedef typename STREAM::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename STREAM::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename STREAM::position_t position_t;
    
    //! \typedef stream_t - Stream type
    typedef STREAM stream_t;
    
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::TextReader 
    //! Create a text reader and initialise the input stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit TextReader(ARGS&&... args) : Stream(std::forward<ARGS>(args)...)
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::~TextReader
    //! Virtual d-tor
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~TextReader()
    {}

    // Copy semantics determined by stream type
    ENABLE_COPY(TextReader);
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::match
    //! Matches a regular expression without advancing the stream
    //! 
    //! \param[in] const &regex - Regular expression to match
    //! \return bool - True if matched, otherwise false
    //! 
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    bool match(const std::regex& regex)
    {
      std::cmatch match;

      // Match regex using direct stream access
      bool result = std::regex_search(Stream.buffer(), match, regex, std::regex_constants::match_continuous)
                 && match.position() == 0;

      // Cleanup 
      Stream.release(0);
      return result;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::read
    //! Reads text matched by regular expression, and advances the stream beyond the match
    //! 
    //! \param[in] const& regex - Regular expression 
    //! \return std::string - First capture group, if any, otherwise matched text
    //////////////////////////////////////////////////////////////////////////////////////////
    std::string read(const std::regex& regex)
    {
      std::cmatch match;

      // Match regex using direct stream access
      if (!std::regex_search(Stream.buffer(), match, regex, std::regex_constants::match_continuous))
      {
        Stream.release(0);
        throw wtl::domain_error(HERE, "No match");
      }
      
      // Advance stream
      Stream.seek(match.length(), FileSeek::Current);

      // Return first capture group, if any, otherwise matched text
      return match[match.size()-1].str();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::read
    //! Reads text matched by regular expression, and advances the stream beyond the match
    //! 
    //! \param[in] const& regex - Regular expression 
    //! \return std::string - First capture group, if any, otherwise matched text
    //////////////////////////////////////////////////////////////////////////////////////////
    /*std::string read(const element_t* format)
    {
      sscanf(
    }*/

    //////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::read
    //! Reads a character from the input stream. No bounds checking is performed.
    //! 
    //! \return element_t - Character 
    //! 
    //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
    //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    element_t read()
    {
      CHECKED_LENGTH(Stream.remaining(), 1);

      // Read char
      return Stream.get();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::readf
    //! Reads variables arguments accoring to a formatting string directly from the output stream.
    //! 
    //! \param[in] const* str - Formatting string
    //! \param[in] ... - Variadic arguments
    //!
    //! \throw wtl::length_error - Formatted string would exceed buffer capacity
    //! \throw wtl::out_of_range - [Debug only] Initial stream position out of bounds
    //////////////////////////////////////////////////////////////////////////////////////////
    //int32 readf(const element_t* format, ...)
    //{
    //  va_list args;
    //  va_start(args, format);

    //  // Attempt to read from stream
    //  int32 numMatches = vsscanf(Stream.buffer(), format, args);
    //  Stream.release();

    //  return numMatches < 0 ? 0 : numMatches;
    //}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // TextReader::operator=
    //! Copy-assign from a text writer of different type
    //! 
    //! \param[in,out] const& r - Text writer of different type
    //! \return TextReader& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    TextReader& operator=(const TextReader<U>& r)
    {
      // Avoid self-assignment, and copy stream
      if (static_cast<void*>(this) != static_cast<const void*>(&r))
        Stream = r.Stream;

      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    stream_t   Stream;     //!< Input stream
  };


  // --------------------------- DELIMITERS --------------------------
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator >> 
  //! Reads elements of an input range delimited by a custom character
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //! \tparam D - Whether array can be resized
  //!
  //! \param[in,out] &w - Text reader
  //! \param[in] const &p - Pair containing an array and delimiter character respectively
  //! \return TextReader<STREAM>& : Reference to reader 'r'
  //!  
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  //template <typename STREAM, typename ITERATOR>
  //TextReader<STREAM>& operator>> (TextReader<STREAM>& r, delimited_range_t<ITERATOR>& range)
  //{
  //  // TODO
  //  return r;
  //}
  //
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
  TextReader<STREAM>& operator>> (TextReader<STREAM>& r, delimited_range_t<ITERATOR>& range) 
  {
    //w.write(range.First, range.Last, range.Delimiter);
    return r;
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Reads any integral type from a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Integral type
  //!
  //! \param[in,out] &r - Text reader
  //! \param[in] val - Integral value 
  //! \return TextReader<STREAM>& - Reference to 'r'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  std::enable_if_t<std::is_integral<U>::value, TextReader<STREAM>&>
  /*TextReader<STREAM>&*/ operator >> (TextReader<STREAM>& r, U& val)
  {
    return r;
  }
  
} //namespace wtl
#endif // WTL_TEXT_READER_HPP
