////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\IconTraits.hpp
//! \brief Defines icon traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ICON_TRAITS_HPP
#define WTL_ICON_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{ 
  //! \enum SystemIcon - System icon Ids
  enum class SystemIcon
  {
    Application = 32512,			    //!< 
    Hand = 32513,			            //!< 
    Question = 32514,			        //!< 
    Exclamation = 32515,			    //!< 
    Asterisk = 32516,			        //!< 
    WinLogo = 32517,			        //!< 
    Shield = 32518,               //!< [Windows 6.00]
    Warning = Exclamation,			  //!< 
    Error = Hand,			            //!< 
    Information = Asterisk,			  //!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemIcon>  : std::false_type  {};
  template <> struct is_contiguous<SystemIcon> : std::false_type  {};
  
  //! Define limits traits
  template <> struct max_value<SystemIcon>     : std::integral_constant<SystemIcon,SystemIcon::Shield>      {};
  template <> struct min_value<SystemIcon>     : std::integral_constant<SystemIcon,SystemIcon::Application> {};

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<HICON> - Encapsulates icon handle allocation
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<HICON>
  {
  protected:
    //! \enum IconFormat - Define data format
    enum class IconFormat : long32 { v2 = 0x00020000, v3 = 0x00030000 };
    
  public:
    //! \var npos - Invalid handle sentinel value
    static const HICON npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HICON>::create
    //! Load icon from resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] instance - Instance containing icon
    //! \param[in] ident - Icon identifier
    //! \return HAlloc<HICON> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<HICON> create(HINSTANCE instance, ResourceId<ENC> ident) 
    { 
      // Load icon handle
      if (HICON icon = getFunc<ENC>(::LoadIconA,::LoadIconW)(instance, ident))
        return { icon, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load icon");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HICON>::create
    //! Load icon from system resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] ident - System icon identifier
    //! \return HAlloc<HICON> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16>
    static HAlloc<HICON> create(SystemIcon ident) 
    { 
      // Load icon handle
      if (HICON icon = getFunc<ENC>(::LoadIconA,::LoadIconW)(nullptr, resource_id<ENC>(ident)))
        return { icon, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load system icon");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HICON>::create
    //! Create icon from image bits
    //! 
    //! \param[in] buffer - Buffer containing image
    //! \param[in] len - Length of buffer
    //! \param[in] size - Desired icon size
    //! \return HAlloc<HICON> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16, typename = enable_if_build_t<WindowVersion::Win2000>>
    static HAlloc<HICON> create(byte* buffer, int32 len, SizeL size) 
    { 
      // Create icon handle from bits
      if (HICON icon = CreateIconFromResourceEx(buffer, len, TRUE, enum_cast(IconFormat::v3), size.width, size.height, LR_DEFAULTCOLOR))
        return { icon, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create icon from bits");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HICON>::create
    //! Create icon from image bits
    //! 
    //! \param[in] buffer - Buffer containing image
    //! \param[in] len - Length of buffer
    //! \param[in] defaultSize - True to pick size based on system metrics. False to use actual size.
    //! \return HAlloc<HICON> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16, typename = enable_if_build_t<WindowVersion::Win2000>>
    static HAlloc<HICON> create(byte* buffer, int32 len, bool defaultSize) 
    { 
      // Create icon handle from bits
      if (HICON icon = CreateIconFromResourceEx(buffer, len, TRUE, enum_cast(IconFormat::v3), 0, 0, defaultSize ? LR_DEFAULTCOLOR|LR_DEFAULTSIZE : LR_DEFAULTCOLOR))
        return { icon, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create icon from bits");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HCURSOR>::create
    //! Load cursor from system resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] ident - System cursor identifier
    //! \return HAlloc<HCURSOR> - Accquired handle
    ///////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16>
    static HAlloc<HCURSOR> create(SystemCursor ident) 
    { 
      // Load cursor handle
      if (HICON cursor = getFunc<ENC>(::LoadCursorA,::LoadCursorW)(nullptr, resource_id<ENC>(ident)))
        return { cursor, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load system cursor");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HICON>::clone
    //! Clone handle
    //! 
    //! \param[in] icon - Icon handle
    //! \return HICON - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HICON> clone(HAlloc<HICON> icon);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HICON>::destroy noexcept
    //! Release brush handle
    //! 
    //! \param[in] icon - Icon handle
    //! \return bool - True iff closed successfully
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<HICON> icon) noexcept
    {
      // no-op
      return true;
    }
  };
  
  //! \alias HIcon - Shared icon handle
  using HIcon = Handle<HICON>;

  
} //namespace wtl
#endif // WTL_ICON_TRAITS_HPP

