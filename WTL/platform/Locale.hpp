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
  
    //! \var <various> - Define various language ids
    static const LanguageId   Neutral,
                              Invariant,
                              English,
                              Chinese,
                              French,
                              Spanish,
                              Welsh,
                              Zulu;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::LANGID   Value;     //!< Language identifier

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::LanguageId constexpr
    //! Create from primary and sub language
    //! 
    //! \param[in] pri - Primary language
    //! \param[in] sub - Sub language
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LanguageId(uint8_t pri, uint8_t sub) noexcept : Value(MAKELANGID(pri,sub)) 
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::LanguageId constexpr
    //! Create from Win32 language identifier
    //! 
    //! \param[in] lang - Win32 Language identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LanguageId(::LANGID lang) noexcept : Value(lang) 
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(LanguageId);      //!< Can be deep copied
    ENABLE_MOVE(LanguageId);      //!< Can be moved 
    DISABLE_POLY(LanguageId);      //!< Cannot be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::sublang constexpr
    //! Query the sublanguage
    //! 
    //! \return uint16_t - Sublanguage identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr uint16_t sublang() const noexcept
    { 
      return SUBLANGID(Value);     
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::primary constexpr
    //! Query the primary language
    //! 
    //! \return uint16_t - Primary language identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr uint16_t primary() const noexcept
    { 
      return PRIMARYLANGID(Value);     
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // LanguageId::operator ::LANGID constexpr
    //! Implicit user conversion to Win32 language identifier
    //! 
    //! \return uint16_t - Win32 language identifier
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
  using SortOrderId = uint16_t;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LocaleId - Encapsulates a Win32 locale identifier (::LCID)
  /////////////////////////////////////////////////////////////////////////////////////////
  struct LocaleId
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var <various> - Define various locale ids
    static const LocaleId   Neutral,      //!< Neutral
                            Invariant,    //!< Invariant
                            User,         //!< User locale
                            System;       //!< System locale

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::LCID Value;       //!< Win32 locale identifier

    // ------------------------------------ CONSTRUCTION ------------------------------------
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
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LocaleId::LocaleId constexpr
    //! Create from Win32 locale identifier
    //! 
    //! \param[in] locale - Win32 locale identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr LocaleId(::LCID locale) : Value(locale) 
    {}
    
	  // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(LocaleId);      //!< Can be deep copied
    ENABLE_MOVE(LocaleId);      //!< Can be moved 
    DISABLE_POLY(LocaleId);     //!< Cannot be polymorphic

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
    // LocaleId::operator ::::LCID constexpr
    //! Implicit user conversion to Win32 locale identifier
    //! 
    //! \return ::LCID - Win32 locale identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    operator ::LCID() const
    {
      return Value;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  
} //namespace wtl
#endif // WTL_LOCALE_HPP

