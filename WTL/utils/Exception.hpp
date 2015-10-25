//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Exception.hpp
//! \brief Provides exceptions with a source and error message
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EXCEPTION_HPP
#define WTL_EXCEPTION_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"           //!< EnumCast
#include "wtl/platform/WindowFlags.hpp"     //!< FormatMessageFlags
#include <exception>                        //!< std::exception
#include <utility>                          //!< std::forward
#include <array>                            //!< std::array
#include <cstdio>                           //!< std::snprintf

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::error_string
  //! Builds a formatted string (of narrow character type) without performing dynamic memory allocation
  //! 
  //! \tparam LENGTH - Length of character array
  //! 
  //! \param[in] const* format - Formatting string
  //! \param[in] &&... args - [optional] Arguments
  //! \return std::array<char,LENGTH> - Character array containing formatted string
  /////////////////////////////////////////////////////////////////////////////////////////
  template <uint32_t LENGTH, typename... ARGS>
  std::array<char,LENGTH> error_string(const char* format, ARGS&&... args)
  {
    std::array<char,LENGTH>  msg;
    std::snprintf(msg.data(), LENGTH, format, args...);
    return msg;
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct exception - Adds source information to an STL exception
  /////////////////////////////////////////////////////////////////////////////////////////
  struct exception : std::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = std::exception;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    char  Location[1024];     //!< Source of throw
    char  Message[16*1024];   //!< Error message

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::exception
    //! Creates an exception from a location only
    //!
    //! \param[in] const* loc - Location
    /////////////////////////////////////////////////////////////////////////////////////////
    exception(const char* loc)
    {
      // Copy location
      std::snprintf(Location, sizeof(Location), loc);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::exception
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity to format string
    //! \throw wtl::logic_error - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    exception(const char* location, const char* format, ARGS&&... args) 
    {
      // Format message
      std::snprintf(Message, sizeof(Message), format, std::forward<ARGS>(args)...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::~exception
    //! Can be polymorphic
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~exception()
    {
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::location const
    //! Get the error location (source of throw)
    //!
    //! \return const char* - Location string
    /////////////////////////////////////////////////////////////////////////////////////////
    const char* location() const
    {
      return Location;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::what const
    //! Get the error message
    //!
    //! \return const char* - Error string
    /////////////////////////////////////////////////////////////////////////////////////////
    const char* what() const noexcept override
    {
      return Message;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct domain_error - Thrown when a domain invariant is violated
  /////////////////////////////////////////////////////////////////////////////////////////
  struct domain_error : wtl::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = wtl::exception;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // domain_error::domain_error
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity to format string
    //! \throw wtl::logic_error - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    domain_error(const char* location, const char* format, ARGS&&... args) : base(location, format, std::forward<ARGS>(args)...)
    {
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct logic_error - Thrown when a logic invariant is violated
  /////////////////////////////////////////////////////////////////////////////////////////
  struct logic_error : wtl::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = wtl::exception;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // logic_error::logic_error
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity to format string
    //! \throw wtl::logic_error - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    logic_error(const char* location, const char* format, ARGS&&... args) : base(location, format, std::forward<ARGS>(args)...)
    {
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct invalid_argument - Thrown when an argument is missing or invalid
  /////////////////////////////////////////////////////////////////////////////////////////
  struct invalid_argument : wtl::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = wtl::exception;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // invalid_argument::invalid_argument
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::invalid_argument - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity to format string
    //! \throw wtl::invalid_argument - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    invalid_argument(const char* location, const char* format, ARGS&&... args) : base(location, format, std::forward<ARGS>(args)...)
    {
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct length_error - Thrown when a capacity is exceeded
  /////////////////////////////////////////////////////////////////////////////////////////
  struct length_error : wtl::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // length_error::length_error
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::length_error - [Debug only] Missing formatting string
    //! \throw wtl::length_error - Insufficent capacity to format string
    //! \throw wtl::length_error - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    length_error(const char* location, const char* format, ARGS&&... args) : base(location, format, std::forward<ARGS>(args)...)
    {
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct out_of_range - Thrown when a value occurs outside of a defined boundary
  /////////////////////////////////////////////////////////////////////////////////////////
  struct out_of_range : wtl::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = wtl::exception;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // out_of_range::out_of_range
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::out_of_range - [Debug only] Missing formatting string
    //! \throw wtl::out_of_range - Insufficent capacity to format string
    //! \throw wtl::out_of_range - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    out_of_range(const char* location, const char* format, ARGS&&... args) : base(location, format, std::forward<ARGS>(args)...)
    {
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct platform_error - Thrown when a value occurs outside of a defined boundary
  /////////////////////////////////////////////////////////////////////////////////////////
  struct platform_error : wtl::exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias base - Define base type
    using base = wtl::exception;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // platform_error::platform_error
    //! Creates an exception from a location and error message
    //!
    //! \tparam ARGS - Arguments parameter pack type
    //!
    //! \param[in] const* location - Location
    //! \param[in] const* format - Formatting string
    //! \param[in] ...&& args - [optional] Variadic arguments
    //!
    //! \throw wtl::out_of_range - [Debug only] Missing formatting string
    //! \throw wtl::out_of_range - Insufficent capacity to format string
    //! \throw wtl::out_of_range - Incorrect number of arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    platform_error(const char* location, const char* format, ARGS&&... args) : base(location), Code(::GetLastError())
    {
      // Format message
      int32_t n = std::snprintf(Message, sizeof(Message), format, std::forward<ARGS>(args)...);

      if (Code != 0)
      {
        char  strError[1024];

        // Lookup and append system error
        ::FormatMessageA(enum_cast(FormatMessageFlags::FromSystem|FormatMessageFlags::IgnoreInserts), nullptr, Code, 0UL, strError, sizeof(strError), nullptr);
        std::snprintf(&Message[n-1], sizeof(Message)-n, ". %s.", strError);
      }
    }

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

    DWORD   Code;     //!< System error code
  };



} // WTL namespace



#endif // WTL_EXCEPTION_HPP


