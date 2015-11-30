/////////////////////////////////////////////////////////////////////////////////////////
//! \file Console.hpp
//! \brief Contains the buffer, manipulators, traits, and stream type for a custom output stream  
//! \brief  that supports writing formatted coloured text to stdout through the Win32 console API 
//! \brief  using the existing stream manipulators provided by the standard library.
//! \date 28 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSOLE_HPP
#define WTL_CONSOLE_HPP

#include <string>                     //!< std::char_traits
#include <vector>                     //!< std::vector
#include <sstream>                    //!< std::basic_stringstream
#include <ios>                        //!< std::ios_base
#include <windows.h>                  //!< Win32 console API
#include <WTL/utils/Point.hpp>        //!< Point
#include <WTL/utils/exception.hpp>    //!< caught-exception

//////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
//////////////////////////////////////////////////////////////////////////////////////////
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct console_traits - Traits type for the debug-console stream providing a custom stream position type
  //! 
  //! \tparam CHAR - Character type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR>
  struct console_traits : std::char_traits<CHAR>
  {
    //////////////////////////////////////////////////////////////////////////////////////////
    //! \struct Coord - Custom stream position type 
    //////////////////////////////////////////////////////////////////////////////////////////
    struct Coord : Point<int16_t>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
      //! \alias type - Define own type
      using type = Coord;

      //! \alias base - Define base type
      using base = Point<int16_t>;

      // ----------------------------------- REPRESENTATION -----------------------------------
    
      // ------------------------------------ CONSTRUCTION ------------------------------------
    
      //////////////////////////////////////////////////////////////////////////////////////////
      // Coord::Coord 
      //! Construct from co-ordinates
      //////////////////////////////////////////////////////////////////////////////////////////
      using base::Point;
    
      //////////////////////////////////////////////////////////////////////////////////////////
      // Coord::Coord 
      //! Construct from base type co-ordinates
      //!
      //! \param[in] const& pt - Co-ordinates
      //////////////////////////////////////////////////////////////////////////////////////////
      template <typename T> 
      Coord(const Point<T>& pt) : base(pt)
      {}
    
      //////////////////////////////////////////////////////////////////////////////////////////
      // Coord::Coord
      //! Dummy constructor for constructing invalid position sentinel value (for compatibilty with basic_streambuf stub methods)
      //!
      //! \param[in] const& - Ignored
      //////////////////////////////////////////////////////////////////////////////////////////
      Coord(int) : base(-1,-1)
      {}

      //////////////////////////////////////////////////////////////////////////////////////////
      // Coord::Coord
      //! Dummy constructor for constructing invalid position sentinel value (for compatibilty with basic_streambuf stub methods)
      //!
      //! \param[in] const& - Ignored
      //////////////////////////////////////////////////////////////////////////////////////////
      template <typename T> 
      Coord(const std::fpos<T>&) : base(-1,-1)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
      //////////////////////////////////////////////////////////////////////////////////////////
      // Coord::operator== const
      //! Compare against other points
      //////////////////////////////////////////////////////////////////////////////////////////
      using base::operator==;

      //////////////////////////////////////////////////////////////////////////////////////////
      // Coord::operator== const
      //! Compare whether equal with invalid position sentinel value (-1)
      //!
      //! \param[in] const& - Ignored
      //////////////////////////////////////////////////////////////////////////////////////////
      bool operator==(const std::streamoff&) const
      {
        return *this == type(-1,-1);
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------
    };

    //! \alias pos_type - Define absolute stream position type as screen co-ordinate
    using pos_type = Coord;

    //! \alias off_type - Define relative stream offset type as position type
    using off_type = pos_type;
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct console_streambuf - Custom debug-console stream buffer 
  //! 
  //! \tparam CHAR - Character type
  //! \tparam TRAITS - [optional] Type providing character traits
  //! 
  //! \remarks Provides a custom stream buffer for the debug-console stream, enabling buffered output, 
  //! \remarks  custom positioning, and handling of foreground/background colour attributes
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS = console_traits<CHAR>>
  struct console_streambuf : std::basic_streambuf<CHAR,TRAITS>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = console_streambuf<CHAR,TRAITS>;

    //! \alias base - Define base type
    using base = std::basic_streambuf<CHAR,TRAITS>;

    //! \alias char_type - Inherit character type
    using char_type = typename base::char_type;

    //! \alias int_type - Inherit integer representation type
    using int_type = typename base::int_type;
  
    //! \alias openmode - Inherit stream open mode
    using openmode = std::ios_base::openmode;

    //! \alias off_type - Inherit offset type
    using off_type = typename base::off_type;

    //! \alias pos_type - Inherit position type
    using pos_type = typename base::pos_type;
  
    //! \alias seekdir - Inherit seek direction
    using seekdir = std::ios_base::seekdir;
  
    //! \alias traits_type - Inherit traits type
    using traits_type = typename base::traits_type;

  private:
    static const uint16_t  BackgroundMask = 0x00f0,     //!< Background text attributes mask
                           ForegroundMask = 0x000f;     //!< Foreground text attributes mask

    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    std::vector<char_type>  Buffer;       //!< Buffer 'put area'  
    ::HANDLE                Handle;       //!< console_stream handle

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::console_streambuf
    //! Construct debug-console buffer 
    //! 
    //! \param[in] len - [optional] Length of buffer (in characters)
    //! 
    //! \throw std::runtime_error - Unable to retrieve handle to stdout
    //////////////////////////////////////////////////////////////////////////////////////////
    console_streambuf(uint32_t len = 1024) : Buffer(len), Handle(::GetStdHandle(STD_OUTPUT_HANDLE))
    {
      // Verify handle to stdout
      if (!Handle || Handle == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Unable to get handle to stdout");

      // Define put area
      this->setp(Buffer.data(), Buffer.data() + Buffer.capacity());   
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  
    console_streambuf(const console_streambuf&) = default;    //!< Copying produces a deep copy  [Handle is not unique]
    console_streambuf(console_streambuf&&) = default;         //!< Can be moved
	
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::~console_streambuf
    //! Flushes the buffer 
    //////////////////////////////////////////////////////////////////////////////////////////
    ~console_streambuf() override
    {
      sync();
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  private:  
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::used const
    //! Query the number of characters written to the 'put area'
    //! 
    //! \return uint32_t - Number of characters in the 'put area'
    //////////////////////////////////////////////////////////////////////////////////////////
    uint32_t used() const
    {
      return this->pptr() - this->pbase();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::setback
    //! Modifies the background colour
    //! 
    //! \param[in] flag - Foreground colour
    //////////////////////////////////////////////////////////////////////////////////////////
    void setback(uint16_t flag)
    {
      ::CONSOLE_SCREEN_BUFFER_INFO sb;
    
      //! Flush 'put area' to output stream
      sync();

      // Preserve foreground formatting, change background formatting
      ::GetConsoleScreenBufferInfo(Handle, &sb);
      ::SetConsoleTextAttribute(Handle, (sb.wAttributes & ForegroundMask) | (flag & BackgroundMask)); 
    }
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::setfore
    //! Modifies the foreground/text colour
    //! 
    //! \param[in] flag - Foreground colour
    //////////////////////////////////////////////////////////////////////////////////////////
    void setfore(uint16_t flag)
    {
      ::CONSOLE_SCREEN_BUFFER_INFO sb;
    
      //! Flush 'put area' to output stream
      sync();

      // Preserve background formatting, change foreground formatting
      ::GetConsoleScreenBufferInfo(Handle, &sb);
      ::SetConsoleTextAttribute(Handle, (sb.wAttributes & BackgroundMask) | (flag & ForegroundMask)); 
    }

  protected:
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::overflow
    //! Synchronizes the buffer and output sequence before writing another character
    //! 
    //! \param[in] ch - Character to write
    //! \return int_type - EOF if unsuccessful
    //////////////////////////////////////////////////////////////////////////////////////////
    int_type overflow(int_type ch = traits_type::eof()) override
    {
      //! Commit put area to the output stream
      if (!flush())
      {
        //! [FAILED] Destroy put area
        this->setp(nullptr, nullptr);
        return traits_type::eof();
      }

      //! [~EOF] Store character in the put area
      if (!traits_type::eq_int_type(ch, traits_type::eof()))
      {
        *this->pptr() = traits_type::to_char_type(ch);
        this->pbump(1);
      }

      return ch;
    }
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::seekpos
    //! Seeks the get-pointer and/or put-pointer to a relative position
    //! 
    //! \param[in] offset - Relative position
    //! \param[in] origin - Seek origin
    //! \param[in] mode - [optional] Whether to seek the get and/or put position
    //! \return pos_type - New absolute position
    //////////////////////////////////////////////////////////////////////////////////////////
	  pos_type seekoff(off_type offset, seekdir origin, openmode mode = std::ios_base::in | std::ios_base::out) override
	  {	
      ::CONSOLE_SCREEN_BUFFER_INFO sb;

      // Flush existing buffer
      sync();
    
      // Query current position
      ::GetConsoleScreenBufferInfo(Handle, &sb);
      pos_type target = pos_type(sb.dwCursorPosition) + offset;

      // Attempt to set new position
      if (!::SetConsoleCursorPosition(Handle, target))
        // [FAILED] Return existing position
        return {sb.dwCursorPosition};
      
      // Return new position
      return target;
	  }

    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::seekpos
    //! Seeks the get-pointer and/or put-pointer to an absolute position
    //! 
    //! \param[in] pos - Absolute position
    //! \param[in] mode - [optional] Whether to seek the get and/or put position
    //! \return pos_type - New absolute position
    //////////////////////////////////////////////////////////////////////////////////////////
    pos_type seekpos(pos_type pos, openmode mode = std::ios_base::in | std::ios_base::out) override
	  {	
      // Flush existing buffer
      sync();

      // Attempt to set new position
      if (!::SetConsoleCursorPosition(Handle, pos))
      {
        ::CONSOLE_SCREEN_BUFFER_INFO sb;

        // [FAILED] Return existing position
        ::GetConsoleScreenBufferInfo(Handle, &sb);
        return {sb.dwCursorPosition};
      }

      // Return new position
      return pos;
	  }

    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::sync
    //! Synchronizes the buffer and output sequence
    //! 
    //! \return int - 0 if successful, -1 otherwise
    //////////////////////////////////////////////////////////////////////////////////////////
    int sync() override
    {
      return !used() || flush() ? 0 : -1;
    }

  private:
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_streambuf::flush
    //! Flushes the buffer to the console
    //! 
    //! \return bool - True iff successfully flushed buffer to output sequence
    //////////////////////////////////////////////////////////////////////////////////////////
    bool flush()
    {
      DWORD    out = 0;
      uint32_t len = used();

      //! Commit put area to the output stream
      if (!::WriteConsoleA(Handle, this->pbase(), len, &out, nullptr) || out != len)
      {
        //! [FAILED] Destroy put area
        this->setp(nullptr, nullptr);
        return false;
      }

      //! Update put pointer 
      this->pbump(0 - len);
      return true;
    }
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct console_stream - Debug-console output stream
  //! 
  //! \tparam CHAR - Character type
  //! \tparam TRAITS - [optional] Character traits provider type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS = console_traits<CHAR>>
  struct console_stream : std::basic_ostream<CHAR,TRAITS>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = console_stream<CHAR,TRAITS>;

    //! \alias base - Define base type
    using base = std::basic_ostream<CHAR,TRAITS>;

    //! \var ident - Index of user-defined formatting flag used as magic number
    static const int Ident;
	
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    console_streambuf<CHAR,TRAITS>  Buffer;   //!< Stream buffer

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_stream::console_stream
    //! Constructs the debug console
    //! 
    //! \throw std::runtime_error - Unable to retrieve handle to stdout
    //////////////////////////////////////////////////////////////////////////////////////////
    console_stream() : base(&Buffer)
    {
      // Set user-defined formatting flag that identifies stream as a debug-console
      this->iword(Ident) = true;
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
	  console_stream(const console_stream&) = default;			//!< Can be copied
	  console_stream(console_stream&&) = default;			      //!< Can be moved
    ~console_stream() override = default;    			        //!< Can be polymorphic
		
    // ----------------------------------- STATIC METHODS -----------------------------------  

    //////////////////////////////////////////////////////////////////////////////////////////
    // ::operator<< friend
    //! Output overload for writing strings with different character traits
    //! 
    //! \param[in,out] &os - Output stream
    //! \param[in] const& str - String
    //! \return base& - Reference to 'os'
    //////////////////////////////////////////////////////////////////////////////////////////
    /*template <typename STR_TRAITS, typename ALLOC>
    friend base& operator<<(base& os, const std::basic_string<CHAR,STR_TRAITS,ALLOC>& str)
    {
      return os.write(str.data(), str.size());
    }*/

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // console_stream::setback
    //! Modifies the background colour
    //! 
    //! \param[in] flag - Background colour
    //////////////////////////////////////////////////////////////////////////////////////////
    void setback(uint16_t flag)
    {
      Buffer.setback(flag);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // console_stream::setfore
    //! Modifies the foreground/text colour
    //! 
    //! \param[in] flag - Foreground colour
    //////////////////////////////////////////////////////////////////////////////////////////
    void setfore(uint16_t flag)
    {
      Buffer.setfore(flag);
    }
  };

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \var console_stream::ident - Index of user-defined formatting flag used by debug-console streams
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS>
  const int console_stream<CHAR,TRAITS>::Ident = std::ios_base::xalloc();
	
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias console - Narrow-character debug-console stream
  //////////////////////////////////////////////////////////////////////////////////////////
  using console = console_stream<char>;

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \alias console - Narrow-character debug-console stream
  //////////////////////////////////////////////////////////////////////////////////////////
  using wconsole = console_stream<wchar_t>;

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \var wtl::cdebug - Narrow character debug console stream
  //////////////////////////////////////////////////////////////////////////////////////////
  extern console cdebug;


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct textcol - Debug-console stream foreground colour manipulator
  //////////////////////////////////////////////////////////////////////////////////////////
  struct textcol
  {
    //! \enum colour_t - Foreground colours
    enum colour_t : uint16_t
    { 
      bold   = FOREGROUND_INTENSITY,                                //!< Set bold text
      black  = 0,                                                   //!< Set black text
      cyan   = FOREGROUND_BLUE|FOREGROUND_GREEN,                    //!< Set cyan text
      blue   = FOREGROUND_BLUE,                                     //!< Set blue text
      green  = FOREGROUND_GREEN,                                    //!< Set green text
      grey   = bold,                                                //!< Set grey text
      purple = FOREGROUND_RED|FOREGROUND_BLUE,                      //!< Set purple text
      red    = FOREGROUND_RED,                                      //!< Set red text
      yellow = FOREGROUND_RED|FOREGROUND_GREEN,                     //!< Set yellow text
      white  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,     //!< Set white text
    };

    //////////////////////////////////////////////////////////////////////////////////////////
    // ::operator| friend
    //! Performs foreground colours to be combined with bitwise OR
    //////////////////////////////////////////////////////////////////////////////////////////
    friend colour_t operator| (const colour_t& a, const colour_t& b)
    {
      return static_cast<colour_t>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // ::operator<< friend
    //! Modifies the foreground/text colour of a debug-console stream
    //! 
    //! \param[in,out] &s - Debug-console stream
    //! \param[in] m - Foreground colour
    //! \return std::basic_ostream<CHAR,TRAITS> - Reference to 's'
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHAR, typename TRAITS>
    friend std::basic_ostream<CHAR,TRAITS>& operator<< (std::basic_ostream<CHAR,TRAITS>& s, colour_t m)
    {
      // Ensure stream is a debug-console then apply foreground formatting
      if (s.iword(console_stream<CHAR,TRAITS>::Ident))
        static_cast<console_stream<CHAR,TRAITS>&>(s).setfore(m);

      return s;
    }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct backcol - Debug-console stream background colour manipulator
  //////////////////////////////////////////////////////////////////////////////////////////
  struct backcol
  {
    //! \enum colour_t - Background colours
    enum colour_t : uint16_t
    { 
      bold   = BACKGROUND_INTENSITY,                              //!< Set bold background
      black  = 0,                                                 //!< Set black background
      cyan   = BACKGROUND_BLUE|BACKGROUND_GREEN,                  //!< Set cyan background
      blue   = BACKGROUND_BLUE,                                   //!< Set blue background
      green  = BACKGROUND_GREEN,                                  //!< Set green background
      grey   = bold,                                              //!< Set grey background
      purple = BACKGROUND_RED|BACKGROUND_BLUE,                    //!< Set purple background
      red    = BACKGROUND_RED,                                    //!< Set red background
      yellow = BACKGROUND_RED|BACKGROUND_GREEN,                   //!< Set yellow background
      white  = BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE,   //!< Set white background
    };
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // ::operator| friend
    //! Performs background colours to be combined with bitwise OR
    //////////////////////////////////////////////////////////////////////////////////////////
    friend colour_t operator| (const colour_t& a, const colour_t& b)
    {
      return static_cast<colour_t>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // ::operator<< friend
    //! Modifies the background colour of a debug-console stream
    //! 
    //! \param[in,out] &s - Debug-console stream
    //! \param[in] m - Background colour
    //! \return std::basic_ostream<CHAR,TRAITS> - Reference to 's'
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHAR, typename TRAITS>
    friend std::basic_ostream<CHAR,TRAITS>& operator<< (std::basic_ostream<CHAR,TRAITS>& s, colour_t m)
    {
      // Ensure stream is a debug-console then apply background formatting
      if (s.iword(console_stream<CHAR,TRAITS>::Ident))
        static_cast<console_stream<CHAR,TRAITS>&>(s).setback(m);

      return s;
    }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct format - Debug-console stream formatting manipulator
  //////////////////////////////////////////////////////////////////////////////////////////
  struct format
  {
    //! \enum command_t - Formatting commands
    enum command_t 
    { 
      heading,      //!< Prints text in cyan
      success,      //!< Prints 'Success' in green
      failure,      //!< Prints 'Failure' in red
      error,        //!< Prints 'ERROR' in red
      warning,      //!< Prints 'WARNING' in blue
    };

    //////////////////////////////////////////////////////////////////////////////////////////
    // ::operator<< friend
    //! Modifies the formatting of a debug-console stream
    //! 
    //! \param[in,out] &s - Debug-console stream
    //! \param[in] m - Formatting
    //! \return std::basic_ostream<CHAR,TRAITS> - Reference to 's'
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHAR, typename TRAITS>
    friend std::basic_ostream<CHAR,TRAITS>& operator<< (std::basic_ostream<CHAR,TRAITS>& s, command_t m)
    {
      switch (m)
      {
      case heading: return s << backcol::black << (textcol::green|textcol::bold);
      case success: return s << backcol::black << (textcol::green|textcol::bold) << "Success: ";
      case failure: return s << backcol::black <<  (textcol::red|textcol::bold)  << "Failure: ";
      case error:   return s << backcol::black <<  (textcol::red|textcol::bold)  << "ERROR: ";
      }
    
      return s;
    }
  };


  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<< 
  //! Write a string to an output stream with different character traits
  //! 
  //! \tparam CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //! \tparam STR_TRAITS - String character type
  //! \tparam ALLOC - String allocator
  //!
  //! \param[in,out] &os - Output stream
  //! \param[in] const& str - String
  //! \return base& - Reference to 'os'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename OS_TRAITS, typename STR_TRAITS, typename ALLOC>
  std::basic_ostream<CHAR,OS_TRAITS>&  operator<<(std::basic_ostream<CHAR,OS_TRAITS>& os, const std::basic_string<CHAR,STR_TRAITS,ALLOC>& str)
  {
    return os.write(str.data(), str.size());
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes an attribute enumeration to a debug console output stream
  //! 
  //! \tparam CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //! \tparam T - Enumeration type
  //!
  //! \param[in,out] &c - Output stream
  //! \param[in] val - Enumeration value
  //! \return std::basic_ostream<CHAR,TRAITS>& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS, typename T, typename = enable_if_attribute_t<T>>
  std::basic_ostream<CHAR,TRAITS>&  operator << (std::basic_ostream<CHAR,TRAITS>& c, T val) 
  { 
    std::basic_ostringstream<CHAR> ss;

    // Format as hex
    ss << std::hex << std::uppercase << enum_cast(val);
    return c << ss.str();
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a non-attribute enumeration to a debug console output stream
  //! 
  //! \tparam CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //! \tparam T - Enumeration type
  //!
  //! \param[in,out] &c - Output stream
  //! \param[in] val - Enumeration value
  //! \return std::basic_ostream<CHAR,TRAITS>& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS, typename T, typename = enable_if_not_attribute_t<T>, typename = void>
  std::basic_ostream<CHAR,TRAITS>&  operator << (std::basic_ostream<CHAR,TRAITS>& c, T val) 
  { 
    return c << enum_cast(val);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a caught-exception to a debug console output stream
  //! 
  //! \tparam CHAR - Output stream character type
  //! \tparam TRAITS - Output stream character traits
  //!
  //! \param[in,out] &c - Output stream
  //! \param[in] const& ex - Caught exception wrapper
  //! \return std::basic_ostream<CHAR,TRAITS>& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename CHAR, typename TRAITS>
  std::basic_ostream<CHAR,TRAITS>&  operator << (std::basic_ostream<CHAR,TRAITS>& c, const caught_exception& ex)  
  { 
    return c << '\n' << (textcol::red   |textcol::bold) << "EXCEPTION: " << textcol::white  << ex.Problem  << "..." << ex.Cause 
             << '\n' << (textcol::yellow|textcol::bold) << "CAUGHT: "    << textcol::yellow << ex.source() << "..." << '\n';
  }


} // WTL namespace

#endif // WTL_CONSOLE_HPP
