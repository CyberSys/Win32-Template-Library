//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\IconTraits.hpp
//! \brief Defines icon traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_ICON_TRAITS_HPP
#define WTL_ICON_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/traits/BuildTraits.hpp"             //!< enable_if_build_t
#include "wtl/traits/EncodingTraits.hpp"          //!< EncodingTraits
#include "wtl/utils/Size.hpp"                  //!< Size
#include "wtl/platform/SystemFlags.hpp"           //!< SystemIcon
#include "wtl/platform/ResourceId.hpp"            //!< ResourceId

//! \namespace wtl - Windows template library
namespace wtl
{ 
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HIcon - Shared icon handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HIcon = Handle<::HICON>;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HICON> - Encapsulates icon handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HICON>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  protected:
    //! \enum IconFormat - Define data format
    enum class IconFormat : long32_t { v2 = 0x00020000, v3 = 0x00030000 };
    
  public:
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HICON npos = defvalue<::HICON>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HICON>::create
    //! Load icon from resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] instance - Instance containing icon
    //! \param[in] ident - Icon identifier
    //! \return NativeHandle<::HICON> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static NativeHandle<::HICON> create(HINSTANCE instance, ResourceId<ENC> ident) 
    { 
      // Load icon handle
      if (::HICON icon = WinAPI<ENC>::loadIcon(instance, ident))
        return { icon, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load icon");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HICON>::create
    //! Load icon from system resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] ident - System icon identifier
    //! \return NativeHandle<::HICON> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16>
    static NativeHandle<::HICON> create(SystemIcon ident) 
    { 
      // Load icon handle
      if (::HICON icon = WinAPI<ENC>::loadIcon(nullptr, resource_id<ENC>(ident)))
        return { icon, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load system icon");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HICON>::create
    //! Create icon from image bits
    //! 
    //! \param[in] buffer - Buffer containing image
    //! \param[in] len - Length of buffer
    //! \param[in] size - Desired icon size
    //! \return NativeHandle<::HICON> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16, typename = enable_if_build_t<WindowVersion::Win2000>>
    static NativeHandle<::HICON> create(byte* buffer, int32_t len, SizeL size) 
    { 
      // Create icon handle from bits
      if (::HICON icon = CreateIconFromResourceEx(buffer, len, True, enum_cast(IconFormat::v3), size.Width, size.Height, LR_DEFAULTCOLOR))
        return { icon, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create icon from bits");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HICON>::create
    //! Create icon from image bits
    //! 
    //! \param[in] buffer - Buffer containing image
    //! \param[in] len - Length of buffer
    //! \param[in] defaultSize - True to pick size based on system metrics. False to use actual size.
    //! \return NativeHandle<::HICON> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16, typename = enable_if_build_t<WindowVersion::Win2000>>
    static NativeHandle<::HICON> create(byte* buffer, int32_t len, bool defaultSize) 
    { 
      // Create icon handle from bits
      if (::HICON icon = CreateIconFromResourceEx(buffer, len, True, enum_cast(IconFormat::v3), 0, 0, defaultSize ? LR_DEFAULTCOLOR|LR_DEFAULTSIZE : LR_DEFAULTCOLOR))
        return { icon, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create icon from bits");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HCURSOR>::create
    //! Load cursor from system resource
    //! 
    //! \tparam ENC - Character encoding 
    //! 
    //! \param[in] ident - System cursor identifier
    //! \return NativeHandle<::HCURSOR> - Accquired handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC = Encoding::UTF16>
    static NativeHandle<::HCURSOR> create(SystemCursor ident) 
    { 
      // Load cursor handle
      if (::HCURSOR cursor = WinAPI<ENC>::loadCursor(nullptr, resource_id<ENC>(ident)))
        return { cursor, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to load system cursor");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HICON>::clone
    //! Clone handle
    //! 
    //! \param[in] icon - Icon handle
    //! \return ::HICON - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HICON> clone(NativeHandle<::HICON> icon);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HICON>::destroy noexcept
    //! Release brush handle
    //! 
    //! \param[in] icon - Icon handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(NativeHandle<::HICON> icon) noexcept
    {
      // no-op
      return true;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };
  
  
} //namespace wtl
#endif // WTL_ICON_TRAITS_HPP

