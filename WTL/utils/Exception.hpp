//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Exception.hpp
//! \brief Provides exceptions with a source and error message
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EXCEPTION_HPP
#define WTL_EXCEPTION_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct exception - Adds source information to an STL exception
  /////////////////////////////////////////////////////////////////////////////////////////
  struct exception : std::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef std::exception base;

    //! \typedef location_string_t - Location string type
    using location_string_t = char [1024];

    //! \typedef error_string_t - Error message string type
    using error_string_t = char [16*1024];

    // ----------------------- CONSTRUCTION -----------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::exception
    //! Creates an exception from a location only
    //! 
    //! \param[in] const* location - Location
    /////////////////////////////////////////////////////////////////////////////////////////
    exception(const char* location) : base(Message)
    { 
      // Copy location 
      strncpy(Location, location, sizeof(Location));
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
    exception(const char* location, const char* format, ARGS&&... args) : exception(location)
    {
      // Format message 
      snprintf_t(Message, sizeof(Message), format, std::forward<ARGS>(args)...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // exception::~exception
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~exception()
    {
    }

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
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
    const char* what() const override 
    {
      return Message;
    }

    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    location_string_t  Location;    //!< Source of throw
    error_string_t     Message;     //!< Error message
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct domain_error - Thrown when a domain invariant is violated
  /////////////////////////////////////////////////////////////////////////////////////////
  struct domain_error : wtl::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ----------------------- CONSTRUCTION -----------------------
    
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

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
  };
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct logic_error - Thrown when a logic invariant is violated
  /////////////////////////////////////////////////////////////////////////////////////////
  struct logic_error : wtl::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ----------------------- CONSTRUCTION -----------------------
    
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

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct invalid_argument - Thrown when an argument is missing or invalid
  /////////////////////////////////////////////////////////////////////////////////////////
  struct invalid_argument : wtl::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ----------------------- CONSTRUCTION -----------------------
    
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

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct length_error - Thrown when a capacity is exceeded
  /////////////////////////////////////////////////////////////////////////////////////////
  struct length_error : wtl::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ----------------------- CONSTRUCTION -----------------------
    
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

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
  };
  
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct out_of_range - Thrown when a value occurs outside of a defined boundary
  /////////////////////////////////////////////////////////////////////////////////////////
  struct out_of_range : wtl::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ----------------------- CONSTRUCTION -----------------------
    
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

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
  };
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct platform_error - Thrown when a value occurs outside of a defined boundary
  /////////////////////////////////////////////////////////////////////////////////////////
  struct platform_error : wtl::exception
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  
    //! \typedef base - Base class type
    typedef wtl::exception base;

    // ----------------------- CONSTRUCTION -----------------------
    
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
      int32 n = snprintf_t(Message, sizeof(Message), format, std::forward<ARGS>(args)...);

      if (Code != 0)
      {
        char  strError[1024];
      
        // Lookup and append system error
        ::FormatMessageA(enum_cast(FormatMessageFlags::FromSystem|FormatMessageFlags::IgnoreInserts), nullptr, Code, 0UL, strError, sizeof(strError), nullptr);
        snprintf_t(&Message[n-1], sizeof(Message)-n, ". %s.", strError);
      }
    }

	  // ------------------------- STATIC ---------------------------
  
    // ------------------------ ACCESSORS -------------------------
    
    // ------------------------- MUTATORS -------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    DWORD   Code;     //!< System error code
  };


  
} // WTL namespace



#endif // WTL_EXCEPTION_HPP

#pragma once
