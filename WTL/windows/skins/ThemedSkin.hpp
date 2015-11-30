//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\skins\ThemedSkin.hpp
//! \brief Provides a themed look'n'feel control renderer
//! \date 8 November 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SKIN_FACTORY_HPP
#define WTL_SKIN_FACTORY_HPP

#include <wtl/WTL.hpp>
#include <WTL/windows/WindowSkin.hpp>             //!< IWindowSkin

//! \namespace wtl - Windows template library
namespace wtl 
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ThemedSkin - Renders standard controls using a 'themed' look n feel
  //! 
  //! \tparam ENC - Message character encoding 
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct ThemedSkin : IWindowSkin<ENC>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = ThemedSkin<ENC>;
  
    //! \alias base - Define base type
    using base = IWindowSkin<ENC>;
    
    //! \var encoding - Inherit character encoding
    static constexpr Encoding  encoding = base::encoding;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    //! \var Instance - Temporary singleton
    static type Instance;

    // ------------------------------------ CONSTRUCTION ------------------------------------
  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::ThemedSkin
    //! Control singleton instance and set as default window skin
    /////////////////////////////////////////////////////////////////////////////////////////
    ThemedSkin()
    {
      SkinFactory<encoding>::set(*this);
    }

    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
		DISABLE_COPY(ThemedSkin);			//!< Singleton
		DISABLE_MOVE(ThemedSkin);			//!< Singleton
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::get
    //! Get the singleton instance
    //! 
    //! \return type& - Reference to singleton
    /////////////////////////////////////////////////////////////////////////////////////////
    static type& get()
    {
      return Instance;
    }
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Draws a standard button control
    //! 
    //! \param[in,out] &btn - Button to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(Button<ENC>& btn, DeviceContext& dc, const RectL& rc) const override
    {
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Draws a standard checkbox control
    //! 
    //! \param[in,out] &chk - CheckBox to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(CheckBox<ENC>& chk, DeviceContext& dc, const RectL& rc) const override
    {
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::draw const
    //! Fallback override for drawing a window
    //! 
    //! \param[in,out] &wnd - Window to be drawn
    //! \param[in,out] &dc - Output device context
    //! \param[in] const &rc - Drawing rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    void draw(Window<ENC>& wnd, DeviceContext& dc, const RectL& rc) const override
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Measures a standard button control 
    //! 
    //! \param[in,out] &btn - Button to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(Button<ENC>& btn, DeviceContext& dc) const override
    {
      return {0,0};
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Measures a standard CheckBox control 
    //! 
    //! \param[in,out] &btn - CheckBox to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(CheckBox<ENC>& chk, DeviceContext& dc) const override
    {
      return {0,0};
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ThemedSkin::measure const
    //! Fallback override for measuring a window
    //! 
    //! \param[in,out] &btn - Window to be measured
    //! \param[in,out] &dc - Output device context
    //! \return SizeL - Required size
    /////////////////////////////////////////////////////////////////////////////////////////
    SizeL measure(Window<ENC>& wnd, DeviceContext& dc) const override
    {
      return {0,0};
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \var ThemedSkin<ENC>::Instance - Singleton instance
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  ThemedSkin<ENC>  ThemedSkin<ENC>::Instance;


} // namespace wtl


#endif // WTL_SKIN_FACTORY_HPP
