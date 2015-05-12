//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\Utils\Console.hpp
//! \brief Provides coloured text output to the debugging console
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSOLE_HPP
#define WTL_CONSOLE_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"           //!< EnumCast
#include "wtl/traits/EnumTraits.hpp"        //!< is_attribute
#include "wtl/utils/Exception.hpp"          //!< wtl::exception
#include "wtl/utils/Bitset.hpp"             //!< Bitset
#include "wtl/utils/CharArray.hpp"          //!< CharArray
#include "wtl/utils/NameValuePair.hpp"      //!< NameValuePair
#include "wtl/platform/Path.hpp"            //!< Path
#include "wtl/windows/MessageBox.hpp"       //!< errorBox()
#include <stdexcept>                        //!< std::exception
#include <tuple>                            //!< std::tuple
#include <string>                           //!< std::string
#include <mutex>                            //!< std::lock_guard
#include <type_traits>                      //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct DebugInfo - Helper object for writing object instances to the debug console
  //! 
  //! \tparam ARGS... - Attribute name/value pair types
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  struct DebugInfo
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef storage_t - Attributes collection
    typedef std::tuple<ARGS...> storage_t;

    //! \var Count - Number of attribute name/value pairs
    static constexpr uint32  Count = sizeof...(ARGS);   
    
    //! \var HasAttributes - Whether element has any attributes
    static constexpr bool  HasAttributes = Count != 0;

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // DebugInfo::DebugInfo
    //! Define type name and attributes
    //! 
    //! \param[in] const* name - Type name
    //! \param[in] &&... args - [optional] Attribute name/value pairs
    //////////////////////////////////////////////////////////////////////////////////////////
    DebugInfo(const char* name, ARGS&&... args) : Name(name), 
                                                  Attributes(std::forward<ARGS>(args)...)
    {}

    // ----------------------------------- REPRESENTATION -----------------------------------

    const char*  Name;          //!< Type name
    storage_t    Attributes;    //!< Attribute name/value pair storage
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::debug_info
  //! Create debug info for an instance and specify its attributes
  //! 
  //! \param[in] const& name - Type name 
  //! \param[in] &&... args - [optional] Attribute name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  DebugInfo<ARGS...>  debug_info(const char* name, ARGS&&... args)
  {
    return DebugInfo<ARGS...>(name, std::forward<ARGS>(args)...);
  }


//! \if WIN32 - Windows implementation
#ifdef WIN32
  //! \enum IoManip - Debugging console Manipulators for changing the text colour
  enum IoManip : uint32
  { 
    Bold   = FOREGROUND_INTENSITY,                 // Set bold text
    Black  = 0,                                    // Set black text
    Cyan   = FOREGROUND_BLUE|FOREGROUND_GREEN,     // Set cyan text
    Blue   = FOREGROUND_BLUE,                      // Set blue text
    Green  = FOREGROUND_GREEN,                     // Set green text
    Grey   = Bold,                                 // Set grey text
    Purple = FOREGROUND_RED|FOREGROUND_BLUE,       // Set purple text
    Red    = FOREGROUND_RED,                       // Set red text
    Yellow = FOREGROUND_RED|FOREGROUND_GREEN,      // Set yellow text
    White  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,     // Set white text

    BkBold   = BACKGROUND_INTENSITY,               // Set bold background
    BkBlack  = 0,                                  // Set black background
    BkCyan   = BACKGROUND_BLUE|BACKGROUND_GREEN,   // Set cyan background
    BkBlue   = BACKGROUND_BLUE,                    // Set blue background
    BkGreen  = BACKGROUND_GREEN,                   // Set green background
    BkGrey   = BkBold,                             // Set grey background
    BkPurple = BACKGROUND_RED|BACKGROUND_BLUE,     // Set purple background
    BkRed    = BACKGROUND_RED,                     // Set red background
    BkYellow = BACKGROUND_RED|BACKGROUND_GREEN,    // Set yellow background
    BkWhite  = BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE,   // Set white background

    GuiMode = 0x10000000,      // Prints to std out
    LogMode = 0x20000000,      // Prints to log file
      
    Heading = 0x00010000,      // Prints text in cyan
    Success = 0x00020000,      // Prints 'Success' in green
    Failure = 0x00040000,      // Prints 'Failure' in red
    Error   = 0x00080000,      // Prints 'ERROR' in red
    Warning = 0x00100000,      // Prints 'WARNING' in blue
    Reset   = 0x00200000,      // Reset to default colours
    Endl    = 0x00400000,      // LineBreak that resets formatting
    Break   = 0x00800000,      // LineBreak that preserves formatting
  };
#else
#error IoManip not implemented
#endif

  //! \typdef Cons - Alias for debugging console i/o manipulators
  typedef IoManip Cons;

  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<Cons>  : std::true_type  {};
  template <> struct is_contiguous<Cons> : std::false_type {};

  //! Define limits traits
  template <> struct max_value<Cons>     : std::integral_constant<Cons,Cons::Break> {};
  template <> struct min_value<Cons>     : std::integral_constant<Cons,Cons::Black> {};

  //! Define names and values
  template <> struct enum_names<Cons>  { static const char* names[]; };
  template <> struct enum_values<Cons> { static const Cons values[]; };

  
//! \if WIN32 - Windows implementation
#ifdef WIN32

  // forward declaration
  struct ConsoleLock;

  //////////////////////////////////////////////////////////////////////////////////////////
  // \struct Coord - Wrapper for Win32 COORD structure
  //////////////////////////////////////////////////////////////////////////////////////////
  struct Coord : public ::COORD
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // Coord::Coord
    //! Create co-ordinate (NB: Values are truncated to 16-bit)
    //! \param[in] x - x co-ordinate
    //! \param[in] y - y co-ordinate
    //////////////////////////////////////////////////////////////////////////////////////////
    Coord(int32 x, int32 y) 
    {
      X = static_cast<int16>(x);
      Y = static_cast<int16>(y);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Coord::Coord
    //! Creates co-ordinate from COORD
    //! \param[in] const &c - Co-ordinate
    //////////////////////////////////////////////////////////////////////////////////////////
    Coord(const COORD& c) 
    {
      X = c.X;
      Y = c.Y;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Coord::Coord
    //! Creates co-ordinate from POINT
    //! \param[in] const &p - Point defining co-ordinate
    //////////////////////////////////////////////////////////////////////////////////////////
    Coord(const POINT& p)
    {
      X = static_cast<int16>(p.x);
      Y = static_cast<int16>(p.y);
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // Coord::operator+
    //! Add two co-ordinates together
    //! 
    //! \param[in] const &p - Point defining co-ordinate to add
    //! \return Coord - Sum of both points
    //////////////////////////////////////////////////////////////////////////////////////////
    Coord operator+(const Coord& r) const
    {
      return Coord(X+r.X, Y+r.Y);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // Coord::operator+=
    //! Add co-ordinate to another
    //! 
    //! \param[in] const &p - Point defining co-ordinate to add
    //! \return Coord - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    Coord& operator+=(const Coord& r) 
    {
      X += r.X;
      Y += r.Y;
      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------

  };
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Console - Provides a debugging console attached to std out 
  //! 
  //! \remarks Provides support for coloured text and cursor control.
  //! \remarks This object is designed to be thread-unsafe for performance reasons. 
  //! \remarks It is the responsibility of the client lock the console appropriately
  //! \remarks in a multi-threaded setting. Console::lock_t provides a RAII lock wrapper
  //////////////////////////////////////////////////////////////////////////////////////////
  struct Console
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  protected:
    //! \var AttributeMask - 
    static constexpr std::underlying_type_t<IoManip> AttributeMask = 0x0000ffff;

    //! \var OutputMask - 
    static constexpr std::underlying_type_t<IoManip> OutputMask = 0xf0000000;                          

  public:
    //! \struct CursorState - Preserves cursor position
    struct CursorState
    {
      //////////////////////////////////////////////////////////////////////////////////////////
      // CursorState::CursorState
      //! Stores the position of the console cursor
      //! 
      //! \param[in,out] &c - Console 
      //////////////////////////////////////////////////////////////////////////////////////////
      CursorState(Console& c) : Owner(c), Position(c.getPosition()), Attributes(c.getAttributes())
      {
      }

      //////////////////////////////////////////////////////////////////////////////////////////
      // CursorState::~CursorState
      //! Restores the position of the console cursor
      //////////////////////////////////////////////////////////////////////////////////////////
      virtual ~CursorState()
      {
        Owner.setPosition(Position);
        Owner.setAttributes(Attributes);
      }

      DISABLE_COPY_ASSIGN(CursorState);

    protected:
      Console& Owner;       //!< Console
      Coord    Position;    //!< Original cursor position
      IoManip  Attributes;  //!< Original console attributes
    };

    //! \typedef delegate_t - Defines signature of output overload
    typedef Console& (*delegate_t)(Console&);
    
    //! \typedef lock_t - Define scoped console lock type
    typedef ConsoleLock  lock_t;
    
    //! \typedef pointer_t - Void pointer 
    typedef void*  pointer_t;

    //! \typedef mutex_t - Define mutex type 
    typedef std::recursive_mutex  mutex_t;

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::Console
    //! Creates a console attached to standard output
    //! 
    //! \throw wtl::domain_error - [Debug only] Unable to retrieve std out
    //////////////////////////////////////////////////////////////////////////////////////////
    Console() : Handle(::GetStdHandle(STD_OUTPUT_HANDLE)), Mode(IoManip::GuiMode)
    {
      try
      {
        // Allocate console
        if (!AllocConsole())
          throw platform_error(HERE, "Unable to allocate console");
        
        // Get std out
        if ((Handle = ::GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
          throw platform_error(HERE, "Unable to get handle to standard out");
        else if (!Handle)
          throw domain_error(HERE, "Standard output unavailable");
        
        // Adjust console size
        SetConsoleScreenBufferSize(Handle, Coord(150,3000));

        // Maximize window
        /*if (HWND wnd = ::GetConsoleWindow())
          ::ShowWindow(wnd, SW_HIDE);*/
        
        // Hide window
        if (HWND wnd = ::GetConsoleWindow())
          ::ShowWindow(wnd, SW_SHOW);

        // Feedback
        Write("Console attached to process");
      }
      catch (exception& e)
      {
        errorBox(::GetDesktopWindow(), c_arr("Debug Console"), e);
      }
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::~Console
    //! Detaches the console
    //////////////////////////////////////////////////////////////////////////////////////////
    ~Console()
    {
      Write("Detaching Console from process");
      ::FreeConsole();
    }
    
    //DISABLE_COPY(Console);     //!< Mutex can't be copied

    // -------------------------- STATIC --------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::getAttributes const
    //! Get the current text attributes
    //! 
    //! \return IoManip - Text attributes
    //////////////////////////////////////////////////////////////////////////////////////////
    IoManip getAttributes() const
    {
      CONSOLE_SCREEN_BUFFER_INFO sb;
      
      // Query screen buffer
      GetConsoleScreenBufferInfo(Handle, &sb);
      return Mode | sb.wAttributes; 
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::getPosition const
    //! Get the cursor position
    //! 
    //! \return Coord - Cursor position in cartesian co-ordinates
    //////////////////////////////////////////////////////////////////////////////////////////
    Coord getPosition() const
    {
      CONSOLE_SCREEN_BUFFER_INFO sb;

      // Query screen buffer
      GetConsoleScreenBufferInfo(Handle, &sb);
      return sb.dwCursorPosition;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::flush 
    //! Flushes the log-file output to disc
    //////////////////////////////////////////////////////////////////////////////////////////
    void flush();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::lock 
    //! Locks the console, blocking if owned by another thread. 
    //! This method can be called by the thread that owns the lock.
    //////////////////////////////////////////////////////////////////////////////////////////
    void lock()
    {
      Mutex.lock();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::Log
    //! Print exception to console, optionally with a message
    //! 
    //! \param[in] const *location - Handler location
    //! \param[in] const &e - Exception
    //! \param[in] const *message - Message
    //////////////////////////////////////////////////////////////////////////////////////////
    void log(const char* location, const std::exception& e);
    void log(const char* location, const wtl::exception& e);
    void log(const char* location, const std::exception& e, const char* message);
    void log(const char* location, const wtl::exception& e, const char* message);
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::setAttributes 
    //! Set the current text attributes
    //! 
    //! \param[in] attr - Text attributes
    //////////////////////////////////////////////////////////////////////////////////////////
    void setAttributes(IoManip attr) 
    {
      SetConsoleTextAttribute(Handle, static_cast<uint16>(attr & AttributeMask)); 
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::setPosition 
    //! Set the cursor position
    //! 
    //! \param[in] const &c - Cursor position in cartesian co-ordinates
    //////////////////////////////////////////////////////////////////////////////////////////
    void setPosition(const Coord& c) 
    {
      SetConsoleCursorPosition(Handle, c);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::setMode
    //! Sets the output mode of the console
    //! 
    //! \param[in] m - Output mode
    //////////////////////////////////////////////////////////////////////////////////////////
    void setMode(IoManip m)
    {
      Mode = m & OutputMask;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::unlock 
    //! Releases ownership of the console lock.
    //! This releases ownership iff an equal number of calls to lock() has been reached.
    //////////////////////////////////////////////////////////////////////////////////////////
    void unlock()
    {
      Mutex.unlock();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::Write
    //! Write character to console
    //! 
    //! \param[in] ch - Character
    //////////////////////////////////////////////////////////////////////////////////////////
    void Write(char val)
    {
      char str[2] {val, '\0'};
      
      // Write
      Write(str);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::Write
    //! Write string at current position with current attributes
    //! 
    //! \param[in] *str - String
    //////////////////////////////////////////////////////////////////////////////////////////
    void Write(const char* str)
    {
      ulong32 bytes = 0;
      
      // Write '(null)' for nullptr
      if (!str)
        str = "(null)";

      // Write to std-out
      if (Mode & IoManip::GuiMode)
        ::WriteConsoleA(Handle, str, strlen(str), &bytes, nullptr);

      // Write to disc
      if (Mode & IoManip::LogMode)
        log(str);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::Writef
    //! Write formatted text
    //! 
    //! \param[in] const *format - Formatting string. If nullptr the results are undefined.
    //////////////////////////////////////////////////////////////////////////////////////////
    void Writef(const char* format, ...)
    {
      char text[1024];
      va_list args;
      
      // Populate string + Write
      va_start(args, format);
      vsnprintf(text, 1024, format, args);
      Write(text);
    }

  protected:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::log
    //! Write string to log with current attributes
    //! 
    //! \param[in] *str - String
    //////////////////////////////////////////////////////////////////////////////////////////
    void log(const char* str);

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HANDLE   Handle;     //!< Console handle
    IoManip  Mode;       //!< Output mode
    mutex_t  Mutex;      //!< Mutex
  };

//! \ifnot WIN32 - Not implemented
#else
  #error Console not implemented
#endif

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ConsoleLock - Provides a RAII wrapper for locking the debug console 
  //! 
  //! \remarks Ownership of the lock is accquired when the object is created, and
  //! \remarks released when the object is destroyed. The c-tor blocks until the
  //! \remarks lock can be accquired.
  //////////////////////////////////////////////////////////////////////////////////////////
  struct ConsoleLock
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \typedef lock_t - Define scoped console lock type
    typedef std::lock_guard<Console>  lock_t;
    
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // ConsoleLock::ConsoleLock
    //! Attempts to create the wrapper, blocking until the lock is accquired
    //////////////////////////////////////////////////////////////////////////////////////////
    ConsoleLock(Console& c) : Lock(c), Output(c)
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    // ConsoleLock::~ConsoleLock
    //! Releases ownership of the lock
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~ConsoleLock() 
    {
      // Flush output
      Output.flush();
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // ConsoleLock::operator << 
    //! Writes any type to the output console
    //!
    //! \tparam T - Any type
    //!
    //! \param[in] const& obj - Value
    //! \return ConsoleLock& : Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    ConsoleLock& operator<< (const T& obj)
    {
      Output << obj;
      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    Console&   Output;      //!< Output console
    lock_t     Lock;        //!< Scoped lock whose lifetime mirrors the wrapper
  };
  

  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes any type to a console via its lock
  //!
  //! \tparam T - Any type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] const &p - Pair containing an array and delimiter character respectively
  //! \return Console& : Reference to console 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  /*template <typename T>
  ConsoleLock& operator<< (ConsoleLock& lock, const T& obj) { return lock << obj; }*/
  
  
  // --------------------------- TRAITS --------------------------
  
  //! \val cdebug - Debug console stream
  extern Console cdebug;
  
  //! \def cdebug_s - Instantiates an anonymous console lock instance, attached to cdebug
  #define cdebug_s  (::wtl::ConsoleLock(::wtl::cdebug) << Cons::Reset)

  
  // --------------------------- OVERLOADS --------------------------
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes array elements to the debug console using a custom delimiter character 
  //!
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] const& range - Delimited input range
  //! \return Console& : Reference to console 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename ITERATOR>
  Console& operator<< (Console& c, const delimited_range_t<ITERATOR>& range)
  {
    return delimit_range(c, range.First, range.Last, range.Delimiter);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Writes a string to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const* str - String
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, const char* str)
  { 
    c.Write(str);
    return c;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Writes an address to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const* ptr - Address
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, const void* ptr)
  { 
    c.Writef("0x%08X", ptr); 
    return c;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Writes a STL string to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& str - String
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, const std::string& str)
  { 
    c.Write(str.c_str());
    return c;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Writes a boolean to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] val - Boolean
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, bool val)
  { 
    c.Write(val ? "true" : "false"); 
    return c;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Writes any integral or floating point type to the debugging console
  //! 
  //! \tparam U - Value type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] val - Integral/floating-point value
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename U>
  std::enable_if_t<(std::is_integral<U>::value || std::is_floating_point<U>::value) 
                && !std::is_same<U,bool>::value, Console&>
  /*Console&*/ operator<< (Console& c, U val)
  {
    c.Writef(format_spec_t<char,U>::value, val);
    return c;
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Invoke console manipulator
  //! 
  //! \param[in] e - Manipulator
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, Cons e)
  {
    switch (e)
    {
    // Output
    case Cons::GuiMode: 
    case Cons::LogMode: c.setMode(e); return c;

    // ENDL/Break/Reset
    case Cons::Endl:    return c << Cons::Reset << Cons::Break;
    case Cons::Break:   return c << "\n";
    case Cons::Reset:   c.setAttributes(Cons::White);  return c;

    // Heading
    case Cons::Heading: return c << Cons::Endl << (Cons::Cyan);

    // Success/Failure
    case Cons::Success: return c <<       Cons::Green      << "Success"  << Cons::White;
    case Cons::Failure: return c << (Cons::Bold|Cons::Red) << "Failed"   << Cons::White;

    // Error/Warning
    case Cons::Error:   return c << (Cons::Bold|Cons::Red)    << "ERROR: "   << Cons::White;
    case Cons::Warning: return c << (Cons::Bold|Cons::Yellow) << "WARNING: " << Cons::White;

    // Formatting
    default:  
      c.setAttributes(e);  return c;
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes any enumeration to the debugging console
  //! 
  //! \tparam E - Enumeration type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] e - Enumeration value
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename E>
  enable_if_enum_t<E,Console&> operator << (Console& c, E e)
  { 
    return c << toString(e);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // std::endl
  //! Convenience overload enabling use of endl instead of Console::Endl 
  //!
  //! \param[in,out] &c - Debugging console
  //! \return Console& - Reference to input console
  //////////////////////////////////////////////////////////////////////////////////////////
	inline Console& endl(Console& c)
	{	
    return c << Cons::Endl;
	}

  //////////////////////////////////////////////////////////////////////////////////////////
  // Console::operator <<
  //! Executes a unary console stream operator overload
  //! 
  //! \param[in,out] Console& c - Console
  //! \param[in] *fn - Function overload
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, Console::delegate_t fn)  
  { 
    return fn(c); 
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Console::Log
  //! Print an STL or WTL exception to console (without custom message)
  //! 
  //! \param[in] const* location - Handler location
  //! \param[in] const& e - Exception
  //////////////////////////////////////////////////////////////////////////////////////////
  inline void Console::log(const char* location, const std::exception& e)
  {
    lock_t lock(*this);   //!< Take ownership of the console

    // Always check whether input is a WTL exception -- simplifies exception guard logic if we only catch one exception type
    if (dynamic_cast<const std::exception*>(&e))
      log(location, reinterpret_cast<const wtl::exception&>(e));
    else
      *this << Cons::Endl 
            << (Cons::Purple|Cons::Bold) << "EXCEPTION: " << Cons::White  << e.what() << "..." << Cons::Endl
            << (Cons::Purple|Cons::Bold) << "SINK: "      << Cons::Yellow << location          << Cons::Endl;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Console::Log
  //! Print WTL exception to console (without custom message)
  //!
  //! \param[in] const *location - Handler location
  //! \param[in] const &e - Exception
  //////////////////////////////////////////////////////////////////////////////////////////
  inline void Console::log(const char* location, const wtl::exception& e)
  {
    lock_t lock(*this);   //!< Take ownership of the console

    *this << Cons::Endl 
          << (Cons::Purple|Cons::Bold) << "EXCEPTION: " << Cons::White  << e.what()     << "..." << Cons::Endl
          << (Cons::Purple|Cons::Bold) << "SOURCE: "    << Cons::Yellow << e.location() << "..." << Cons::Endl
          << (Cons::Purple|Cons::Bold) << "SINK: "      << Cons::Yellow << location     << "..." << Cons::Endl;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Console::Log
  //! Print an STL or WTL exception to console (with a custom message)
  //! 
  //! \param[in] const *location - Handler location
  //! \param[in] const &e - STL or WTL exception
  //! \param[in] const *message - Custom message
  //////////////////////////////////////////////////////////////////////////////////////////
  inline void Console::log(const char* location, const std::exception& e, const char* message)
  {
    lock_t lock(*this);   //!< Take ownership of the console

    // Always check whether input is a WTL exception -- simplifies exception guard logic if we only catch one exception type
    if (dynamic_cast<const std::exception*>(&e))
      log(location, reinterpret_cast<const wtl::exception&>(e));
    else
      *this << Cons::Endl 
            << (Cons::Purple|Cons::Bold) << "EXCEPTION: " << Cons::White  << message << ": " << e.what() << "..." << Cons::Endl
            << (Cons::Purple|Cons::Bold) << "SINK: "      << Cons::Yellow << location                    << "..." << Cons::Endl;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // Console::Log
  //! Print WTL exception to console (with a custom message)
  //!
  //! \param[in] const *location - Handler location
  //! \param[in] const &e - Exception
  //! \param[in] const *message - Custom message
  //////////////////////////////////////////////////////////////////////////////////////////
  inline void Console::log(const char* location, const wtl::exception& e, const char* message)
  {
    lock_t lock(*this);   //!< Take ownership of the console

    *this << Cons::Endl 
          << (Cons::Purple|Cons::Bold) << "EXCEPTION: " << Cons::White  << message << ": " << e.what() << "..." << Cons::Endl
          << (Cons::Purple|Cons::Bold) << "SOURCE: "    << Cons::Yellow << e.location()                << "..." << Cons::Endl
          << (Cons::Purple|Cons::Bold) << "SINK: "      << Cons::Yellow << location                    << "..." << Cons::Endl;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator<<
  //! Writes a path to the debugging console in yellow, then resets the output colour
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& path - Path
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  inline Console& operator<< (Console& c, const Path<ENC>& path)
  { 
    return c << Cons::Yellow << path.c_str() << Cons::Reset;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a name-value pair to the debug console, with the name in yellow and value in white
  //!
  //! \tparam VALUE - Value type
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& value - Name-Value pair representing a named value
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  std::enable_if_t<!std::is_same<VALUE,const char*>::value, Console&>
  /*Console&*/ operator << (Console& c, const NameValuePair<VALUE>& value)
  {
    // Write: Name=Value
    return c << Cons::Yellow << value.Name << '='
             << Cons::White  << value.Value;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a name-value pair to the debug console, with the name in yellow and value in white
  //!
  //! \tparam VALUE - const char*
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const& value - Name-Value pair representing a named value
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename VALUE>
  std::enable_if_t<std::is_same<VALUE,const char*>::value, Console&>
  /*Console&*/ operator << (Console& c, const NameValuePair<VALUE>& value)
  {
    // Write: Name='Value'
    return c << Cons::Yellow << value.Name << "='"
             << Cons::White  << value.Value 
             << Cons::Yellow << "'";
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write integral or enumeration elements of an array to the console
  //!
  //! \tparam D - Array element type
  //! \tparam L - Array capacity
  //! \tparam S - Whether array can be resized
  //! \tparam SF - Ignored
  //!
  //! \param[in,out] &s - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename D, uint32 L, bool S>
  std::enable_if_t<std::is_integral<D>::value || std::is_enum<D>::value, Console&>
  /*Console&*/ operator << (Console& c, const Array<D,L,S>& r) 
  {
    // Write opening tag, size attribute, comma delimited elements, and closing tag.
    return c << debug_info("Array")
             << name_value_pair(" Size", r.size())
             << Cons::Yellow << " Elements=" 
             << Cons::White << delimited_range(r, ',')
             << Cons::Yellow << '}';
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write complex elements of an array to the console
  //!
  //! \tparam D - Array element type
  //! \tparam L - Array capacity
  //! \tparam S - Whether array can be resized
  //! \tparam SF - Ignored
  //!
  //! \param[in,out] &s - Console
  //! \param[in] const &r - Array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename D, uint32 L, bool S>
  std::enable_if_t<!std::is_integral<D>::value && !std::is_enum<D>::value, Console&>
  /*Console&*/ operator << (Console& c, const Array<D,L,S>& r) 
  {
    // Write tag + size
    c << debug_info("Array")
      << name_value_pair(" Size", r.size())
      << endl;

    // Write N index/elements pairs
    for (uint32 i = 0UL, count = r.size(); i < count; ++i) 
      c << name_value_pair(" Index", i)
        << name_value_pair(" Element", r[i])
        << Cons::Break;
    
    // Close
    return c << Cons::Yellow << '}';
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Write a character array to the console
  //!
  //! \tparam ENC - Character encoding 
  //! \tparam LENGTH - Character buffer capacity
  //!
  //! \param[in,out] &s - Console
  //! \param[in] const &r - Character array
  //! \return Console& : Reference to console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned LENGTH>
  Console& operator<<(Console& c, const CharArray<ENC,LENGTH>& r) 
  {
    // Write text and length
    return c << debug_info("CharArray", name_value_pair("Text", r.c_str()), 
                                        name_value_pair("Length", r.size()) );
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints the valid bits of a BitSet to the debugging console
  //!
  //! \tparam DATA - Bitset data type
  //! \param[in,out] &c - Debugging console
  //! \param[in] const &bs - Bitset 
  //! \return Console& - Reference to input console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA>
  Console& operator<<(Console& c, const Bitset<DATA>& bs)
  {
    Bitset<DATA>::BitArray bits;    //!< Storage for bit indicies

    // Open
    c << debug_info("Bitset");

    // Print comma separated zero-based indicies of high-bits
    if (!bs.empty())
      c << Cons::White << delimited_range(bs.flatten(bits), ',');
    
    // Close
    return c << Cons::Yellow << '}';
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct info_unpacker - Writes a name/value pair tuple to an output stream
  //! 
  //! \tparam IDX - Zero-based iteration index
  //! \tparam ARGS - Name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, unsigned COUNT, typename... ARGS>
  struct info_unpacker
  {
    //////////////////////////////////////////////////////////////////////////////////////////
	  //! info_unpacker::unpack
    //! Unpacks attributes and writes them as a sequence of space-delimited name/value pairs
    //! 
    //! \param[in,out] &c - Console
    //! \param[in] const& info - Debug info
    //////////////////////////////////////////////////////////////////////////////////////////
    static void unpack(Console& c, const DebugInfo<ARGS...>& info)
    {
      c << ' ' << std::get<IDX>(info.Attributes);
      info_unpacker<IDX+1,COUNT,ARGS...>::unpack(c, info);
    }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct info_unpacker - Base case 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned COUNT, typename... ARGS>
  struct info_unpacker<COUNT, COUNT, ARGS...>
  {
    static void unpack(Console& c, const DebugInfo<ARGS...>& info)
    { /*no-op*/ }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes object debug info to the debug console
  //!
  //! \tparam ARGS... - Attribute name/value pair types
  //! 
  //! \param[in,out] &c - Console
  //! \param[in] const& info - Debug info
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  std::enable_if_t<DebugInfo<ARGS...>::HasAttributes, Console&>
  /*Console&*/ operator << (Console& c, const DebugInfo<ARGS...>& info)
  {
    static_assert(sizeof...(ARGS) != 0, "Invalid parameter pack size");

    // Open tag
    c << Cons::Yellow << '{';
    
    // Write attributes
    c << Cons::White  << info.Name << ':';
    info_unpacker<0,sizeof...(ARGS),ARGS...>::unpack(c, info);

    // Close
    return c << Cons::Yellow << '}';
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes the opening tag of object debug info to the debug console
  //!
  //! \tparam ARGS... - Empty pack
  //! 
  //! \param[in,out] &c - Console
  //! \param[in] const& info - Debug info
  //! \return Console& : Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  std::enable_if_t<!DebugInfo<ARGS...>::HasAttributes, Console&>
  /*Console&*/ operator << (Console& c, const DebugInfo<ARGS...>& info)
  {
    static_assert(sizeof...(ARGS) == 0, "Invalid parameter pack size");

    // Write opening tag
    c << Cons::Yellow << '{';
    return c << Cons::White  << info.Name << ':';
  }

} //namespace std

#endif // WTL_CONSOLE_HPP


