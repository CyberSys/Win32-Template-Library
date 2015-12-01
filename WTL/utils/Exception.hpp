//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Exception.hpp
//! \brief Provides exceptions with a source and error message
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EXCEPTION_HPP
#define WTL_EXCEPTION_HPP

#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>           //!< EnumCast
#include <wtl/platform/WindowFlags.hpp>     //!< FormatMessageFlags
#include <wtl/io/StreamIterator.hpp>        //!< StreamIterator
#include <wtl/utils/ForEach.hpp>            //!< for_each_t
#include <exception>                        //!< std::exception
#include <stdexcept>                        //!< std::logic_error, std::domain_error, std::length_error, std::out_of_range
#include <utility>                          //!< std::forward
#include <array>                            //!< std::array
#include <string>                           //!< std::string
#include <cstdio>                           //!< std::snprintf
#include <sstream>                          //!< std::ostringstream

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::error_string
  //! Builds a formatted string of narrow character type
  //! 
  //! \tparam ARGS - String stream arguments
  //! 
  //! \param[in] &&... args - Arguments
  //! \return std::string - Formatted string
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  std::string  error_string(ARGS&&... args)
  {
    static_assert(sizeof...(ARGS) > 0, "Missing string arguments");

    std::ostringstream ss;

    // Assemble and return string
    for_each_t(std::tuple<ARGS...>(args...), stream_iterator(ss));
    return ss.str();
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct error_site - Mix-in class providing the source of an exception
  /////////////////////////////////////////////////////////////////////////////////////////
  struct error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    std::string Source;     //!< Source of throw

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // error_site::error_site
    //! Creates from a source-file location string
    //!
    //! \param[in] && loc - String containing source-file location
    /////////////////////////////////////////////////////////////////////////////////////////
    error_site(std::string&& loc) : Source(loc)
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(error_site);        //!< Can be copied
    ENABLE_MOVE(error_site);        //!< Can be moved
    ENABLE_POLY(error_site);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // error_site::where const noexcept
    //! Get the error location (source of throw)
    //!
    //! \return const char* - Location string
    /////////////////////////////////////////////////////////////////////////////////////////
    const char* where() const noexcept
    {
      return Source.c_str();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct caught_exception - Helper class used to extract the source of an exception
  /////////////////////////////////////////////////////////////////////////////////////////
  struct caught_exception
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    const std::string Cause,        //!< Exception error message
                      Problem;      //!< Top-level error message

  protected:
    const std::string Sink;         //!< Source of catch
    const error_site* Source;       //!< Source of throw

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // caught_exception::caught_exception
    //! Creates from a message, exception, and source-file location string
    //!
    //! \param[in] const* msg - Top-level error message
    //! \param[in] && sink - Location of catch
    //! \param[in] const& e - Exception
    /////////////////////////////////////////////////////////////////////////////////////////
    caught_exception(const char* msg, std::string&& sink, const std::exception& e) : Cause(e.what()),
                                                                                     Problem(msg), 
                                                                                     Sink(sink),
                                                                                     Source(dynamic_cast<const error_site*>(&e))
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // caught_exception::caught_exception
    //! Creates from a message and source-file location string. Used for fall-back catch handlers (...)
    //!
    //! \param[in] const* msg - Top-level error message
    //! \param[in] && sink - Location of catch
    /////////////////////////////////////////////////////////////////////////////////////////
    caught_exception(const char* msg, std::string&& sink) : Cause("Unspecified"),
                                                            Problem(msg), 
                                                            Sink(sink),
                                                            Source(nullptr)
    {}

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(caught_exception);        //!< Can be copied
    ENABLE_MOVE(caught_exception);        //!< Can be moved
    ENABLE_POLY(caught_exception);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // caught_exception::sink const noexcept
    //! Get the location where the exception was caught
    //!
    //! \return const char* - Source-file location string
    /////////////////////////////////////////////////////////////////////////////////////////
    std::string message() const noexcept
    {
      return Problem + '\n' + Cause;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // caught_exception::sink const noexcept
    //! Get the location where the exception was caught
    //!
    //! \return const char* - Source-file location string
    /////////////////////////////////////////////////////////////////////////////////////////
    const char* sink() const noexcept
    {
      return Sink.c_str();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // caught_exception::source const noexcept
    //! Get the location where the exception was raised
    //!
    //! \return const char* - Source-file location string
    /////////////////////////////////////////////////////////////////////////////////////////
    const char* source() const noexcept
    {
      return Source ? Source->where() : "Unspecified";
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
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
  std::basic_ostream<CHAR,TRAITS>& operator << (std::basic_ostream<CHAR,TRAITS>& c, const caught_exception& ex);


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct domain_error - Thrown when a domain invariant is violated
  /////////////////////////////////////////////////////////////////////////////////////////
  struct domain_error : std::domain_error, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::domain_error;

    //! \alias site_base - Define location base type
    using site_base = error_site;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // domain_error::domain_error
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    domain_error(std::string&& location, ARGS&&... args) : error_base(error_string(std::forward<ARGS>(args)...)),
                                                           site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(domain_error);        //!< Can be copied
    ENABLE_MOVE(domain_error);        //!< Can be moved
    ENABLE_POLY(domain_error);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct logic_error - Thrown when a logic invariant is violated
  /////////////////////////////////////////////////////////////////////////////////////////
  struct logic_error : std::logic_error, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::logic_error;

    //! \alias site_base - Define location base type
    using site_base = error_site;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // logic_error::logic_error
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    logic_error(std::string&& location, ARGS&&... args) : error_base(error_string(std::forward<ARGS>(args)...)),
                                                          site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(logic_error);        //!< Can be copied
    ENABLE_MOVE(logic_error);        //!< Can be moved
    ENABLE_POLY(logic_error);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct invalid_argument - Thrown when an argument is missing or invalid
  /////////////////////////////////////////////////////////////////////////////////////////
  struct invalid_argument : std::invalid_argument, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::invalid_argument;

    //! \alias site_base - Define location base type
    using site_base = error_site;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // invalid_argument::invalid_argument
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    invalid_argument(std::string&& location, ARGS&&... args) : error_base(error_string(std::forward<ARGS>(args)...)),
                                                               site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(invalid_argument);        //!< Can be copied
    ENABLE_MOVE(invalid_argument);        //!< Can be moved
    ENABLE_POLY(invalid_argument);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct length_error - Thrown when a capacity is exceeded
  /////////////////////////////////////////////////////////////////////////////////////////
  struct length_error : std::length_error, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::length_error;

    //! \alias site_base - Define location base type
    using site_base = error_site;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // length_error::length_error
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    length_error(std::string&& location, ARGS&&... args) : error_base(error_string(std::forward<ARGS>(args)...)),
                                                           site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(length_error);        //!< Can be copied
    ENABLE_MOVE(length_error);        //!< Can be moved
    ENABLE_POLY(length_error);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct out_of_range - Thrown when a value occurs outside of a defined boundary
  /////////////////////////////////////////////////////////////////////////////////////////
  struct out_of_range : std::out_of_range, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::out_of_range;

    //! \alias site_base - Define location base type
    using site_base = error_site;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // out_of_range::out_of_range
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    out_of_range(std::string&& location, ARGS&&... args) : error_base(error_string(std::forward<ARGS>(args)...)),
                                                           site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(out_of_range);        //!< Can be copied
    ENABLE_MOVE(out_of_range);        //!< Can be moved
    ENABLE_POLY(out_of_range);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct runtime_error - Thrown when a runtime error occurs
  /////////////////////////////////////////////////////////////////////////////////////////
  struct runtime_error : std::runtime_error, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::runtime_error;

    //! \alias site_base - Define location base type
    using site_base = error_site;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // runtime_error::runtime_error
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    runtime_error(std::string&& location, ARGS&&... args) : error_base(error_string(std::forward<ARGS>(args)...)),
                                                           site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(runtime_error);        //!< Can be copied
    ENABLE_MOVE(runtime_error);        //!< Can be moved
    ENABLE_POLY(runtime_error);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------

  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct platform_error - Thrown when a value occurs outside of a defined boundary
  /////////////////////////////////////////////////////////////////////////////////////////
  struct platform_error : std::runtime_error, error_site
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias error_base - Define exception base type
    using error_base = std::runtime_error;

    //! \alias site_base - Define location base type
    using site_base = error_site;
    
    // ----------------------------------- REPRESENTATION -----------------------------------

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // platform_error::platform_error
    //! Creates an exception from a location and a stream of arguments used to build an error message
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] && location - Location string
    //! \param[in] ...&& args - Error message stream arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    platform_error(std::string&& location, ARGS&&... args) : error_base( formatMessage(::GetLastError(), std::forward<ARGS>(args)...) ),
                                                             site_base(std::move(location))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(platform_error);        //!< Can be copied
    ENABLE_MOVE(platform_error);        //!< Can be moved
    ENABLE_POLY(platform_error);        //!< Can be polymorphic

	  // ----------------------------------- STATIC METHODS -----------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // platform_error::formatMessage
    //! Creates an exception message combining the exception and system error
    //!
    //! \tparam ARGS - Message stream argument types
    //!
    //! \param[in] code - System error code
    //! \param[in] ...&& args - Error message stream arguments
    //! \return std::string - String containing exception and system error messages
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    std::string  formatMessage(DWORD code, ARGS&&... args)
    {
      char  strError[1024] {};

      // Lookup system error
      if (code != 0) 
      {
        uint32_t n = ::FormatMessageA(enum_cast(FormatMessageFlags::FromSystem|FormatMessageFlags::IgnoreInserts), nullptr, code, 0, strError, sizeof(strError), nullptr);
        strError[n] = '\0';
      }

      // Assemble exception message and append system error
      return error_string(std::forward<ARGS>(args)..., ". ", strError);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };



} // WTL namespace



#endif // WTL_EXCEPTION_HPP


