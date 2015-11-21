//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\Metrics.hpp
//! \brief Encapsulates system metrics
//! \date 20 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////

#include <wtl/platform/Metrics.hpp>       //!< Metrics

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl
{

  SizeL  Metrics::SmallIcon     { SystemMetric::cxSmallIcon, SystemMetric::cySmallIcon };     //!< The recommended size of a small icon for the window caption, in pixels
  SizeL  Metrics::Cursor        { SystemMetric::cxCursor, SystemMetric::cyCursor };           //!< The size of a cursor, in pixels
  SizeL  Metrics::HScrollbar    { SystemMetric::cxHScroll, SystemMetric::cyHScroll };         //!< The size of a horizontal scrollbar, in pixels
  SizeL  Metrics::VScrollbar    { SystemMetric::cxVScroll, SystemMetric::cyVScroll };         //!< The size of a vertical scrollbar, in pixels
  SizeL  Metrics::WindowBorder  { SystemMetric::cxBorder, SystemMetric::cyBorder };           //!< The size of a 3D window border, in pixels
  SizeL  Metrics::WindowEdge    { SystemMetric::cxEdge, SystemMetric::cyEdge };               //!< The size of a 3D window edge, in pixels
  SizeL  Metrics::WindowIcon    { SystemMetric::cxIcon, SystemMetric::cyIcon };               //!< The default size of an icon, in pixels, as loaded by ::LoadIcon()

}
