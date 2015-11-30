//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowSkin.hpp
//! \brief 
//! \date 8 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_SKIN_HPP
#define WTL_WINDOW_SKIN_HPP

#include <wtl/WTL.hpp>
#include <wtl/windows/events/OwnerDrawCtrlEvent.hpp>              //!< OwnerDrawCtrlEvent
#include <wtl/windows/events/OwnerMeasureCtrlEvent.hpp>           //!< OwnerMeasureCtrlEventArgs
//#include <wtl/gdi/DeviceContext.hpp>                        //!< DeviceContext
#include <wtl/utils/Size.hpp>                               //!< SizeL

//! \namespace wtl - Windows template library
namespace wtl 
{
  //! Forward declarations
  struct DeviceContext;
  template <Encoding ENC> struct Window;
  template <Encoding ENC> struct Button;
  template <Encoding ENC> struct CheckBox;


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IWindowSkin - Interface for all window rendering visitors
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct IWindowSkin
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = IWindowSkin<ENC>;
	
    //! \var encoding - Define character encoding
    static constexpr Encoding  encoding = ENC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    ENABLE_POLY(IWindowSkin);			//!< Abstract base class

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    //! Drawing
    virtual void  draw(Button<encoding>& btn, DeviceContext& dc, const RectL& rc) const = 0;
    virtual void  draw(CheckBox<encoding>& btn, DeviceContext& dc, const RectL& rc) const = 0;
    virtual void  draw(Window<encoding>& wnd, DeviceContext& dc, const RectL& rc) const = 0;

    //! Measuring
    virtual SizeL measure(Button<encoding>& btn, DeviceContext& dc) const = 0;
    virtual SizeL measure(CheckBox<encoding>& btn, DeviceContext& dc) const = 0;
    virtual SizeL measure(Window<encoding>& wnd, DeviceContext& dc) const = 0;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct IWindowSkin - Interface for all window rendering visitors
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct SkinFactory
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = SkinFactory<ENC>;
	
    //! \var encoding - Define character encoding
    static constexpr Encoding  encoding = ENC;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    static IWindowSkin<encoding>* Current;      //!< Current window skin

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    DISABLE_CTOR(SkinFactory);			//!< Abstract base class

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // SkinFactory::get
    //! Get the active window skin
    //! 
    //! \return IWindowSkin<encoding>* - Pointer to active window skin 
    /////////////////////////////////////////////////////////////////////////////////////////
    static IWindowSkin<encoding>* get()
    {
      return Current;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // SkinFactory::set
    //! Set the active window skin
    //! 
    //! \param[in] *sf - Pointer to window skin 
    /////////////////////////////////////////////////////////////////////////////////////////
    static void set(IWindowSkin<encoding>* sf)
    {
      Current = sf;
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var SkinFactory<ENC>::Current - Current window skin
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  IWindowSkin<SkinFactory<ENC>::encoding>* SkinFactory<ENC>::Current = nullptr;

} // namespace wtl


#endif // WTL_WINDOW_SKIN_HPP
