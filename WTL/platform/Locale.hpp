//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Locale.hpp
//! \brief Encapsulates platform locale identifiers
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LOCALE_HPP
#define WTL_LOCALE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LanguageId - Encapsulates a Win32 language identifier (::LANGID)
  /////////////////////////////////////////////////////////////////////////////////////////
  struct LanguageId 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var Neutral - Define neutral language id
    static const LanguageId   Neutral;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::LANGID   Value;     //!< Language identifier

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::LanguageId constexpr
    //! Create from primary and sub language
    //! 
    //! \param[in] pri - Primary language
    //! \param[in] sub - Sub language
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LanguageId(uint8 pri, uint8 sub) noexcept : Value(MAKELANGID(pri,sub)) 
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::LanguageId constexpr
    //! Create from Win32 language identifier
    //! 
    //! \param[in] lang - Win32 Language identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LanguageId(::LANGID lang) noexcept : Value(lang) 
    {}
    
	  // -------------------------------- COPY & MOVE SEMANTICS -------------------------------

    DEFAULT_COPY(LanguageId);
    DEFAULT_MOVE(LanguageId);

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::sublang constexpr
    //! Query the sublanguage
    //! 
    //! \return uint16 - Sublanguage identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr uint16 sublang() const noexcept
    { 
      return SUBLANGID(Value);     
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::primary constexpr
    //! Query the primary language
    //! 
    //! \return uint16 - Primary language identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr uint16 primary() const noexcept
    { 
      return PRIMARYLANGID(Value);     
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::operator ::LANGID constexpr
    //! Implicit user conversion to Win32 language identifier
    //! 
    //! \return uint16 - Win32 language identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr operator ::LANGID() const noexcept
    { 
      return Value;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias SortOrderId - Win32 Sort order identifier
  /////////////////////////////////////////////////////////////////////////////////////////
  using SortOrderId = uint16;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LocaleId - Encapsulates a Win32 locale identifier (LCID)
  /////////////////////////////////////////////////////////////////////////////////////////
  struct LocaleId
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    LCID Value;       //!< Win32 locale identifier

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocaleId::LocaleId constexpr
    //! Create from language and sort order
    //! 
    //! \param[in] lang - Language
    //! \param[in] sort - Sort order
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LocaleId(LanguageId lang, SortOrderId sort) : Value(MAKELCID(lang,sort)) 
    {}

    // -------------------------------- COPY & MOVE SEMANTICS -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocaleId::language constexpr
    //! Query the language
    //! 
    //! \return LanguageId - Sublanguage identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LanguageId  language() const noexcept
    { 
      return LANGIDFROMLCID(Value);     
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // LocaleId::order constexpr
    //! Query the sort order
    //! 
    //! \return SortOrderId - Primary language identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr SortOrderId  order() const noexcept
    { 
      return SORTIDFROMLCID(Value);     
    }

    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocaleId::operator ::LCID constexpr
    //! Implicit user conversion to Win32 locale identifier
    //! 
    //! \return LCID - Win32 locale identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    operator LCID() const
    {
      return Value;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------


  };
  
  
} //namespace wtl
#endif // WTL_LOCALE_HPP

