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
  //! \struct StringResource - Provides access to application resources
  //! 
  //! \tparam ENC - Character encoding type
  ///////////////////////////////////////////////////////////////////////////////
  //template <Encoding ENC = Encoding::UTF16>
  struct StringResource : Resources //<ENC>
  {      
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base class
    using base = Resources;

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

    // ------------------------ STATIC -------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // StringResource::load
    //! Loads a string resource
    //! 
    //! \tparam LEN - Output buffer capacity
    //! \tparam ENC - Output string character encoding (Also resource id encoding)
    //! 
    //! \param[in] module - Module containing string
    //! \param[in] id - String identifier
    //! \param[in,out] &str - Output buffer
    //! \return bool - True iff was found 
    //! 
    //! \throw wtl::domain_error - Insufficient buffer capacity to store string
    ///////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN, Encoding ENC = Encoding::UTF16>
    static bool load(HMODULE module, ResourceId<ENC> id, CharArray<ENC,LEN>& str) 
    {
      decltype(id)  table(id.Value.Numeral / 16 + 1);  //!< String table resource
      int32         index = id.Value.Numeral % 16;     //!< Index of desired string within table
      
      // Lookup string table handle
      HResource resource(module, table, ResourceType::String, LanguageId());
      int32 size = ::SizeofResource(module, resource);

      // Load string table
      StringTableEntry* item = load<StringTableEntry>(module, resource);

      // Find desired string
      for (int32 idx = 0; item && idx < index; idx++)
        item = item->next();

      // [NOT-FOUND] Return false & empty string 
      if (!item)
        return (str.clear(), false);

      // [FOUND] Ensure sufficient space is available
      if (item->Length > LEN)
        throw domain_error(HERE, "String resource %d requires %d chars but only %d available", id.Value.Numeral, (int32)item->Length, (int32)LEN);

      // Convert from UTF16 if necessary
      str = CharArray<Encoding::UTF16,LEN>(item->Text);
      return true;
    }
    
    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------

    // -------------------- REPRESENTATION ---------------------

  };

  
} //namespace wtl
#endif // WTL_STRING_RESOURCES_HPP

