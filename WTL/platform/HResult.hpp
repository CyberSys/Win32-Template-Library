//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\HResult.hpp
//! \brief Encapsulates the ::HRESULT type and simplifies its usage
//! \date 6 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_HRESULT_HPP
#define WTL_HRESULT_HPP

#include <wtl/WTL.hpp>
#include <wtl\platform\WindowFlags.hpp>       //!< FormatMessageFlags
#include <wtl\utils\String.hpp>               //!< String
#include <wtl\utils\Exception.hpp>            //!< platform_error
#include <wtl\utils\LengthOf.hpp>             //!< lengthof

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct HResult - Encapsulates the ::HRESULT type
  /////////////////////////////////////////////////////////////////////////////////////////
  struct HResult
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = HResult;
	
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::HRESULT   Value;      //!< Value

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // HResult::HResult
    //! Create from ::HRESULT
    /////////////////////////////////////////////////////////////////////////////////////////
    HResult(::HRESULT hr) : Value(hr)
    {
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
		ENABLE_COPY(HResult);			//!< Can be copied
		ENABLE_MOVE(HResult);			//!< Can be moved
		DISABLE_POLY(HResult);	  //!< Cannot be polymorphic 
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // HResult::success const
    //! Query whether value indicates operation succeeded
    //! 
    //! \return bool - True iff value indicates operation succeeded
    /////////////////////////////////////////////////////////////////////////////////////////
    bool success() const
    {
      return SUCCEEDED(Value);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // HResult::check const
    //! Throw this error as an exception
    //! 
    //! \throw wtl::platform_error - Value indicates the operation failed
    /////////////////////////////////////////////////////////////////////////////////////////
    /*void check() const
    {
      if (FAILED(Value))
        throw platform_error(Value);
    }*/
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // HResult::toString const
    //! Formats the error as a string
    //! 
    //! \tparam ENC - Character encoding
    //!
    //! \return String<ENC> - String representation of value
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    String<ENC> toString() const
    {
      encoding_char_t<ENC> strError[1024];

      // Lookup system error
      if (!WinAPI<ENC>::formatMessage(enum_cast(FormatMessageFlags::FromSystem|FormatMessageFlags::IgnoreInserts), nullptr, Value, 0, strError, lengthof(strError), nullptr))
        return String<ENC>("Unrecognised error code");

      // Assemble exception message and append system error
      return String<ENC>( strError );
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // HResult::operator ! const
    //! Query whether value indicates operation failed
    //! 
    //! \return bool - True iff value indicates operation failed
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator !() const
    {
      return FAILED(Value);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // HResult::operator bool const
    //! Query whether value indicates operation succeeded
    //! 
    //! \return bool - True iff value indicates operation succeeded
    /////////////////////////////////////////////////////////////////////////////////////////
    /*operator bool() const
    {
      return success();
    }*/

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
}



#endif // WTL_HRESULT_HPP


