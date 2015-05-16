//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\ExceptionLog.hpp
//! \brief Encapsulates logging different exception types in a consistent manner
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EXCEPTION_LOG_HPP
#define WTL_EXCEPTION_LOG_HPP

#include "wtl/WTL.hpp"
#include "wtl/io/Console.hpp"         //!< Console

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct exception_log_t - Encapsulates exception meta-data for logging
  //////////////////////////////////////////////////////////////////////////////////////////
  struct exception_log_t
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    const char  *Source,        //!< Throw location
                *Sink,          //!< Catch location
                *Error,         //!< Error 
                *Context;       //!< Context

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // exception_log_t::exception_log_t
    //! Create from no exception 
    //! 
    //! \param[in] const* sink - Catch block
    //! \param[in] const* message - [optional] Catch message
    //////////////////////////////////////////////////////////////////////////////////////////
    exception_log_t(const char* sink, const char* message = nullptr) : Sink(sink), 
                                                                       Source("Unspecified"), 
                                                                       Context(message ? message : "An error has occurred"), 
                                                                       Error("Unknown error")
    {}
    //////////////////////////////////////////////////////////////////////////////////////////
    // exception_log_t::exception_log_t
    //! Create from STL exception 
    //! 
    //! \param[in] const* sink - Catch block
    //! \param[in] const& e - Exception
    //! \param[in] const* message - [optional] Catch message
    //////////////////////////////////////////////////////////////////////////////////////////
    exception_log_t(const char* sink, const std::exception& e, const char* message = nullptr) : Sink(sink), 
                                                                                                Source("Unspecified"), 
                                                                                                Context(message ? message : "An error has occurred"), 
                                                                                                Error(e.what())
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // exception_log_t::exception_log_t
    //! Create from WTL exception 
    //! 
    //! \param[in] const* sink - Catch block
    //! \param[in] const& e - Exception
    //! \param[in] const* message - [optional] Catch message
    //////////////////////////////////////////////////////////////////////////////////////////
    exception_log_t(const char* sink, const exception& e, const char* message = nullptr) : Sink(sink), 
                                                                                           Source(e.location()), 
                                                                                           Context(message ? message : "An error has occurred"), 
                                                                                           Error(e.what())
    {}

    // -------------------------------- COPYING & DESTRUCTION -------------------------------
    
    ENABLE_COPY(exception_log_t);     //!< Can be copied  (No move optimization)
    ENABLE_POLY(exception_log_t);     //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // std::exception_log
  //! Create an exception log object
  //!
  //! \param[in] const* sink - Catch block
  //! \param[in] const* message - [optional] Catch message console
  //! \return exception_log_t - Exception log
  //////////////////////////////////////////////////////////////////////////////////////////
  exception_log_t  exception_log(const char* sink, const char* message = nullptr)
	{	
    // Exceptionless
    return exception_log_t(sink,message);
	};

  //////////////////////////////////////////////////////////////////////////////////////////
  // std::exception_log
  //! Create an exception log object
  //!
  //! \param[in] const* sink - Catch block
  //! \param[in] const& e - Exception
  //! \param[in] const* message - [optional] Catch message console
  //! \return exception_log_t - Exception log
  //////////////////////////////////////////////////////////////////////////////////////////
  exception_log_t  exception_log(const char* sink, const std::exception& e, const char* message = nullptr)
	{	
    // Upcast to wtl::exception if possible
    if (auto ptr = dynamic_cast<const exception*>(&e))
      return exception_log_t(sink,*ptr,message);

    // Standard exception
    return exception_log_t(sink,e,message);
	};


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes an exception to the debugging console
  //!
  //! \param[in,out] &c - Debug console
  //! \param[in] const& - Exception details
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  Console& operator << (Console& c, const exception_log_t& ex)
  {
    return c << Cons::Endl 
             << (Cons::Purple|Cons::Bold) << "EXCEPTION: " << Cons::White  << ex.Context << ": " << ex.Error << "..." << Cons::Endl
             << (Cons::Purple|Cons::Bold) << "SOURCE: "    << Cons::Yellow <<            ex.Source           << "..." << Cons::Endl
             << (Cons::Purple|Cons::Bold) << "SINK: "      << Cons::Yellow <<             ex.Sink            << "..." << Cons::Endl;
  };
  
  

} // WTL namespace



#endif // WTL_EXCEPTION_LOG_HPP


