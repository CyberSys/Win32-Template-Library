//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\WindowClassTraits.hpp
//! \brief Defines window class traits
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_CLASS_TRAITS_HPP
#define WTL_WINDOW_CLASS_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"         //!< EnumCast
#include "wtl/utils/Handle.hpp"           //!< Handle
#include "wtl/utils/Default.hpp"          //!< Default
#include "wtl/platform/ResourceId.hpp"    //!< ResourceId
#include "wtl/traits/EncodingTraits.hpp"  //!< Encoding

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias Atom - Shared class atom handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HAtom = Handle<::ATOM>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::ATOM> - Encapsulates window class registration
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::ATOM>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::ATOM npos = default<::ATOM>(); 

    //! \alias WndClassEx - Window class data type
    template <Encoding ENC>
    using WndClassEx = getType<encoding_char_t<ENC>,::WNDCLASSEXA,::WNDCLASSEXW>;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::ATOM>::create
    //! Create from individual properties
    //! 
    //! \tparam ENC - String encoding
    //!
    //! \param[in] instance - Instance handle
    //! \param[in] name - Class name
    //! \param[in] style - Window style
    //! \param[in] proc - Window procedure
    //! \param[in] menu - Menu resource id
    //! \param[in] cursor - Window cursor handle
    //! \param[in] brush - Background brush handle
    //! \param[in] smIcon - Small icon handle
    //! \param[in] bgIcon - Large icon handle
    //! \param[in] clsBytes - Size of class storage, if any
    //! \param[in] wndBytes - Size of handle storage, if any
    //! \return HAlloc<::ATOM> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    static HAlloc<::ATOM> create(::HINSTANCE instance, 
                                 ResourceId<ENC> name,
                                 ClassStyle style, 
                                 ::WNDPROC proc, 
                                 ResourceId<ENC> menu, 
                                 ::HCURSOR cursor, 
                                 ::HBRUSH brush, 
                                 ::HICON smIcon, 
                                 ::HICON bgIcon, 
                                 int32 clsBytes = 0, 
                                 int32 wndBytes = 0)
    {
      // Init window class
      WndClassEx<ENC> wndCls = {sizeof(wndCls), enum_cast(style), proc, clsBytes, wndBytes, instance, bgIcon, cursor, brush, menu, name, smIcon};

      /*wndCls.style = enum_cast(style);
      wndCls.lpfnWndProc = proc;
      wndCls.cbClsExtra = clsBytes;
      wndCls.cbWndExtra = wndBytes;
      wndCls.hInstance = instance;
      wndCls.hIcon = bgIcon;
      wndCls.hIconSm = smIcon;
      wndCls.lpszMenuName = menu;
      wndCls.lpszClassName = name;
      wndCls.hbrBackground = brush;
      wndCls.hCursor = cursor;*/

      // Register class
      if (::ATOM atom = getFunc<ENC>(::RegisterClassExA,::RegisterClassExW)(&wndCls))
        return { atom, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to register window class");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::ATOM>::clone
    //! Clone window class atom
    //! 
    //! \param[in] atom - Window class atom
    //! \return HAlloc<::ATOM> - Duplicate of handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::ATOM> clone(HAlloc<::ATOM> atom);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::ATOM>::destroy noexcept
    //! Unregister window class
    //! 
    //! \param[in] atom - Registered window class atom
    //! \return bool - True iff unregistered successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::ATOM> atom) noexcept
    {
      // Delete without checking if handle is valid
      return ::UnregisterClassW((const wchar_t*)atom.Handle, nullptr) != FALSE;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_WINDOW_CLASS_TRAITS_HPP

