//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\traits\ImageListTraits.hpp
//! \brief Defines image list traits
//! \date 6 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_IMAGELIST_TRAITS_HPP
#define WTL_IMAGELIST_TRAITS_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>                   //!< Handle
#include <wtl/utils/Size.hpp>                     //!< Size
#include <wtl/platform/DrawingFlags.hpp>          //!< ImageListType

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias HImageList - Shared image-list handle
  /////////////////////////////////////////////////////////////////////////////////////////
  using HImageList = Handle<::HIMAGELIST>;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct handle_alloc<::HIMAGELIST> - Encapsulates creating image-list handles
  /////////////////////////////////////////////////////////////////////////////////////////
  template <>
  struct handle_alloc<::HIMAGELIST>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \var npos - Invalid handle sentinel value
    static constexpr ::HIMAGELIST npos = defvalue<::HIMAGELIST>(); 
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    DISABLE_CTOR(handle_alloc);     //!< Cannot instantiate

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    DISABLE_COPY(handle_alloc);     //!< Cannot instantiate
    DISABLE_MOVE(handle_alloc);     //!< Cannot instantiate
    DISABLE_DTOR(handle_alloc);     //!< Cannot instantiate

    // ----------------------------------- STATIC METHODS -----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HIMAGELIST>::create
    //! Create image-list handle
    //! 
    //! \param[in] const& sz - Image size
    //! \param[in] flags - Creation flags
    //! \param[in] count - Number of initial images
    //! \param[in] grow - Number of extra elements to add when the capacity is reached
    //! \return NativeHandle<::HIMAGELIST> - Created handle
    //!
    //! \throw wtl::platform_error - Unable to allocate handle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    static NativeHandle<::HIMAGELIST>  create(const Size<T>& sz, ImageListType flags, int32_t count, int32_t grow) 
    { 
      if (::HIMAGELIST list = ImageList_Create(sz.Width, sz.Height, enum_cast(flags), count, grow))
        return { list, AllocType::Create };

      // Error: Failed  
      throw platform_error(HERE, "Unable to create image list");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HIMAGELIST>::clone
    //! Clone handle
    //! 
    //! \param[in] list - Handle
    //! \return NativeHandle<::HIMAGELIST> - Duplicate of handle
    /////////////////////////////////////////////////////////////////////////////////////////
    static NativeHandle<::HIMAGELIST>  clone(NativeHandle<::HIMAGELIST> list);

    /////////////////////////////////////////////////////////////////////////////////////////
    // handle_alloc<::HIMAGELIST>::destroy noexcept
    //! Release handle 
    //! 
    //! \param[in] list - Handle
    //! \return bool - True iff closed successfully
    //!
    //! \throw wtl::invalid_argument - [Debug only] Handle is invalid
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool  destroy(NativeHandle<::HIMAGELIST> list) noexcept
    {
      // Delete without checking if handle is valid
      switch (list.Method)
      {
      case AllocType::Accquire: return true;
      case AllocType::Create:   return ImageList_Destroy(list.Handle) != False;
      case AllocType::WeakRef:  return true;
      }
      return false;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} //namespace wtl
#endif // WTL_IMAGELIST_TRAITS_HPP

