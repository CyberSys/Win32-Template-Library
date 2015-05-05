//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\CursorTraits.hpp
//! \brief Defines cursor traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_CURSOR_TRAITS_HPP
#define WTL_CURSOR_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum SystemCursor - System cursor Ids
  enum class SystemCursor
  {
    Arrow = 32512,			  //!< 
    IBeam = 32513,			  //!< 
    Wait = 32514,			    //!< 
    Cross = 32515,			  //!< 
    Uparrow = 32516,			//!< 
    Size = 32640,			    //!< obsolete: use SizeAll
    Icon = 32641,			    //!< obsolete: use Arrow
    SizeNWSE = 32642,			//!< 
    SizeNESW = 32643,			//!< 
    SizeWE = 32644,			  //!< 
    SizeNS = 32645,			  //!< 
    SizeAll = 32646,			//!< 
    No = 32648,			      //!< 
    Hand = 32649,			    //!< [Windows 5.00]
    AppStarting = 32650,	//!< 
    Help = 32651,			    //!< 
  };
  
  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SystemCursor>  : std::false_type  {};
  template <> struct is_contiguous<SystemCursor> : std::false_type  {};
  
  //! Define limits traits
  template <> struct max_value<SystemCursor>     : std::integral_constant<SystemCursor,SystemCursor::Help>  {};
  template <> struct min_value<SystemCursor>     : std::integral_constant<SystemCursor,SystemCursor::Arrow> {};




  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<HCURSOR> - Encapsulates menu handle allocation
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <>
  //struct handle_alloc<HCURSOR>
  //{
  //  //! \var npos - Invalid handle sentinel value
  //  static const HCURSOR npos; 

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // handle_alloc<HCURSOR>::create
  //  //! Load cursor from resource
  //  //! 
  //  //! \tparam ENC - Character encoding 
  //  //! 
  //  //! \param[in] instance - Instance containing cursor
  //  //! \param[in] ident - Cursor identifier
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <Encoding ENC>
  //  static HCURSOR create(HINSTANCE instance, ResourceId<ENC> ident) 
  //  { 
  //    return getFunc<ENC>(::LoadCursorA,::LoadCursorW)(instance, ident); 
  //  }
  //  
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // handle_alloc<HCURSOR>::create
  //  //! Load cursor from system resource
  //  //! 
  //  //! \tparam ENC - Character encoding 
  //  //! 
  //  //! \param[in] ident - System cursor identifier
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <Encoding ENC = Encoding::UTF16>
  //  static HCURSOR create(SystemCursor ident) 
  //  { 
  //    return getFunc<ENC>(::LoadCursorA,::LoadCursorW)(npos, ident); 
  //  }
  //  
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // handle_alloc<HCURSOR>::clone
  //  //! Clone handle
  //  //! 
  //  //! \param[in] cursor - Cursor handle
  //  //! \return HCURSOR - Duplicate of handle
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  static HCURSOR clone(HCURSOR menu);

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  // handle_alloc<HCURSOR>::destroy noexcept
  //  //! Release handle
  //  //! 
  //  //! \param[in] cursor - Cursor handle
  //  //! \return bool - True iff closed successfully
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  static bool destroy(HCURSOR menu) noexcept
  //  {
  //    // no-op
  //    return true;
  //  }
  //};


  //! \alias HCursor - Shared cursor handle
  using HCursor = Handle<HCURSOR>;

  
} //namespace wtl
#endif // WTL_CURSOR_TRAITS_HPP

