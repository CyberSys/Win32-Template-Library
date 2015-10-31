//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\gdi\DeviceContext.hpp
//! \brief Encapsulates GDI device contexts
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_DEVICE_CONTEXT_HPP
#define WTL_DEVICE_CONTEXT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/EnumCast.hpp"                 //!< EnumCast
#include "wtl/casts/NativeCast.hpp"               //!< NativeCast
#include "wtl/utils/Handle.hpp"                   //!< Handle
#include "wtl/utils/CharArray.hpp"                //!< CharArray
#include "wtl/utils/LengthOf.hpp"                 //!< LengthOf
#include "wtl/utils/String.hpp"                   //!< String
#include "wtl/traits/DeviceContextTraits.hpp"     //!< HDeviceContext
#include "wtl/traits/BrushTraits.hpp"             //!< HBrush
#include "wtl/traits/FontTraits.hpp"              //!< HFont
#include "wtl/traits/PenTraits.hpp"               //!< HPen
#include "wtl/platform/Colours.hpp"               //!< Colours
#include "wtl/platform/DrawingFlags.hpp"          //!< DrawTextFlags
#include "wtl/utils/Rectangle.hpp"                //!< Rectangle
#include "wtl/utils/Point.hpp"                    //!< Point
#include "wtl/utils/Size.hpp"                     //!< Size
#include "wtl/utils/Triangle.hpp"                 //!< Triangle
#include "wtl/utils/Queue.hpp"                    //!< Queue

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct ObjectStack - Encapsulates management of device context drawing objects 
  //! 
  //! \tparam OBJ - Drawing object handle type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename OBJ>
  struct ObjectStack 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef type - Define own type
    using type = ObjectStack<OBJ>;

    //! \typedef handle_t - Object handle type
    using handle_t = OBJ;

    //! \typedef native_t - Native handle type
    using native_t = typename handle_t::native_t;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HDeviceContext    DC;      //!< Device context handle
    Queue<OBJ>        Items;   //!< Previous handles

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::ObjectStack
    //! Selects a brush into the device, and preserves the old one
    //! 
    //! \param[in] brush - Brush 
    /////////////////////////////////////////////////////////////////////////////////////////
    ObjectStack(const HDeviceContext& dc) : DC(dc)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::~ObjectStack
    //! Empties the stack upon destruction
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~ObjectStack()
    {
      clear();
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(ObjectStack);      //!< Cannot be copied
    ENABLE_MOVE(ObjectStack);       //!< Can be moved
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::empty const
    //! Query whether the stack is empty
    //! 
    //! \return bool - True iff empty
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Items.empty();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::empty const
    //! Query whether the stack is empty
    //! 
    //! \return bool - True iff empty
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear() 
    {
      // Pop until empty
      while (!empty())
        pop();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::push
    //! Select an object into the device, and preserves the old one
    //! 
    //! \param[in] obj - Object handle
    /////////////////////////////////////////////////////////////////////////////////////////
    void push(const handle_t& obj)
    {
      // Select new object and store old one
      Items.push( select(obj) );
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::pop
    //! Discards the current object and replaces it with the previous one
    /////////////////////////////////////////////////////////////////////////////////////////
    void pop()
    {
      select(Items.peek());
      Items.pop();
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // ObjectStack::select
    //! Selects an object into the attached device context
    //! 
    //! \param[in] obj - Object handle
    //! \return handle_t - Weak reference to previously selected object
    /////////////////////////////////////////////////////////////////////////////////////////
    handle_t select(const handle_t& obj)
    {
      // Wrap in weak reference
      return handle_t((native_t)::SelectObject(DC, obj), AllocType::WeakRef);
    }
  };

  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct DeviceContext - Encapsulates a device context
  /////////////////////////////////////////////////////////////////////////////////////////
  struct DeviceContext : ObjectStack<HBrush>, 
                         ObjectStack<HPen>,
                         ObjectStack<HFont>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \var ScreenDC - Screen device context
    static DeviceContext  ScreenDC;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    HDeviceContext   Handle;    //!< DC Handle
      
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::DeviceContext
    //! Create device context from a native handle
    //! 
    //! \param[in] dc - Native device context handle
    /////////////////////////////////////////////////////////////////////////////////////////
    DeviceContext(HDC hdc) : DeviceContext(HDeviceContext(hdc, AllocType::WeakRef))
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::DeviceContext
    //! Create device context
    //! 
    //! \param[in] dc - Device context handle
    /////////////////////////////////////////////////////////////////////////////////////////
    DeviceContext(const HDeviceContext& dc) : ObjectStack<HBrush>(dc),
                                              ObjectStack<HPen>(dc),
                                              ObjectStack<HFont>(dc),
                                              Handle(dc)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(DeviceContext);      //!< Cannot be copied
    ENABLE_MOVE(DeviceContext);       //!< Can be moved
    ENABLE_POLY(DeviceContext);      //!< Can be polymorphic
    
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::getStock
    //! Get a handle to a stock drawing object
    //! 
    //! \return Handle<T> - Weak reference to stock object handle 
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    static ::wtl::Handle<T> getStock(StockObject obj)
    {
      return (T)::GetStockObject(enum_cast(obj));
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::getFont
    //! Creates a font scaled for this device context
    //! 
    //! \param[in] name - Font name
    //! \param[in] points - Size in points
    //! \param[in] weight - Emboldened weight
    //! \param[in] italic - Italicised
    //! \param[in] underline - Underlined
    //! \param[in] quality - Output quality
    //! \param[in] charSet - Character set
    //! \return HFont - Handle to new font
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    HFont getFont(const CharArray<ENC,LEN>& name, int32_t points, FontWeight weight = FontWeight::Normal, bool italic = false, bool underline = false, FontQuality quality = FontQuality::AntiAliased, FontCharSet charSet = FontCharSet::Default)
    {
      return HFont(name, getFontHeight(points), weight, italic, underline, false, charSet, quality);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::getFontHeight
    //! Calculates the height of a font
    //! 
    //! \param[in] points - Size in points
    //! \return int32_t - Height in twips
    /////////////////////////////////////////////////////////////////////////////////////////
    int32_t getFontHeight(int32_t points)
    {
      // Query logical pixel height & convert
      if (auto logPixels = ::GetDeviceCaps(Handle, LOGPIXELSY))
        return -::MulDiv(points, logPixels, 72);

      // Failure
      //throw platform_error(HERE, "Unable to query device caps"); 
      return 0;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::clear
    //! Restores device to original state by removing all previously selected objects, if any
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      ObjectStack<HFont>::clear();
      ObjectStack<HBrush>::clear();
      ObjectStack<HPen>::clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::ellipse
    //! Draws a filled ellipse with the current brush and pen
    //! 
    //! \param[in] rc - Area of ellipse defined as a rectangle
    //!
    //! \throw wtl::platform_error - Unable to draw ellipse
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void  ellipse(const Rect<T>& rc)
    {
      // Fill & outline ellipse 
      if (::Ellipse(Handle, rc.left, rc.top, rc.right, rc.bottom) == False)
        throw platform_error(HERE, "Unable to draw ellipse");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::fill
    //! Fills a rectangle interior with the current brush
    //! 
    //! \param[in] rc - Drawing rectangle
    //!
    //! \throw wtl::platform_error - Unable to fill rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void  fill(const Rect<T>& rc)
    {
      // Fill target rectangle with current brush
      if (::FillRect(Handle, rc, (HBRUSH)::GetCurrentObject(Handle, enum_cast(DrawObjectType::Brush))) == False)
        throw platform_error(HERE, "Unable to fill rect");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::fill
    //! Fills a rectangle interior with a custom brush
    //! 
    //! \param[in] rc - Drawing rectangle
    //! \param[in] brush - Custom brush 
    //!
    //! \throw wtl::platform_error - Unable to fill rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void  fill(const Rect<T>& rc, const HBrush& brush)
    {
      // Fill target rectangle with custom brush
      if (::FillRect(Handle, &native_cast(rc), brush) == False)
        throw platform_error(HERE, "Unable to fill custom rect");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::measure
    //! Measure text using the current font
    //! 
    //! \tparam ENC - Character encoding
    //!
    //! \param[in] const &txt - String
    //! \return SizeL - Size in pixels
    //!
    //! \throw wtl::platform_error - Unable to measure text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    SizeL measure(const String<ENC>& txt)
    {
      SizeL sz;   //!< Text size

      // Measure text
      if (choose<ENC>(::GetTextExtentPoint32A,::GetTextExtentPoint32W)(Handle, txt.c_str(), txt.size(), &native_cast(sz)) == False)
        throw platform_error(HERE, "Unable to measure text");

      return sz;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::measure
    //! Measure text using the current font
    //! 
    //! \tparam CHR - Character type
    //! \tparam ENC - [optional] Character encoding
    //!
    //! \param[in] const* txt - Null terminated string
    //! \return SizeL - Size in pixels
    //!
    //! \throw wtl::platform_error - Unable to measure text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHR, Encoding ENC = default_encoding<CHR>::value>
    SizeL measure(const CHR* txt)
    {
      SizeL sz;   //!< Text size

      // Measure text
      if (choose<ENC>(::GetTextExtentPoint32A,::GetTextExtentPoint32W)(Handle, txt, strlen(txt), &native_cast(sz)) == False)
        throw platform_error(HERE, "Unable to measure text");

      return sz;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::pop
    //! Discards the current object and replaces it with the previous one
    //! 
    //! \tparam OBJ - Object type
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    void pop()
    {
      ObjectStack<OBJ>::pop();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::push
    //! Select an object into the device, and preserves the old one
    //! 
    //! \tparam OBJ - Object type
    //! \param[in] const& obj - Mutable object reference
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    void push(const OBJ& obj)
    {
      ObjectStack<OBJ>::push(obj);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::rect
    //! Draws a filled rectangle with the current brush and pen
    //! 
    //! \param[in] rc - Drawing rectangle
    //!
    //! \throw wtl::platform_error - Unable to draw rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void  rect(const Rect<T>& rc)
    {
      // Outline target rectangle with current pen
      if (::Rectangle(Handle, rc.left, rc.top, rc.right, rc.bottom) == False)
        throw platform_error(HERE, "Unable to draw rectangle");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::polygon
    //! Draws a filled polygon with the current brush and pen
    //! 
    //! \param[in] rc - Drawing rectangle in which to centre the ellipse
    //!
    //! \throw wtl::platform_error - Unable to draw polygon
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LENGTH>
    void  polygon(::POINT (&points)[LENGTH])
    {
      // Fill & outline polygon
      if (::Polygon(Handle, points, lengthof(points)) == False)
        throw platform_error(HERE, "Unable to draw polygon");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::triangle
    //! Draws a filled triangle with the current brush and pen
    //! 
    //! \param[in] rc - Drawing rectangle
    //!
    //! \throw wtl::platform_error - Unable to draw rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void  triangle(const Triangle<T>& triangle)
    {
      // Draw triangle with current pen and brush
      if (::Polygon(Handle, triangle, 3) == False)
        throw platform_error(HERE, "Unable to draw triangle");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::set
    //! Changes the background drawing mode
    //! 
    //! \param[in] mode - New drawing mode
    //! \return DrawingMode - Previous drawing mode
    //! 
    //! \throw wtl::platform_error - Unable to set drawing mode
    /////////////////////////////////////////////////////////////////////////////////////////
    DrawingMode set(DrawingMode mode)
    {
      // Change background drawing mode
      if (auto prev = ::SetBkMode(Handle, enum_cast(mode)))
        return static_cast<DrawingMode>(prev);

      // Failed
      throw platform_error(HERE, "Unable to set drawing mode");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::setBackColour
    //! Changes the background colour (used for text backgrounds and non-solid pen backgrounds)
    //! 
    //! \param[in] col - New colour
    //! \return Colour - Previous colour
    //! 
    //! \throw wtl::platform_error - Unable to set colour
    /////////////////////////////////////////////////////////////////////////////////////////
    Colour  setBackColour(Colour col)
    {
      // Change background colour
      auto prev = static_cast<Colour>(::SetBkColor(Handle, enum_cast(col)));
      if (prev != Colour::Invalid)
        return prev;

      // Failed
      throw platform_error(HERE, "Unable to set background colour");
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::setTextColour
    //! Changes the current text colour (Used for text foregrounds)
    //! 
    //! \param[in] col - New colour
    //! \return Colour - Previous colour
    //! 
    //! \throw wtl::platform_error - Unable to set colour
    /////////////////////////////////////////////////////////////////////////////////////////
    Colour  setTextColour(Colour col)
    {
      // Change text colour
      auto prev = static_cast<Colour>(::SetTextColor(Handle, enum_cast(col)));
      if (prev != Colour::Invalid)
        return prev;

      // Failed
      throw platform_error(HERE, "Unable to set text colour");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::write
    //! Writes text into a rectangle
    //! 
    //! \tparam ENC - Character encoding
    //!
    //! \param[in] txt - Text
    //! \param[in] rc - Drawing rectangle
    //! \param[in] flags - Drawing flags
    //! \return int32_t - Height of the text in logical units iff successful. (If DT_VCENTER or DT_BOTTOM is specified then the offset from lpRect->top to the bottom of the drawn text)
    //!                 Zero upon failure.
    //!
    //! \throw wtl::platform_error - Unable to draw text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    int32_t write(const String<ENC>& txt, RectL& rc, DrawTextFlags flags = DrawTextFlags::Left|DrawTextFlags::VCentre)
    {
      // Draw text
      if (int32_t height = choose<ENC>(::DrawTextA,::DrawTextW)(Handle, txt.c_str(), txt.size(), &native_cast(rc), enum_cast(flags)))
        return height;

      // Failure
      throw platform_error(HERE, "Unable to draw text");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // DeviceContext::write
    //! Writes text into a rectangle
    //! 
    //! \param[in] txt - Text
    //! \param[in] len - Text length, in characters
    //! \param[in] rc - Drawing rectangle
    //! \param[in] flags - Drawing flags
    //! \return int32_t - Height of the text in logical units iff successful. (If DT_VCENTER or DT_BOTTOM is specified then the offset from lpRect->top to the bottom of the drawn text)
    //!                 Zero upon failure.
    //!
    //! \throw wtl::platform_error - Unable to draw text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename CHR>
    int32_t write(const CHR* txt, int32_t len, RectL& rc, DrawTextFlags flags = DrawTextFlags::Left|DrawTextFlags::VCentre)
    {
      // Draw text
      if (int32_t height = choose<default_encoding<CHR>>(::DrawTextA,::DrawTextW)(Handle, txt, len, &native_cast(rc), enum_cast(flags)))
        return height;

      // Failure
      throw platform_error(HERE, "Unable to draw text");
    }
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator +=
  //! Select a drawing object into a device context
  //! 
  //! \tparam OBJ - Drawing object handle type
  //!
  //! \param[in] &dc - Device context
  //! \param[in] const& obj - Immutable drawing object reference
  //! \return DeviceContext& - Returns 'dc'
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename OBJECT>
  DeviceContext& operator+= (DeviceContext& dc, const OBJECT& obj)
  {
    dc.push(obj);
    return dc;
  }

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator +=
  //! Changes the background drawing mode of a device context
  //! 
  //! \param[in] mode - New drawing mode
  //! \return DeviceContext& - Returns 'dc'
  //! 
  //! \throw wtl::platform_error - Unable to set drawing mode
  /////////////////////////////////////////////////////////////////////////////////////////
  inline DeviceContext& operator+= (DeviceContext& dc, const DrawingMode mode)
  {
    dc.set(mode);
    return dc;
  }


}

#endif // WTL_DEVICE_CONTEXT_HPP
