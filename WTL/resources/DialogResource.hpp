//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\resources\DialogResource.hpp
//! \brief Encapsulates reading dialog resources
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DIALOG_RESOURCES_HPP
#define WTL_DIALOG_RESOURCES_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \struct DLGTEMPLATEEX - Extended dialog template
  struct DLGTEMPLATEEX
  {
    WORD           version;
    WORD           signature;
    DWORD          helpID;
    WindowStyleEx  styleEx;
    WindowStyle    style;
    WORD           numItems;
    short          x, y,
                   cx, cy;
    WCHAR*         menu;                    //!< Variable-length field
    /*
    WCHAR*         windowClass;             //!< Variable-length field
    WCHAR          titleLen;
    WCHAR          title[titleLen];         //!< Variable-length field
    WORD           pointsize;
    WORD           weight;
    BYTE           italic;
    BYTE           charset;
    WCHAR          stringLen;
    WCHAR          typeface[stringLen];     //!< Variable-length field
    */
  };
    
  //! \struct DLGITEMTEMPLATEEX - Extended Dialog Item Template
  struct DLGITEMTEMPLATEEX
  {
    ulong32       helpId;     //!< Control help Id
    WindowStyleEx styleEx;    //!< Extended style
    WindowStyle   style;      //!< Style
    int16         x,          //!< X co-ordinate in dialog units
                  y,          //!< Y co-ordinate in dialog units
                  cx,         //!< Width in dialog units
                  cy;         //!< Height in dialog units
    ulong32       ctrlId;     //!< Control Id
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Resources - Provides access to application resources
  //! 
  //! \tparam ENC - Character encoding type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC = Encoding::UTF16>
  struct DialogResource : Resources<ENC>
  {      
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias char_t - Defines character type
    using char_t = encoding_char_t<ENC>;

    //! \alias stream_t - Defines memory stream type
    using stream_t = MemoryStream<uint8>;

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct DialogItemTemplate - Variable length dialog item template
    /////////////////////////////////////////////////////////////////////////////////////////
    struct DialogItemTemplate 
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias ByteVector - User data storage type
      using ByteVector = std::vector<uint8>;

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // DialogItemTemplate::DialogItemTemplate 
      //! Create from extended item template (DLGITEMTEMPLATEEX)
      //! 
      //! \param[in] &stream - Binary reader containing extended item template
      /////////////////////////////////////////////////////////////////////////////////////////
      DialogItemTemplate(BinaryReader<stream_t>& stream) 
      {
        // Read item data
        stream >> HelpId 
               >> StyleEx 
               >> Style;

        int16 x, y, cx, cy;   //!< Item position and size (in dialog units)

        // Read position
        stream >> x >> y >> cx >> cy;
        Position.set(x,y,cx,cy);
      
        // Read control Id
        in >> Ident;

        // Read window class
        ResourceId<ENC> idClass;
        in >> ident;

        // Copy/lookup window class
        if (ident.isString())
          WndClass = ident.toString();
        else
          WndClass = getWindowClass(static_cast<SystemClass>(ident.toOrdinal()));

        // Read title
        in >> DlgTitle;

        // Read amount of creation data
        int16 numBytes;
        in >> numBytes;

        // Read creation data, if any
        if (numBytes)
          UserData.assign(in.position(), in.position()+numBytes);
      }

      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------			

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      // ----------------------------------- REPRESENTATION -----------------------------------

      WindowStyle    Style;          //!< Window style
      WindowStyleEx  StyleEx;        //!< Extended window style
      WindowId       Ident;          //!< Control id
      long32         HelpId;         //!< Help id context
      Rect<int16>    Position;       //!< Position
      std::wstring   Text,           //!< Item text
                     WndClass;       //!< Window class name
      ByteVector     UserData;       //!< Creation data
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct DialogTemplate - Variable length dialog item template
    //! 
    //! \tparam ENC - Character encoding
    /////////////////////////////////////////////////////////////////////////////////////////
    struct DialogTemplate 
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias stream_t - Defines input stream type
      using stream_t = MemoryStream<encoding_char_t<ENC>>;

      using ItemCollection = std::vector<DialogItemTemplate>;

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // DialogTemplate::DialogTemplate 
      //! Create from an extended dialog template
      //! 
      //! \param[in] &stream - Binary reader containing extended dialog item template
      /////////////////////////////////////////////////////////////////////////////////////////
      DialogTemplate(BinaryReader<stream_t>& stream) 
      {
        int16  version,       //!< Version
               signature;     //!< Signature
        
        // Read version 
        stream >> version 
               >> signature;

        // Check version for extended dialog signature
        if (version != 1 || signature != 0xFFFF)
          throw;

        int16 numItems;

        // Read dialog data
        stream >> HelpId 
               >> Style 
               >> StyleEx
               >> numItems;

        int16 x, y, cx, cy;   //!< Dialog position and size (in dialog units)

        // Read position
        stream >> x >> y >> cx >> cy;
        Position.set(x,y,cx,cy);
      
        // Read menu
        in >> MenuId;

        // Read items
        for (uint32 idx = 0; idx < numItems; ++idx)
          Items.emplace_back(stream);
      }
      
      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------			

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      // ----------------------------------- REPRESENTATION -----------------------------------
    public:
      WindowStyle    Style;          //!< Window style
      WindowStyleEx  StyleEx;        //!< Extended window style
      WindowId       Ident;          //!< Window id
      Rect<int16>    Position;       //!< Position
      std::wstring   DlgTitle,       //!< Dialog title
                     WndClass,       //!< Window class name
                     CreateData;     //!< Creation data
      ItemCollection Items;          //!< Item templates
    };

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // DialogItemTemplate::getWindowClass
    //! Get a system window class name from its ordinal
    //! 
    //! \param[in] id - System window class identifier
    //! 
    //! \throw wtl::domain_error - Unrecognised window class
    /////////////////////////////////////////////////////////////////////////////////////////
    static std::wstring getWindowClass(SystemClass id)
    {
      switch (id)
      {
      case SystemClass::Button:      return WC_BUTTONW; 
      case SystemClass::Edit:        return WC_EDITW; 
      case SystemClass::Static:      return WC_STATICW; 
      case SystemClass::ListBox:     return WC_LISTBOXW;
      case SystemClass::ScrollBar:   return WC_SCROLLBARW;
      case SystemClass::ComboBox:    return WC_COMBOBOXW;
      }

      // Unrecognised window class ordinal
      throw domain_error(HERE, "Unrecognised window class ordinal '%d'", enum_cast(id));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Resources::load
    //! Find the resource data block associated with a resource handle
    //! 
    //! \tparam DATA - Resource storage format
    //! 
    //! \param[in] module - Module containing resource
    //! \param[in] res - Resource handle
    //! \return DATA* - Resource data block
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename DATA = uint8>
    static DATA* load(HMODULE module, const HResource& res)
    {
      // Load resource
      if (HGLOBAL block = ::LoadResource(module, res))
      {
        // Retrieve data
        if (DATA* data = reinterpret_cast<DATA*>(::LockResource(block)))
          return data;

        // Failed
        throw platform_error(HERE, "Unable to lock resource");
      }
      
      // Not found
      throw platform_error(HERE, "Missing resource");
    }

  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Resources::loadDialog
    //! Loads a string resource
    //! 
    //! \tparam ENC - Output character encoding 
    //! \tparam LENGTH - Output buffer length
    //! 
    //! \param[in] module - Module containing string
    //! \param[in] id - String identifier
    //! \param[in,out] &str - Output buffer
    //! \return bool - True iff was found 
    //! 
    //! \throw wtl::domain_error - Insufficient buffer capacity
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LENGTH>
    bool loadDialog(HMODULE module, ResourceId<ENC> id, DLGTEMPLATE) 
    {
    }

  };

  
} //namespace wtl
#endif // WTL_DIALOG_RESOURCES_HPP

