////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\BrushTraits.hpp
//! \brief Defines brush traits 
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BRUSH_TRAITS_HPP
#define WTL_BRUSH_TRAITS_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<HBRUSH> - Encapsulates brush handle allocation
  ///////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<HBRUSH>
  {
    //! \var npos - Invalid handle sentinel value
    static const HBRUSH npos; 

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HBRUSH>::create
    //! Create solid brush
    //! 
    //! \param[in] colour - Brush colour
    //! \return HAlloc<HBRUSH> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HBRUSH> create(Colour colour) 
    { 
      // Create solid colour brush handle
      if (HBRUSH brush = ::CreateSolidBrush(enum_cast(colour)))
        return { brush, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create solid brush");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HBRUSH>::create
    //! Create hatch brush
    //! 
    //! \param[in] style - Hatch style
    //! \param[in] colour - Brush colour
    //! \return HAlloc<HBRUSH> - Created handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HBRUSH> create(HatchStyle style, Colour colour) 
    { 
      // Create hatch brush handle
      if (HBRUSH brush = ::CreateHatchBrush(enum_cast(style), enum_cast(colour)))
        return { brush, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create hatch brush");
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HBRUSH>::create
    //! Create system colour brush
    //! 
    //! \param[in] col - System colour
    //! \return HAlloc<HBRUSH> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HBRUSH> create(SystemColour col) 
    { 
      // Accquire system colour brush handle
      if (HBRUSH brush = ::GetSysColorBrush(enum_cast(col)))
        return { brush, AllocType::Accquire };

      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire system colour brush");
    }
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HBRUSH>::create
    //! Accquire stock brush handle
    //! 
    //! \param[in] obj - Stock object
    //! \return HAlloc<HBRUSH> - Accquired handle
    //! 
    //! \throw wtl::platform_error - Failed to allocate handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HBRUSH> create(StockObject obj) 
    { 
      switch (obj)
      {
      case StockObject::WhiteBrush:
      case StockObject::LtGreyBrush:
      case StockObject::GreyBrush:
      case StockObject::DkBreyBrush:
      case StockObject::BlackBrush:
      case StockObject::NullBrush:
      case StockObject::DcBrush:
        // Accquire stock brush
        if (HBRUSH brush = (HBRUSH)::GetStockObject(enum_cast(obj)))
          return { brush, AllocType::WeakRef };
      }
      
      // Error: Failed  
      throw platform_error(HERE, "Unable to accquire stock brush");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HBRUSH>::clone
    //! Clone handle
    //! 
    //! \param[in] brush - Brush handle
    //! \return HAlloc<HBRUSH> - Duplicate of handle
    //! 
    //! \throw wtl::platform_error - Failed to clone handle
    ///////////////////////////////////////////////////////////////////////////////
    static HAlloc<HBRUSH>  clone(HAlloc<HBRUSH> brush);

    ///////////////////////////////////////////////////////////////////////////////
    // handle_alloc<HBRUSH>::destroy noexcept
    //! Release brush handle
    //! 
    //! \param[in] brush - Brush handle
    //! \return bool - True iff closed successfully
    ///////////////////////////////////////////////////////////////////////////////
    static bool destroy(HAlloc<HBRUSH> brush) noexcept
    {
      // Delete without checking if handle is valid
      switch (brush.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return ::DeleteObject(brush.Handle) != FALSE;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
  };


  //! \alias HBrush - Shared brush handle
  using HBrush = Handle<HBRUSH>;

  
} //namespace wtl
#endif // WTL_BRUSH_TRAITS_HPP

