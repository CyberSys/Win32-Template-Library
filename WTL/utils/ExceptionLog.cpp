//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\ExceptionLog.cpp
//! \brief Implements exception logging
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#include "wtl/WTL.hpp"
#include "wtl/utils/ExceptionLog.hpp"         //!< exception_log

//////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
//////////////////////////////////////////////////////////////////////////////////////////
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // std::exception_log
  //! Create an exception log object
  //!
  //! \param[in] const* sink - Catch block
  //! \param[in] const* message - [optional] Catch message console
  //! \return exception_log_t - Exception log
  //////////////////////////////////////////////////////////////////////////////////////////
  exception_log_t  exception_log(const char* sink, const char* message)
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
  exception_log_t  exception_log(const char* sink, const std::exception& e, const char* message)
	{	
    // Upcast to wtl::exception if possible
    if (auto ptr = dynamic_cast<const exception*>(&e))
      return exception_log_t(sink,*ptr,message);

    // Standard exception
    return exception_log_t(sink,e,message);
	};


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes an exception to the debug console
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

