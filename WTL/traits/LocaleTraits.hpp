////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\LocaleTraits.hpp
//! \brief Defines locale traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LOCALE_TRAITS_HPP
#define WTL_LOCALE_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct LanguageId - Encapsulates a Win32 language identifier (LANGID)
  ///////////////////////////////////////////////////////////////////////////////
  struct LanguageId 
  {
    LanguageId() : LanguageId(LANG_NEUTRAL,SUBLANG_NEUTRAL) {}
    LanguageId(uint8 pri, uint8 sub) : Value(MAKELANGID(pri,sub)) {}
    LanguageId(LANGID lang) : Value(lang) {}
    
    uint16 sublang() const    { return SUBLANGID(Value);     }
    uint16 primary() const    { return PRIMARYLANGID(Value); }
    operator LANGID() const   { return Value;                }

  protected:
    LANGID Value;
  };


  //! \alias SortOrderId - Win32 Sort order identifier
  using SortOrderId = uint16;

  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct LocaleId - Encapsulates a Win32 locale identifier (LCID)
  ///////////////////////////////////////////////////////////////////////////////
  struct LocaleId
  {
    LocaleId(LanguageId lang, SortOrderId sort) : Value(MAKELCID(lang,sort)) {}

    LanguageId  language() const  { return LANGIDFROMLCID(Value); }
    SortOrderId sort() const      { return SORTIDFROMLCID(Value); }
    operator LCID() const         { return Value;                 }

  protected:
    LCID Value;
  };
  
  
} //namespace wtl
#endif // WTL_LOCALE_TRAITS_HPP

