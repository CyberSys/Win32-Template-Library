//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\StringResource.hpp
//! \brief Encapsulates reading string resources
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_RESOURCES_HPP
#define WTL_STRING_RESOURCES_HPP

#include "wtl/WTL.hpp"
#include "wtl/Resources/ResourceBlob.hpp"       //!< ResourceBlob
#include "wtl/utils/CharArray.hpp"              //!< CharArray
#include "wtl/utils/String.hpp"                 //!< String

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct StringResource - Encapsulates loading a string from the resource table
  /////////////////////////////////////////////////////////////////////////////////////////
  struct StringResource 
  {      
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct StringTableEntry - Variable length string table entry
    /////////////////////////////////////////////////////////////////////////////////////////
    struct StringTableEntry
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
      StringTableEntry() = delete;

      // ---------------------------------- ACCESSOR METHODS ----------------------------------			

      /////////////////////////////////////////////////////////////////////////////////////////
      // StringTableEntry::next
      //! Advance beyond this variable length entry to the next
      //! 
      //! \return const StringTableEntry* - Immutable pointer to next string in table
      /////////////////////////////////////////////////////////////////////////////////////////
      const StringTableEntry* next() const
      {
        return reinterpret_cast<const StringTableEntry*>(Text + Length);
      }
      
      // ----------------------------------- MUTATOR METHODS ----------------------------------

      // ----------------------------------- REPRESENTATION -----------------------------------

      uint16         Length;        //!< Length of current entry, in characters
      const wchar_t  Text[0xFFFF];  //!< String Text in UTF16
    };

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ResourceBlob             Table;       //!< Table resource
    const StringTableEntry*  Entry;       //!< Desired entry
    uint16                   Ident;       //!< String id
    
    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // StringResource::StringResource
    //! Loads a string resource
    //! 
    //! \tparam ENC - Character encoding type
    //! 
    //! \param[in] id - String identifier
    //! \param[in] lang - String language
    //! 
    //! \throw wtl::logic_error - Missing string -or- Insufficient buffer capacity to store string
    //! \throw wtl::platform_error - Unable to load resource
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    explicit StringResource(ResourceId<ENC> id, LanguageId lang = LanguageId::Neutral) : Table(LoadedModules.findString(id,lang)), 
                                                                                         Entry(Table.get<StringTableEntry>()),
                                                                                         Ident(id.toOrdinal())
    {
      // [CHECK] Ensure table found
      if (!Table.exists())
        throw platform_error(HERE, "String resource %d does not exist", Ident);

      // Find desired string
      for (int32 i = 1, index = Ident % 16; Entry && i < index; i++)
        Entry = Entry->next();

      // [NOT-FOUND] Return false & empty string 
      if (!Entry)
        throw logic_error(HERE, "String resource %d does not exist", Ident);
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // StringResource::c_arr const
    //! Populate character array from string resource
    //! 
    //! \tparam ENC - Character encoding type
    //! \tparam LEN - Buffer capacity
    //! 
    //! \return CharArray<ENC,LENGTH> - Character array containing string resource
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    CharArray<ENC,LEN> c_arr() const
    {
      // [FOUND] Ensure sufficient space is available
      if (Entry->Length > LEN)
        throw logic_error(HERE, "String resource %d requires %d chars but only %d available", Ident, Entry->Length, LEN);

      // Copy string as UTF16 (Convert on return if necessary)
      return CharArray<Encoding::UTF16,LEN>(Entry->Text, Entry->Text+Entry->Length);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // StringResource::c_str const
    //! Populate dynamic string from string resource
    //! 
    //! \tparam ENC - Character encoding type
    //! 
    //! \return String<ENC> - Dynamic string containing string resource
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    String<ENC> c_str() const
    {
      // Copy string as UTF16 (Convert on return if necessary)
      return String<Encoding::UTF16>(Entry->Text, Entry->Text+Entry->Length);
    }
      
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  
} //namespace wtl
#endif // WTL_STRING_RESOURCES_HPP

