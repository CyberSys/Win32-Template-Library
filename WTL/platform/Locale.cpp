//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Locale.cpp
//! \brief Defines locale storage
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \var LanguageId::<various> - Initialize common language ids
  const LanguageId  LanguageId::Neutral(LANG_NEUTRAL,SUBLANG_NEUTRAL),
                    LanguageId::Invariant(LANG_INVARIANT,SUBLANG_NEUTRAL),
                    LanguageId::English(LANG_ENGLISH,SUBLANG_NEUTRAL),
                    LanguageId::Chinese(LANG_CHINESE,SUBLANG_NEUTRAL),
                    LanguageId::French(LANG_FRENCH,SUBLANG_NEUTRAL),
                    LanguageId::Spanish(LANG_SPANISH,SUBLANG_NEUTRAL),
                    LanguageId::Welsh(LANG_WELSH,SUBLANG_NEUTRAL),
                    LanguageId::Zulu(LANG_ZULU,SUBLANG_NEUTRAL);

  //! \var LocaleId::<various> - Initialize common language ids
  const LocaleId  LocaleId::Neutral(LOCALE_NEUTRAL),
                  LocaleId::Invariant(LOCALE_INVARIANT),
                  LocaleId::User(LOCALE_USER_DEFAULT),
                  LocaleId::System(LOCALE_SYSTEM_DEFAULT);


} //namespace wtl

