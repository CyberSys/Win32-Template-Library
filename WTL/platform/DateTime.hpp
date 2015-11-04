//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\DateTime.hpp
//! \brief Encapsulates date & time functionality
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DATE_TIME_HPP
#define WTL_DATE_TIME_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/BaseCast.hpp"           //!< base_cast
#include "wtl/platform/Locale.hpp"          //!< LocaleId
#include "wtl/platform/SystemFlags.hpp"     //!< DateFlags
#include "wtl/traits/EncodingTraits.hpp"    //!< Encoding
#include "wtl/utils/CharArray.hpp"          //!< CharArray
#include "wtl/utils/Zero.hpp"               //!< Zero

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct DateTime - Encapsulates date/time functions
  /////////////////////////////////////////////////////////////////////////////////////////
  struct DateTime : ::SYSTEMTIME
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = ::SYSTEMTIME;

    //! \alias type - Define own type
    using type = DateTime;

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------- CONSTRUCTION -----------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // DateTime::DateTime
    //! Create with current system time
    /////////////////////////////////////////////////////////////////////////////////////////
    /*DateTime() : DateTime(now())
    {
    }*/
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DateTime::DateTime
    //! Create from Win32 system time
    /////////////////////////////////////////////////////////////////////////////////////////
    DateTime(const ::SYSTEMTIME& tm) : base(tm)
    {}

	  // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    ENABLE_COPY(DateTime);    //!< Can be copied  (No move optimization)
    ENABLE_POLY(DateTime);    //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // DateTime::now
    //! Retrieves the current system time
    //! 
    //! \return DateTime - Current system time
    /////////////////////////////////////////////////////////////////////////////////////////
    static DateTime  now()
    {
      ::SYSTEMTIME t;

      // Query the current system time
      ::GetSystemTime(&t);
      return DateTime(t);
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DateTime::format const
    //! Formats the current date and time
    //! 
    //! \tparam ENC - Character encoding
    //! \tparam LEN - Output buffer capacity
    //! 
    //! \param[in,out] &txt - Output buffer
    //! \param[in] const *str - Formatting string
    //! \param[in] locale - [optional] Custom locale
    //!
    //! \throw wtl::platform_error - Unable to format date
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    void  format(CharArray<ENC,LEN>& txt, const encoding_char_t<ENC>* str, LocaleId locale = LocaleId::Neutral) const
    {
      // Format date according to user settings
      if (!WinAPI<ENC>::getDateFormat(locale, enum_cast(zero<DateFlags>()), base_cast(this), str, txt.buffer(), LEN))
        throw platform_error(HERE, "Unable to format date");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DateTime::format const
    //! Formats the current date and time
    //! 
    //! \param[in,out] &txt - Output buffer
    //! \param[in] flags - Formatting flags
    //! \param[in] locale - [optional] Custom locale
    //!
    //! \throw wtl::platform_error - Unable to format date
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    void  format(CharArray<ENC,LEN>& txt, DateFlags flags, LocaleId locale = LocaleId::Neutral) const
    { 
      // Format date according to user settings
      if (!WinAPI<ENC>::getDateFormat(locale, enum_cast(flags), base_cast(this), nullptr, txt.buffer(), LEN))
        throw platform_error(HERE, "Unable to format date");
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LongDateString - Character array containing the current date in long format
  //!
  //! \tparam ENC - Character encoding
  //! \tparam LEN - Output buffer capacity
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned LEN = 128>
  struct LongDateString : CharArray<ENC,LEN>
  {
    using base = CharArray<ENC,LEN>;
    using type = LongDateString<ENC,LEN>;

    LongDateString()
    {
      DateTime tm(DateTime::now());
      tm.format(*this, DateFlags::LongDate);    //format(L"%d/%m/%Y %I:%M%p")
    }
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ShortDateString - Character array containing the current date in short format
  //!
  //! \tparam ENC - Character encoding
  //! \tparam LEN - Output buffer capacity
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned LEN = 128>
  struct ShortDateString : CharArray<ENC,LEN>
  {
    using base = CharArray<ENC,LEN>;
    using type = ShortDateString<ENC,LEN>;

    ShortDateString()
    {
      DateTime tm(DateTime::now());
      tm.format(*this, DateFlags::ShortDate);    //format(L"%d/%m/%Y %I:%M%p")
    }
  };
}



#endif // WTL_DATE_TIME_HPP


