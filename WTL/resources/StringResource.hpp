//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\StringResource.hpp
//! \brief Encapsulates reading string resources
//! \date 31 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_RESOURCES_HPP
#define WTL_STRING_RESOURCES_HPP

#include "wtl/WTL.hpp"
#include "wtl/Resources/ResourceBlob.hpp"       //!< ResourceBlob
#include "wtl/modules/Module.hpp"               //!< LoadedModules
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
#pragma pack (push, 1)
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct StringTableEntry - Variable length string table entry
    //!
    //! \remarks String table is a block of 16 PASCAL-style strings (Prefixed with length as WORD)
    //! \remarks Some of the 16 entries may be blank (Zero length, null string)
    /////////////////////////////////////////////////////////////////////////////////////////
    struct StringTableEntry
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
      
      //! \var size - 
      static constexpr int32_t  size = sizeof(wchar_t) + sizeof(uint16_t);
      
      // ----------------------------------- REPRESENTATION -----------------------------------

      uint16_t       Length;        //!< Length of current entry, in characters
      const wchar_t  Text[0xFFFF];  //!< String Text in UTF16

      // ------------------------------------ CONSTRUCTION ------------------------------------
	
      DISABLE_CTOR(StringTableEntry);     //!< Cannot instantiate

      // -------------------------------- COPY, MOVE & DESTROY --------------------------------

      DISABLE_COPY(StringTableEntry);     //!< Cannot instantiate
      DISABLE_MOVE(StringTableEntry);     //!< Cannot instantiate
      DISABLE_DTOR(StringTableEntry);     //!< Cannot instantiate

      // ---------------------------------- ACCESSOR METHODS ----------------------------------			

      /////////////////////////////////////////////////////////////////////////////////////////
      // StringTableEntry::next
      //! Advance beyond this variable length entry to the next
      //! 
      //! \return const StringTableEntry* - Immutable pointer to next string in table
      /////////////////////////////////////////////////////////////////////////////////////////
      const StringTableEntry* next() const
      {
        // [EMPTY] Skip entry
        if (!Length)
          return reinterpret_cast<const StringTableEntry*>(Text);

        // [NON-EMPTY] Skip entry text 
        return reinterpret_cast<const StringTableEntry*>(Text + Length);
      }
      
      // ----------------------------------- MUTATOR METHODS ----------------------------------
    };
#pragma pack (pop)

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ResourceBlob             Table;       //!< Table resource
    const StringTableEntry*  Entry;       //!< Desired entry
    uint16_t                 Ident;       //!< String id
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
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
    template <Encoding ENC> explicit 
    StringResource(ResourceId<ENC> id, LanguageId lang = LanguageId::Neutral) : Table(LoadedModules.findString(id,lang)), 
                                                                                Entry(Table.get<StringTableEntry>()),
                                                                                Ident(id.toOrdinal())
    {
      // [CHECK] Ensure table found
      if (!Table.exists())
        throw platform_error(HERE, "String resource ", Ident, " does not exist");

      // Find desired string
      for (int32_t i = 0, index = Ident % 16; Entry && i < index; i++)
        Entry = Entry->next();

      // [NOT-FOUND] Return false & empty string 
      if (!Entry || !Entry->Length)
        throw logic_error(HERE, "String resource ", Ident, " does not exist");
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(StringResource);       //!< Can be shallow copied  
    ENABLE_MOVE(StringResource);       //!< Can be moved
    ENABLE_POLY(StringResource);       //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
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

