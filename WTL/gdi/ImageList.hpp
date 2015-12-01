/////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\ImageList.hpp
//! \brief Encapsulates a Win32 ImageList
//! \date 6 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_IMAGE_LIST_H
#define WTL_IMAGE_LIST_H

#include <wtl/WTL.hpp>
#include <wtl/gdi/DeviceContext.hpp>               //!< HDeviceContext
#include <wtl/traits/ImageListTraits.hpp>          //!< HImageList
#include <wtl/platform/HResult.hpp>                //!< HResult

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ImageList - Eventually this will encapsulate an ImageList
  /////////////////////////////////////////////////////////////////////////////////////////
  struct ImageList 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
     
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HImageList   Handle;     //!< ImageList handle
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ImageList::ImageList
    //! Create theme from class name
    //!
    //! \param[in] const& sz - Image size
    //! \param[in] flags - Creation flags
    //! \param[in] count - Number of initial images
    //! \param[in] grow - Number of extra elements to add when the capacity is reached
    //! 
    //! \throw wtl::platform_error - Failed to acquire theme handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    ImageList(const Size<T>& sz, ImageListType flags, int32_t count, int32_t grow) : Handle(wnd, names)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    //  ** TODO **

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    //  ** TODO **
  };

  

} // namespace wtl
#endif // WTL_IMAGE_LIST_H

