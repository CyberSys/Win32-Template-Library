//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\Enumeration.hpp
//! \brief Defines enumeration traits + helper global operators
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_PEN_TRAITS_HPP
#define WTL_PEN_TRAITS_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Handle.hpp"               //!< Handle
#include "wtl/platform/Colours.hpp"           //!< Colour, SystemColour
#include "wtl/platform/DrawingFlags.hpp"      //!< StockObject, PenStyle
#include "wtl/casts/EnumCast.hpp"             //!< EnumCast

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HPen - Shared pen handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HPen = Handle<::HPEN>;

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HPEN> - Encapsulates creating device context pens
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HPEN>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HPEN npos = default<::HPEN>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HPEN>::create
    //! Create pen handle
    //! 
    //! \param[in] style - Style
    //! \param[in] width - Width
    //! \param[in] colour - RGB colour
    //! \return HAlloc<HMENU> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HPEN> create(PenStyle style, int32 width, Colour colour) 
    { 
      // Create solid colour brush handle
      if (::HPEN pen = ::CreatePen(enum_cast(style), width, enum_cast(colour)))
        return { pen, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create geometric pen");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HPEN>::create
    //! Accquire stock pen handle
    //! 
    //! \param[in] obj - Stock object
    //! \return HAlloc<::HPEN> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HPEN> create(StockObject obj) 
    { 
      switch (obj)
      {
      case StockObject::WhitePen:
      case StockObject::BlackPen:
      case StockObject::NullPen:
      case StockObject::DcPen:
        // Accquire stock pen
        if (::HPEN pen = (::HPEN)::GetStockObject(enum_cast(obj)))
          return { pen, AllocType::WeakRef };
      }
      
      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire stock pen");
    }
    

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HPEN>::clone
    //! Clone handle
    //! 
    //! \param[in] pen - Handle
    //! \return HAlloc<::HPEN> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static HAlloc<::HPEN> clone(HAlloc<::HPEN> pen);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HPEN>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] pen - Handle
    //! \return bool - True iff closed successfully
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<::HPEN> pen) noexcept
    {
      // Delete without checking if handle is valid
      switch (pen.Method)
      {
      case AllocType::Accquire: return ::DeleteObject(pen.Handle) != False;
      case AllocType::Create:   return false;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_PEN_TRAITS_HPP

