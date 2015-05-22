//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\Console.hpp
//! \brief Provides coloured text output to the debugging console
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CONSOLE_HPP
#define WTL_CONSOLE_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"           //!< EnumCast
#include "wtl/casts/NativeCast.hpp"         //!< NativeCast
#include "wtl/traits/EnumTraits.hpp"        //!< is_attribute
#include "wtl/utils/Sequence.hpp"           //!< integral_sequence
#include "wtl/utils/Point.hpp"              //!< Point
#include "wtl/utils/Exception.hpp"          //!< wtl::exception
#include <stdexcept>                        //!< std::exception
#include <mutex>                            //!< std::lock_guard, std::recursive_mutex
#include <type_traits>                      //!< std::enable_if

//! \namespace wtl - Windows template library
namespace wtl
{
  // Forward declaration
  struct ConsoleLock;

  //! \enum IoManip - Console stream manipulators 
  enum class IoManip : uint32
  { 
    Bold   = FOREGROUND_INTENSITY,                 //!< Set bold text
    Black  = 0,                                    //!< Set black text
    Cyan   = FOREGROUND_BLUE|FOREGROUND_GREEN,     //!< Set cyan text
    Blue   = FOREGROUND_BLUE,                      //!< Set blue text
    Green  = FOREGROUND_GREEN,                     //!< Set green text
    Grey   = Bold,                                 //!< Set grey text
    Purple = FOREGROUND_RED|FOREGROUND_BLUE,       //!< Set purple text
    Red    = FOREGROUND_RED,                       //!< Set red text
    Yellow = FOREGROUND_RED|FOREGROUND_GREEN,      //!< Set yellow text
    White  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,     //!< Set white text

    BkBold   = BACKGROUND_INTENSITY,               //!< Set bold background
    BkBlack  = 0,                                  //!< Set black background
    BkCyan   = BACKGROUND_BLUE|BACKGROUND_GREEN,   //!< Set cyan background
    BkBlue   = BACKGROUND_BLUE,                    //!< Set blue background
    BkGreen  = BACKGROUND_GREEN,                   //!< Set green background
    BkGrey   = BkBold,                             //!< Set grey background
    BkPurple = BACKGROUND_RED|BACKGROUND_BLUE,     //!< Set purple background
    BkRed    = BACKGROUND_RED,                     //!< Set red background
    BkYellow = BACKGROUND_RED|BACKGROUND_GREEN,    //!< Set yellow background
    BkWhite  = BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE,   //!< Set white background

    Heading = 0x00010000,      //!< Prints text in cyan
    Success = 0x00020000,      //!< Prints 'Success' in green
    Failure = 0x00040000,      //!< Prints 'Failure' in red
    Error   = 0x00080000,      //!< Prints 'ERROR' in red
    Warning = 0x00100000,      //!< Prints 'WARNING' in blue
    Reset   = 0x00200000,      //!< Reset to default colours
    Endl    = 0x00400000,      //!< LineBreak that resets formatting
    Break   = 0x00800000,      //!< LineBreak that preserves formatting
  };

  //! \alias Cons - Alias for debugging console i/o manipulators
  using Cons = IoManip;

  //! Define traits: Non-contiguous Attribute
  template <> struct is_attribute<Cons>  : std::true_type  {};
  template <> struct is_contiguous<Cons> : std::false_type {};

  //! Define names and values
  //template <> struct enum_names<Cons>  { static const char* names[]; };
  template <> struct enum_values<Cons> : integral_sequence<Cons, Cons::Bold,
                                                                 Cons::Black,
                                                                 Cons::Cyan,
                                                                 Cons::Blue,
                                                                 Cons::Green,
                                                                 Cons::Grey,
                                                                 Cons::Purple,
                                                                 Cons::Red,
                                                                 Cons::Yellow,
                                                                 Cons::White,

                                                                 Cons::BkBold,
                                                                 Cons::BkBlack,
                                                                 Cons::BkCyan,
                                                                 Cons::BkBlue,
                                                                 Cons::BkGreen,
                                                                 Cons::BkGrey,
                                                                 Cons::BkPurple,
                                                                 Cons::BkRed,
                                                                 Cons::BkYellow,
                                                                 Cons::BkWhite,

                                                                 Cons::Heading,
                                                                 Cons::Success,
                                                                 Cons::Failure,
                                                                 Cons::Error,
                                                                 Cons::Warning,
                                                                 Cons::Reset,
                                                                 Cons::Endl,
                                                                 Cons::Break>  {};

  //////////////////////////////////////////////////////////////////////////////////////////
  // \struct Coord - Wrapper for Win32 COORD structure
  //////////////////////////////////////////////////////////////////////////////////////////
  using Coord = Point<int16>;
  
  
  

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
  public:
    //! \alias delegate_t - Defines signature of output overload
    using delegate_t = Console& (*)(Console&);
    
    //! \alias lock_t - Define scoped console lock type
    using lock_t = ConsoleLock;
    
    //! \alias pointer_t - Void pointer 
    using pointer_t = void*;

    //! \alias mutex_t - Define mutex type 
    using mutex_t = std::recursive_mutex;
    
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

      //! \alias lock_t - Define scoped console lock type
      using lock_t = std::lock_guard<Console>;
    
      // ----------------------------------- REPRESENTATION -----------------------------------
    protected:
      Console&   Output;      //!< Output console
      lock_t     Lock;        //!< Scoped lock whose lifetime mirrors the wrapper

      // ------------------------------------ CONSTRUCTION ------------------------------------
    public:
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

      // -------------------------------- COPYING & DESTRUCTION -------------------------------
    
      DISABLE_COPY(ConsoleLock);     //!< Cannot be copied
      ENABLE_MOVE(ConsoleLock);      //!< Can be moved

      // ----------------------------------- STATIC METHODS -----------------------------------

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
      ConsoleLock& operator << (const T& obj)
      {
        Output << obj;
        return *this;
      }
    };
  
    //////////////////////////////////////////////////////////////////////////////////////////
    //! \struct CursorState - Preserves cursor position
    //////////////////////////////////////////////////////////////////////////////////////////
    struct CursorState
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
      // ----------------------------------- REPRESENTATION -----------------------------------
    protected:
      Console& Owner;       //!< Console
      Coord    Position;    //!< Original cursor position
      IoManip  Attributes;  //!< Original console attributes

      // ------------------------------------ CONSTRUCTION ------------------------------------
    public:
      //////////////////////////////////////////////////////////////////////////////////////////
      // CursorState::CursorState
      //! Stores the position of the console cursor
      //! 
      //! \param[in,out] &c - Console 
      //////////////////////////////////////////////////////////////////////////////////////////
      CursorState(Console& c) : Owner(c), Position(c.getPosition()), Attributes(c.getAttributes())
      {
      }
      
      // -------------------------------- COPY, MOVE & DESTROY --------------------------------

      DISABLE_COPY(CursorState);    //!< Cannot be copied
      DISABLE_MOVE(CursorState);    //!< Cannot be moved
      
      //////////////////////////////////////////////////////////////////////////////////////////
      // CursorState::~CursorState
      //! Restores the position of the console cursor
      //////////////////////////////////////////////////////////////////////////////////////////
      virtual ~CursorState()
      {
        Owner.setPosition(Position);
        Owner.setAttributes(Attributes);
      }

      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      // ----------------------------------- MUTATOR METHODS ----------------------------------
    };

  protected:
    //! \var AttributeMask - Bitmask exposing text formatting bits 
    static constexpr std::underlying_type_t<IoManip> AttributeMask = 0x0000ffff;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HANDLE   Handle;     //!< Console handle
    mutex_t  Mutex;      //!< Mutex

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::Console
    //! Creates a console attached to standard output
    //! 
    //! \throw wtl::domain_error - [Debug only] Unable to retrieve std out
    //////////////////////////////////////////////////////////////////////////////////////////
    Console() : Handle(::GetStdHandle(STD_OUTPUT_HANDLE))
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
        SetConsoleScreenBufferSize(Handle, native_cast(Coord(150,3000)));

        // Show window
        if (HWND wnd = ::GetConsoleWindow())
          ::ShowWindow(wnd, SW_SHOW);
      }
      // [ERROR] Unable to attach console
      catch (exception& e)
      {
        ::MessageBoxA(default<::HWND>(), e.what(), "Debug Console", enum_cast(MessageBoxFlags::Ok|MessageBoxFlags::IconError));
      }
    }

    // -------------------------------- COPYING & DESTRUCTION -------------------------------
    
    DISABLE_COPY(Console);     //!< Cannot be copied
    ENABLE_MOVE(Console);      //!< Can be moved
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::~Console
    //! Detaches the console
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Console()
    {
      ::FreeConsole();
    }
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::getAttributes const
    //! Get the current text attributes
    //! 
    //! \return Cons - Text attributes
    //////////////////////////////////////////////////////////////////////////////////////////
    Cons getAttributes() const
    {
      CONSOLE_SCREEN_BUFFER_INFO sb;
      
      // Query screen buffer
      GetConsoleScreenBufferInfo(Handle, &sb);
      return enum_cast<Cons>(sb.wAttributes); 
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
      return native_cast(sb.dwCursorPosition);
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
    // Console::setAttributes 
    //! Set the current text attributes
    //! 
    //! \param[in] attr - Text attributes
    //////////////////////////////////////////////////////////////////////////////////////////
    void setAttributes(IoManip attr) 
    {
      SetConsoleTextAttribute(Handle, enum_cast(attr & AttributeMask)); 
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::setPosition 
    //! Set the cursor position
    //! 
    //! \param[in] const& pt - Cursor position in cartesian co-ordinates
    //////////////////////////////////////////////////////////////////////////////////////////
    void setPosition(const Coord& pt) 
    {
      SetConsoleCursorPosition(Handle, native_cast(pt));
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
    // Console::write
    //! write character to console
    //! 
    //! \param[in] ch - Character
    //////////////////////////////////////////////////////////////////////////////////////////
    void write(char val)
    {
      char str[2] {val, '\0'};
      
      // write
      write(str);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::write
    //! write string at current position with current attributes
    //! 
    //! \param[in] *str - String
    //////////////////////////////////////////////////////////////////////////////////////////
    void write(const char* str)
    {
      ulong32 bytes = 0;
      
      // write '(null)' for nullptr
      if (!str)
        str = "(null)";

      // write to std-out
      ::WriteConsoleA(Handle, str, strlen(str), &bytes, nullptr);

      // write to disc
      log(str);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::writef
    //! write formatted text
    //! 
    //! \param[in] const *format - Formatting string. If nullptr the results are undefined.
    //////////////////////////////////////////////////////////////////////////////////////////
    void writef(const char* format, ...)
    {
      char text[1024];
      va_list args;
      
      // Populate string + write
      va_start(args, format);
      vsnprintf(text, 1024, format, args);
      write(text);
    }

  protected:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Console::log
    //! write string to log with current attributes
    //! 
    //! \param[in] *str - String
    //////////////////////////////////////////////////////////////////////////////////////////
    void log(const char* str);
  };


  //! \val cdebug - Debug console stream
  extern Console cdebug;
  
  //! \def cdebug_s - Instantiates an anonymous console lock attached to cdebug
  #define cdebug_s  (::wtl::ConsoleLock(::wtl::cdebug) << Cons::Reset)

  
  
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
  ConsoleLock& operator << (ConsoleLock& lock, const T& obj) { return lock << obj; }*/
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a string to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const* str - String
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator << (Console& c, const char* str)
  { 
    c.write(str);
    return c;
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes an address to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] const* ptr - Address
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator << (Console& c, const void* ptr)
  { 
    c.writef("0x%08X", ptr); 
    return c;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a boolean to the debugging console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in] val - Boolean
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator << (Console& c, bool val)
  { 
    c.write(val ? "true" : "false"); 
    return c;
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes an integral type to the debugging console
  //! 
  //! \tparam T - Value type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] val - Integral value
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  enable_if_integer_t<T,Console&> operator << (Console& c, T val)
  {
    c.writef(format_spec_t<char,T>::value, val);
    return c;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes any floating-point type to the debugging console
  //! 
  //! \tparam T - Value type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] val - Floating point value
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  enable_if_floating_t<T,Console&> operator << (Console& c, T val)
  {
    c.writef(format_spec_t<char,T>::value, val);
    return c;
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes the string representation of any enumeration type to the debugging console
  //! 
  //! \tparam T - Enumeration type
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] val - Enumeration value
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  enable_if_enum_t<T,Console&> operator << (Console& c, T val)
  { 
    return c << toString(val);
  }
  

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Invoke console manipulator
  //! 
  //! \param[in] e - Manipulator
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator << (Console& c, Cons e)
  {
    switch (e)
    {
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
  inline Console& operator << (Console& c, Console::delegate_t fn)  
  { 
    return fn(c); 
  }

  
} // namespace wtl

#endif // WTL_CONSOLE_HPP


