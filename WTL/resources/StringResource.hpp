////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\StringResource.hpp
//! \brief Encapsulates reading string resources
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_STRING_RESOURCES_HPP
#define WTL_STRING_RESOURCES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct StringResource - Encapsulates loading a string from the resource table
  //! 
  //! \tparam ENC - Character encoding type
  //! \tparam LEN - Buffer capacity
  ///////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, unsigned LEN>
  struct StringResource : CharArray<ENC,LEN>, 
                          Resource
  {      
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias resource_base - Define resource base class
    using resource_base = Resource;

    //! \alias string_base - Define character array base class
    using string_base = CharArray<ENC,LEN>;
    
    ///////////////////////////////////////////////////////////////////////////////
    //! \struct StringTableEntry - Variable length string table entry
    ///////////////////////////////////////////////////////////////////////////////
    struct StringTableEntry
    {
      // ------------------- TYPES & CONSTANTS -------------------

      //! \alias string_t - String buffer type
      using string_t = const wchar_t*;

      // --------------------- CONSTRUCTION ----------------------
      
      // ---------------------- ACCESSORS ------------------------			

      ///////////////////////////////////////////////////////////////////////////////
      // StringTableEntry::next
      //! Advance beyond this variable length entry to the next
      //! 
      //! \return const StringTableEntry* - Immutable pointer to next string in table
      ///////////////////////////////////////////////////////////////////////////////
      const StringTableEntry* next() const
      {
        return reinterpret_cast<const StringTableEntry*>(Text + Length);
      }
      
      // ----------------------- MUTATORS ------------------------

      // -------------------- REPRESENTATION ---------------------

      uint16   Length;      //!< Length of current entry, in characters
      string_t Text;        //!< String Text in UTF16
    };

    // --------------------- CONSTRUCTION ----------------------
    
    ///////////////////////////////////////////////////////////////////////////////
    // StringResource::StringResource
    //! Loads a string resource
    //! 
    //! \param[in] id - String identifier
    //! \param[in] lang - String language
    //! 
    //! \throw wtl::logic_error - Missing string -or- Insufficient buffer capacity to store string
    //! \throw wtl::platform_error - Unable to load resource
    ///////////////////////////////////////////////////////////////////////////////
    StringResource(ResourceId<ENC> id, LanguageId lang = LanguageId::Neutral) 
      : resource_base(LoadedModules.findString(id,lang))
    {
      const int32  index = id.Value.Numeral % 16;                   //!< Index of desired string within table
      
      // Load string table
      const StringTableEntry* item = get<StringTableEntry>();

      // Find desired string
      for (int32 idx = 0; item && idx < index; idx++)
        item = item->next();

      // [NOT-FOUND] Return false & empty string 
      if (!item)
        return logic_error(HERE, "String resource %d does not exist", id.Value.Numeral);

      // [FOUND] Ensure sufficient space is available
      if (item->Length > LEN)
        throw logic_error(HERE, "String resource %d requires %d chars but only %d available", id.Value.Numeral, (int32)item->Length, (int32)LEN);

      // Convert from UTF16 if necessary
      assign<Encoding::UTF16>(item->Text);
      return true;
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

  };

  
} //namespace wtl
#endif // WTL_STRING_RESOURCES_HPP

