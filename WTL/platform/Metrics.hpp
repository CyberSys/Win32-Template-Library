//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Metrics.hpp
//! \brief Encapsulates system metrics
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_METRICS_HPP
#define WTL_METRICS_HPP

#include <wtl/WTL.hpp>
#include <wtl/platform/SystemFlags.hpp>       //!< SystemMetric
#include <wtl/utils/Size.hpp>                 //!< Size

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Metrics - Encapsulates system metrics
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Metrics 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    static SizeL  SmallIcon,          //!< The recommended size of a small icon for the window caption, in pixels
                  Cursor,             //!< The size of a cursor, in pixels
                  HScrollbar,         //!< The size of a horizontal scrollbar, in pixels
                  VScrollbar,         //!< The size of a vertical scrollbar, in pixels
                  WindowBorder,       //!< The size of a 3D window border, in pixels
                  WindowEdge,         //!< The size of a 3D window edge, in pixels
                  WindowIcon;         //!< The default size of an icon, in pixels, as loaded by ::LoadIcon()
          
    // ------------------------------------ CONSTRUCTION ------------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // Metrics::Metrics
    //! Cannot be instantiated
    /////////////////////////////////////////////////////////////////////////////////////////
    Metrics() = delete;
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

}

#endif  // WTL_METRICS_HPP
