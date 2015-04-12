////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Widget.hpp
//! \brief Simple shell widgets for the console
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WIDGET_HPP
#define WTL_WIDGET_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace GLM
{
  ////////////////////////////////////////////////////////////////////////////////
  // \class Point - Wrapper for Win32 POINT structure
  ////////////////////////////////////////////////////////////////////////////////
  struct Point : public ::POINT
  {
    ////////////////////////////////////////////////////////////////////////////////
    // Point::Point
    //! Creates point
    //! \param[in] x - x co-ordinate
    //! \param[in] y - y co-ordinate
    ////////////////////////////////////////////////////////////////////////////////
    Point(int32 X, int32 Y)
    {
      this->x = X;
      this->y = Y;
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // Point::Point
    //! Creates point from co-ordinate
    //! 
    //! \param[in] const &c - co-ordinate
    ////////////////////////////////////////////////////////////////////////////////
    Point(const Coord& c)
    {
      this->x = c.X;
      this->y = c.Y;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Point::operator+
    //! Add two co-ordinates together
    //! 
    //! \param[in] const &p - Point defining co-ordinate to add
    //! \return Point - Sum of both points
    ////////////////////////////////////////////////////////////////////////////////
    Point operator+(const Point& r) const
    {
      return Point(x+r.x, y+r.y);
    }
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class Rect - Wrapper for Win32 RECT structure
  ////////////////////////////////////////////////////////////////////////////////
  struct Rect : public ::RECT
  {
    // ---------------------- TYPES & CONSTANTS -------------------
  protected:
  
    // ----------------------- CONSTRUCTION -----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect
    //! Create from RECT
    ////////////////////////////////////////////////////////////////////////////////
    Rect(RECT& r) 
    {
      assign(r);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Rect::Rect
    //! Create from RECT
    //! 
    //! \param[in] l - Left
    //! \param[in] t - Top
    //! \param[in] r - Right
    //! \param[in] b - Bottom
    ////////////////////////////////////////////////////////////////////////////////
    Rect(long l, long t, long r, long b)
    {
      RECT rc = {l,t,r,b};
      assign(rc);
    }
	
	  // ------------------------- STATIC ---------------------------

    // ------------------------ ACCESSORS -------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // Rect::bottomLeft const
    //! Get co-ordinate of bottom left corner
    //! \return Point - Co-ordinates of bottom left corner
    ////////////////////////////////////////////////////////////////////////////////
    Point bottomLeft() const { return Point(left, bottom); }

    ////////////////////////////////////////////////////////////////////////////////
    // Rect::height const
    //! Get rectangle height
    //! \return uint32 - Height
    ////////////////////////////////////////////////////////////////////////////////
    uint32 height() const { return bottom - top; }

    ////////////////////////////////////////////////////////////////////////////////
    // Rect::topLeft const
    //! Get co-ordinate of top left corner
    //! \return Point - Co-ordinates of top left corner
    ////////////////////////////////////////////////////////////////////////////////
    Point topLeft() const { return Point(left, top); }

    ////////////////////////////////////////////////////////////////////////////////
    // Rect::width const
    //! Get rectangle width
    //! \return uint32 - Width
    ////////////////////////////////////////////////////////////////////////////////
    uint32 width() const  { return right - left; }

    // ------------------------- MUTATORS -------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // Rect::assign
    //! Assign values from RECT
    //! 
    //! \param[in] const &r - Source rectangle
    ////////////////////////////////////////////////////////////////////////////////
    void assign(const RECT& r)
    {
      left = r.left;
      top = r.top;
      right = r.right;
      bottom = r.bottom;
    }

    // ---------------------- REPRESENTATION ----------------------
  protected:
  };

  
  ////////////////////////////////////////////////////////////////////////////////
  // \class WidgetBase - Abstract non-templated base class for all Console widgets
  ////////////////////////////////////////////////////////////////////////////////
  class WidgetBase 
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::WidgetBase
    //! Create a console widget at the origin
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    WidgetBase(Console& c) : Colour(Console::White|Console::BkBlue),
                             Owner(c), 
                             Position(0,0)
    {}

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::WidgetBase
    //! Create a console widget
    //! 
    //! \param[in,out] &c - Output console
    //! \param[in] const &p - Initial position
    ////////////////////////////////////////////////////////////////////////////////
    WidgetBase(Console& c, const Coord& p) : Colour(Console::White|Console::BkBlue),
                                             Owner(c), 
                                             Position(p)
    {}

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::~WidgetBase
    //! Virtual destructor
    ////////////////////////////////////////////////////////////////////////////////
    virtual ~WidgetBase()
    {}

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::getPosition const
    //! Get current position
    //! 
    //! \return Coord - Position
    ////////////////////////////////////////////////////////////////////////////////
    virtual Coord getPosition() const
    {
      return Position;
    }

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::clear
    //! Erase the entire widget
    ////////////////////////////////////////////////////////////////////////////////
    virtual void clear()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::draw
    //! Draw the entire widget
    ////////////////////////////////////////////////////////////////////////////////
    void draw()
    {
      // Position console
      Console::CursorState s(Owner);
      Owner.setPosition(Position);

      // Set colour + draw
      Owner << Colour;
      onDraw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::move
    //! Move the widget
    //! 
    //! \param[in] const &c - Destination in console co-ordinates
    ////////////////////////////////////////////////////////////////////////////////
    void move(const Coord& c) 
    {
      clear();
      setPosition(c);
      draw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::setColour
    //! Set the colour of the widget without redrawing
    //! 
    //! \param[in] c - Foreground/background colour
    ////////////////////////////////////////////////////////////////////////////////
    void setColour(Cons c)
    {
      Colour = c;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::setPosition
    //! Set the position of the widget without redrawing
    //! 
    //! \param[in] const &c - Destination in console co-ordinates
    ////////////////////////////////////////////////////////////////////////////////
    void setPosition(const Coord& c) 
    {
      Position = c;
      onMoved();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::update
    //! Redraw the animatable components of the widget
    ////////////////////////////////////////////////////////////////////////////////
    void update()
    {
      // Position console
      Console::CursorState s(Owner);
      Owner.setPosition(Position);

      // Set colour + draw
      Owner << Colour;
      onUpdate();
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::onDraw
    //! Draw the entire widget
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    virtual void onDraw() = 0;

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::onMoved
    //! Raised after widget is moved so subclasses can move children
    //! 
    //! /pre Position has been updated, but widget not redrawn
    ////////////////////////////////////////////////////////////////////////////////
    virtual void onMoved()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WidgetBase::onUpdate
    //! Redraw the animatable widget components
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    virtual void onUpdate()
    {
      onDraw();
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    Console&             Owner;       //!< Attached console
    Coord                Position;    //!< Current position
    Cons Colour;      //!< Foreground/background colours
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class Widget - Abstract base class for all Console widgets
  //! 
  //! \tparam WIDTH - Width in characters
  //! \tparam HEIGHT - Height in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH, uint32 HEIGHT>
  class Widget : public WidgetBase
  {
    // ------------------- TYPES & CONSTANTS -------------------
  public:
    //! \enum - Defines dimensions
    enum 
    {
      width = WIDTH,
      height = HEIGHT,
    };

    //! \typedef base - Friendly base class alias
    typedef WidgetBase base;

    //! \struct LineText - Fixed length char array populated with strings of characters
    //! \tparam _WIDTH - Length of the array in characters
    template <uint32 _WIDTH>
    struct LineText : public CharArray<_WIDTH>
    {
      // ---------------------- ACCESSORS ------------------------			

      ////////////////////////////////////////////////////////////////////////////////
      // empty
      //! Query whether string is empty
      ////////////////////////////////////////////////////////////////////////////////
      bool empty() const
      {
        return front() == '\0';
      }

      // ----------------------- MUTATORS ------------------------

      ////////////////////////////////////////////////////////////////////////////////
      // centre
      //! Centres text within the line
      //! 
      //! \param[in] const *str - Text to be centred. (Truncated if necessary)
      ////////////////////////////////////////////////////////////////////////////////
      void centre(const char* str)
      {
        uint32 len = strlen(str); 

        if (len < length)
        {
          // Calculate first character index
          uint32 start = (length-len)/2;

          // Fill gap with spaces
          iterator firstChar = begin()+start;
          std::fill(begin(), firstChar, ' ');

          // Centre text
          StringCchCopyA(&Data[start], length-start, str);

          // Fill remainder with spaces
          std::fill(firstChar+len, end(), ' ');
          back() = '\0';
        }
        else
          StringCchCopyA(Data, length, str);
      }

      ////////////////////////////////////////////////////////////////////////////////
      // clear
      //! Fill the line with spaces
      ////////////////////////////////////////////////////////////////////////////////
      void clear() override
      {
        fill(' ');
      }

      ////////////////////////////////////////////////////////////////////////////////
      // fill
      //! Fills the line with a single character
      //! 
      //! \param[in] ch - Fill character
      ////////////////////////////////////////////////////////////////////////////////
      void fill(char ch)
      {
        fill(begin(), end(), ch);
      }

      ////////////////////////////////////////////////////////////////////////////////
      // fill
      //! Fills a range on the line with a single character
      //! 
      //! \param[in] first - Position of first character in range
      //! \param[in] last - Position after last character in range
      //! \param[in] ch - Fill character
      ////////////////////////////////////////////////////////////////////////////////
      void fill(iterator first, iterator last, char ch)
      {
        std::fill(first, last, ch);
        back() = '\0';
      }

      ////////////////////////////////////////////////////////////////////////////////
      // fill
      //! Sets the first and last character of the line, then fills the remainder
      //! 
      //! \param[in] first - First character
      //! \param[in] mid - Fill character
      //! \param[in] last - Last character
      ////////////////////////////////////////////////////////////////////////////////
      void fill(char first, char mid, char last)
      {
        fill(mid);
        front() = first;
        at(length-2) = last;
        back() = '\0';
      }
    };


    //! \typedef line_t - Line text type
    typedef LineText<WIDTH> line_t;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // Widget::Widget
    //! Create a console widget at the origin
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    Widget(Console& c) : base(c)
    {}

    ////////////////////////////////////////////////////////////////////////////////
    // Widget::Widget
    //! Create a console widget
    //! 
    //! \param[in,out] &c - Output console
    //! \param[in] const &p - Initial position
    ////////////////////////////////////////////////////////////////////////////////
    Widget(Console& c, const Coord& p) : base(c,p)
    {}

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // Widget::getClientRect const
    //! Get client rectangle 
    //! 
    //! \return Rect - Client rectangle
    ////////////////////////////////////////////////////////////////////////////////
    virtual Rect getClientRect() const
    {
      return Rect(Position.X, Position.Y, Position.X+width, Position.Y+height);
    }

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // Widget::clear
    //! Erase the entire widget
    ////////////////////////////////////////////////////////////////////////////////
    void clear() override
    {
      // Prepare empty line
      line_t str;
      str.fill(' ');

      // Erase lines
      Console::CursorState s(Owner);
      for (uint16 i = 0; i < height; ++i)
      {
        Owner.setPosition(Position+Coord(0,i));
        Owner << str.c_str() << Console::Break;
      }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Widget::drawBox const
    //! Draws a filled rectangle with a double-frame to the console
    ////////////////////////////////////////////////////////////////////////////////
    template <uint32 BOX_WIDTH, uint32 BOX_HEIGHT>
    void drawBox(const Coord& pos)
    {
      //! \typedef box_line_t - Line text sized to the width of the box
      typedef LineText<BOX_WIDTH>  box_line_t;

      //! \typedef LineArray - Array of BOX_HEIGHT box_line_t, erased using their clear() method
      typedef StaticArray<box_line_t,BOX_HEIGHT,clear_method>  BoxLineArray;

      // Defines ASCII characters used to create the box
      const unsigned char chFill = ' ',
                          chVert = 0xBA, chHorz = 0xCD, 
                          chTopLeft = 0xC9, chTopRight = 0xBB, 
                          chBtmLeft = 0xC8, chBtmRight = 0xBC;

      // Render box
      BoxLineArray box;
      Console::CursorState s(Owner);
      for (BoxLineArray::iterator l = box.begin(), last = box.end(); l != last; ++l)
      {
        // Generate line text
        switch (l.getIndex())
        {
        case 0:            l->fill(chTopLeft, chHorz, chTopRight); break;
        default:           l->fill(chVert,    chFill, chVert);     break;
        case BOX_HEIGHT-1: l->fill(chBtmLeft, chHorz, chBtmRight); break;
        }
        
        // Draw line
        Owner.setPosition(pos + Coord(0, l.getIndex()));
        Owner << l->c_str() << Console::Break;
      }
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // Widget::setDefaultCursorPosition
    //! Move the console cursor to beneath the window
    //! 
    //! /post Position has been updated
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setDefaultCursorPosition()
    {
      // Move the cursor to beneath the window
      Owner.setPosition(Coord(0, Position.Y + height + 1));
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
  };

  
  
  
  ////////////////////////////////////////////////////////////////////////////////
  // \class TextWidget - Single line text widget
  //! 
  //! \tparam WIDTH - Width in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH>
  class TextWidget : public Widget<WIDTH,1>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::TextWidget
    //! Create a text widget at the origin
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    TextWidget(Console& c) : Widget(c)
    {}

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::assign
    //! Sets the widget text verbatim
    //! 
    //! \param[in] const *str - Widget text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void assign(const char* str)
    {
      StringCchCopyA(Text.c_arr(), line_t::length, str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::setText 
    //! Sets and centres the widget text
    //! 
    //! \param[in] const *str - Widget text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setText(const char* str)
    {
      Text.centre(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::setTextf
    //! Sets and centres the widget text
    //! 
    //! \param[in] const *format - Formatting string
    //! \param[in] args - Variadic arguments
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setTextf(const char* format, ...)
    {
      va_list args;
      va_start(args, format);
      
      // Format text
      setTextvf(format, args);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::setTextv
    //! Sets and centres the widget text
    //! 
    //! \param[in] const *format - Formatting string
    //! \param[in] args - Variadic arguments
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setTextvf(const char* format, va_list args)
    {
      char buf[WIDTH];
      
      // Format + set text
      StringCchVPrintfA(buf, WIDTH, format, args);
      Text.centre(buf);
    }

  protected:
   ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::onDraw
    //! Draw the entire widget text
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Draw line
      Owner << Text.c_str() << Console::Break;
    }
    // -------------------- REPRESENTATION ---------------------
  protected:
    line_t  Text;     //!< Backing store for line text
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class LabelWidget - Displays a labelled value on a single line
  //! 
  //! \tparam WIDTH - Width in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH>
  class LabelWidget : public TextWidget<WIDTH>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // LabelWidget::TextWidget
    //! Create a label widget at the origin
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    LabelWidget(Console& c) : TextWidget(c), 
                              Colour(Console::Bold|Console::Yellow|Console::BkBlue)
    {}

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // LabelWidget::setLabel
    //! Sets the label name, but does not redraw
    //! 
    //! \param[in] const *str - Label name
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setLabel(const char* str)
    {
      Text.assign(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // LabelWidget::setInt
    //! Sets the value to an integer but does not redraw
    //! 
    //! \param[in] const val - Integer value
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setInt(const int32 val)
    {
      char num[16];
      ValueText.assign(itoa(val, num, 10));
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // LabelWidget::setBool
    //! Sets the value to a boolean but does not redraw
    //! 
    //! \param[in] const val - Boolean value
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setBool(const bool val)
    {
      ValueText.assign(val ? "true" : "false");
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // LabelWidget::setString
    //! Sets the value to a string but does not redraw
    //! 
    //! \param[in] const *val - String value
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setString(const char* val)
    {
      ValueText.assign(val);
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // LabelWidget::setValueColour
    //! Sets the value colour, but does not redraw
    //! 
    //! \param[in] m - Console colour
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setValueColour(Cons m)
    {
      Colour = m;
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // TextWidget::onDraw
    //! Draw the entire widget text
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Draw 'Label: <value>'
      Owner << Text.c_str() << ": " << Colour << ValueText.c_str() << Console::Break;
    }
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    Cons  Colour;       //!< Label value colour
    line_t                ValueText;    //!< Label value text
  };



  ////////////////////////////////////////////////////////////////////////////////
  // \class ProgressBarWidget - Progress bar widget
  //! 
  //! \tparam WIDTH - Width in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH>
  class ProgressBarWidget : public TextWidget<WIDTH>
  {
    // ------------------- TYPES & CONSTANTS -------------------
    

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::ProgressBarWidget
    //! Create widget at the origin with initial range 0 -> 100 and value 0.
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    ProgressBarWidget(Console& c) 
      : TextWidget(c),
        Value(0),
        LowerBound(0),
        UpperBound(100)
    {
      setProgress(0);
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::getProgress
    //! Get the current progress as a percentage expressed between 1 and 0
    //! 
    //! \return float - 0.0f <= value <= 1.0f
    ////////////////////////////////////////////////////////////////////////////////
    float getProgress() const
    {
      return static_cast<float>(Value - LowerBound) / (UpperBound - LowerBound);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::getProgressValue
    //! Get the current raw progress value 
    //! 
    //! \return uint32 - Value
    ////////////////////////////////////////////////////////////////////////////////
    uint32 getProgressValue() const
    {
      return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::getLowerBound
    //! Get the lower bound
    //! 
    //! \return uint32 - Lower bound
    ////////////////////////////////////////////////////////////////////////////////
    uint32 getLowerBound() const
    {
      return LowerBound;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::getUpperBound
    //! Get the upper bound
    //! 
    //! \return uint32 - Upper bound
    ////////////////////////////////////////////////////////////////////////////////
    uint32 getUpperBound() const
    {
      return UpperBound;
    }

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::setProgress
    //! Changes the progress value without redrawing
    //! 
    //! \param[in] v - Progress value 
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setProgress(uint32 v)
    {
      // Validate params
      if (v < LowerBound || v > UpperBound)
        return;

      // Generate text
      Value = v;
      setProgressText();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::setRange
    //! Changes the upper/lower range without redrawing. 
    //! (Enforces new range on the progress value)
    //! 
    //! \param[in] lower - Lower bound
    //! \param[in] upper - Upper bound
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setRange(uint32 lower, uint32 upper)
    {
      if (lower >= upper)
        return;

      // Update range. Limit value.
      LowerBound = lower;
      UpperBound = upper;
      Value = wtl::range(LowerBound, Value, UpperBound);

      // Update text
      setProgress(Value);
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressBarWidget::setProgressText
    //! Generates but does not draw the progress bar text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setProgressText()
    {
      const uint8 chComplete = 219, chRemaining = 178;

      // Calculate length of completed section in characters
      uint32 bars = static_cast<uint32>(getProgress() * width);

      // Set completed/remaining characters separately
      std::fill(Text.begin(), Text.begin()+bars, chComplete);
      std::fill(Text.begin()+bars, Text.end(), chRemaining);
      Text.back() = '\0';
    }
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    uint32  Value,         //!< Current value
            LowerBound,    //!< Value range lower bound
            UpperBound;    //!< Value range upper bound
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class WindowWidget - Draws a window with a caption to the console
  //! 
  //! \tparam WIDTH - Width in characters
  //! \tparam HEIGHT - Height in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH, uint32 HEIGHT>
  class WindowWidget : public Widget<WIDTH,HEIGHT>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  protected:
    //! \enum - Define constants
    enum 
    { 
      border_width = 2,                     // Width of each border, in characters
      total_border = 2*border_width,        // Cumulative width of both borders
      client_width = width-total_border,    // Width of the client rectangle
      client_height = height-total_border,  // Height of the client rectangle
    };

    //! \typedef TextLabel - Text label as wide as the window, minus a 2 character border
    typedef TextWidget<client_width>  TextLabel;

    //! \typedef WindowLineArray - Array of 'height' x line_t, erased using their clear() method
    typedef StaticArray<line_t, height, clear_method>  WindowLineArray;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::WindowWidget
    //! Creates a window widget at the origin
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    WindowWidget(Console& c) : Widget(c), Caption(c)
    {
      // Position the caption
      onMoved();
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::getClientRect const
    //! Get client rectangle which excludes the widget border and caption
    //! 
    //! \return Rect - Client rectangle
    ////////////////////////////////////////////////////////////////////////////////
    Rect getClientRect() const override
    {
      return Rect(Position.X+1, Position.Y+border_width+1, 
                  Position.X+width-border_width, Position.Y+height-border_width);
    }

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::setCaption
    //! Sets the window caption
    //! 
    //! \param[in] const str* - Caption text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setCaption(const char* str)
    {
      Caption.setText(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::setCaptionf
    //! Sets the window caption
    //! 
    //! \param[in] const *format - Formatting string
    //! \param[in] ... - Variadic arguments
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setCaptionf(const char* format, ...)
    {
      va_list args;
      va_start(args, format);
      Caption.setTextvf(format, args);
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::onChar
    //! Notifies a modal window of character input
    //! 
    //! \param[in] c - Character pressed
    ////////////////////////////////////////////////////////////////////////////////
    virtual void onChar(char c) 
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::onDraw
    //! Draw the entire widget
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Render window
      for (WindowLineArray::iterator l = Lines.begin(), last = Lines.end(); l != last; ++l)
        onDrawLine(l, l.getIndex());

      // Draw caption text
      Caption.draw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::onDrawLine 
    //! Draws a line of the window
    //! 
    //! \param[in] const &l - Line to draw
    //! \param[in] idx - Zero-based line index
    ////////////////////////////////////////////////////////////////////////////////
    virtual void onDrawLine(const typename WindowLineArray::iterator& l, uint32 idx)
    {
      // Defines ASCII characters used to create the box
      const unsigned char chFill = ' ',       chClose = 'X',    chMinimize = '_',
                          chVert = 0xBA,      chHorz = 0xCD, 
                          chTopLeft = 0xC9,   chTopRight = 0xBB, 
                          chBtmLeft = 0xC8,   chBtmRight = 0xBC, 
                          chVertRight = 0xCC, chVertLeft = 0xB9,
                          chHorzDown = 0xCB,  chHorzUp = 0xCA;

      switch (idx)
      {
      // Caption:
      case 0: 
        l->fill(chTopLeft, chHorz, chTopRight); 
        setCaptionSeparator(*l, 1, chHorzDown); 
        setCaptionSeparator(*l, 2, chHorzDown); 
        break;
      case 1: 
        l->fill(chVert, chFill, chVert);     
        setCaptionSeparator(*l, 1, chVert);     
        setCaptionSeparator(*l, 2, chVert);     
        setCaptionButton(*l, 1, chClose);
        setCaptionButton(*l, 2, chMinimize);
        break;
      case 2: 
        l->fill(chVertRight,chHorz, chVertLeft); 
        setCaptionSeparator(*l, 1, chHorzUp); 
        setCaptionSeparator(*l, 2, chHorzUp); 
        break;
      
      // Middle:
      default: 
        l->fill(chVert, chFill, chVert);  
        break;

      // Bottom:
      case height-1: 
        l->fill(chBtmLeft, chHorz, chBtmRight); 
        break;
      }
      
      // Draw line
      Owner.setPosition(Position + Coord(0, idx));
      Owner << l->c_str() << Console::Break;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::onMoved
    //! Raised after widget is moved so subclasses can move children
    //! 
    //! /pre Position has been updated, but widget not redrawn
    ////////////////////////////////////////////////////////////////////////////////
    void onMoved() override
    {
      // Move caption
      Caption.setPosition(Position + Coord(border_width,1));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::setCaptionButton
    //! Sets a character representing a caption button to line text
    //! 
    //! \param[in,out] &str - Line text
    //! \param[in] index - One-based window index
    //! \param[in] ch - Bar character
    ////////////////////////////////////////////////////////////////////////////////
    void setCaptionButton(line_t& str, char index, char ch)
    {
      // Offset separator by two chars
      uint32 idx = line_t::length - (4*index);

      // Set character
      if (idx < line_t::length)
        str[idx] = ch;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // WindowWidget::setCaptionSeparator
    //! Sets a character representing a caption separator to line text
    //! 
    //! \param[in,out] &str - Line text
    //! \param[in] index - One-based window index
    //! \param[in] ch - Bar character
    ////////////////////////////////////////////////////////////////////////////////
    void setCaptionSeparator(line_t& str, char index, char ch)
    {
      // Indent on RHS by 4 chars, accounting for null-term + window border.
      uint32 idx = (line_t::length-2) - (total_border*index);

      // Set character
      if (idx < line_t::length)
        str[idx] = ch;
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    TextWidget<WIDTH-11>  Caption;    //!< Window caption text
    WindowLineArray       Lines;      //!< Lines making up entire window
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class GroupBoxWidget - Draws a window with a caption to the console
  //! 
  //! \tparam WIDTH - Width in characters
  //! \tparam HEIGHT - Height in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH, uint32 HEIGHT>
  class GroupBoxWidget : public Widget<WIDTH,HEIGHT>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  protected:
    //! \enum - Define constants
    enum 
    { 
      border_width = 2,                     // Width of each border, in characters
      total_border = 2*border_width,        // Cumulative width of both borders
      client_width = width-total_border,    // Width of the client rectangle
      client_height = height-total_border,  // Height of the client rectangle
    };

    //! \typedef TextLabel - Text label as wide as the window, minus a 2 character border
    typedef TextWidget<client_width>  TextLabel;

    //! \typedef WindowLineArray - Array of 'height' x line_t, erased using their clear() method
    typedef StaticArray<line_t, height, clear_method>  WindowLineArray;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // GroupBoxWidget::GroupBoxWidget
    //! Creates a group-box widget at the origin
    //! 
    //! \param[in,out] &c - Output console
    ////////////////////////////////////////////////////////////////////////////////
    GroupBoxWidget(Console& c) : Widget(c), Caption(c)
    {
      // Position the caption
      onMoved();
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // GroupBoxWidget::getClientRect const
    //! Get client rectangle which excludes the widget border
    //! 
    //! \return Rect - Client rectangle
    ////////////////////////////////////////////////////////////////////////////////
    Rect getClientRect() const override
    {
      return Rect(Position.X+1, Position.Y+1, Position.X+width-border_width, Position.Y+height-border_width);
    }

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // GroupBoxWidget::setCaption
    //! Sets the group box caption [optional]
    //! 
    //! \param[in] const str* - Caption text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setCaption(const char* str)
    {
      Caption.assign(str);
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // GroupBoxWidget::onDrawLine
    //! Draws a line of the groupbox
    //! 
    //! \param[in] const &l - Line to draw
    //! \param[in] idx - Zero-based line index
    ////////////////////////////////////////////////////////////////////////////////
    void onDrawLine(const typename WindowLineArray::iterator l, uint32 idx)
    {
      // Defines ASCII characters used to create the box
      const unsigned char chFill = ' ',
                          chVert = 0xBA, chHorz = 0xCD, 
                          chTopLeft = 0xC9, chTopRight = 0xBB, 
                          chBtmLeft = 0xC8, chBtmRight = 0xBC;

      // Generate line text
      switch (idx)
      {
      // Top/middle/bottom
      case 0:        l->fill(chTopLeft, chHorz, chTopRight); break;
      default:       l->fill(chVert,    chFill, chVert);     break;
      case height-1: l->fill(chBtmLeft, chHorz, chBtmRight); break;
      }
      
      // Offset + draw
      Owner.setPosition(Position + Coord(0,idx));
      Owner << l->c_str() << Console::Break;
    }

    
    ////////////////////////////////////////////////////////////////////////////////
    // GroupBoxWidget::onDraw
    //! Draw the entire widget
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Render box
      for (WindowLineArray::iterator l = Lines.begin(), last = Lines.end(); l != last; ++l)
        onDrawLine(l, l.getIndex());

      // Draw caption, if any
      Caption.draw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // GroupBoxWidget::onMoved
    //! Raised after widget is moved so subclasses can move children
    //! 
    //! /pre Position has been updated, but widget not redrawn
    ////////////////////////////////////////////////////////////////////////////////
    void onMoved() override
    {
      // Move caption
      Caption.setPosition(Position + Coord(border_width,0));
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    TextWidget<WIDTH-3>  Caption;    //!< Groupbox title text, if any
    WindowLineArray      Lines;      //!< Lines making up entire window
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class OperationWidget - Displays a progress bar within the centre of a small window
  //! Also provides calculation and display of time remaining
  //! 
  //! \tparam WIDTH - Width of the window, in characters
  //! \tparam HEIGHT - Height of the window, in characters
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH, uint32 HEIGHT = 7>
  class OperationWidget : public GroupBoxWidget<WIDTH,HEIGHT>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  public:
    //! \typedef base - Friendly base class alias
    typedef GroupBoxWidget<WIDTH,HEIGHT>  base;

    //! \typedef ProgressBar - Progress bar accounting for border and percentage indicator
    typedef ProgressBarWidget<WIDTH-total_border-5UL> ProgressBar;

    //! \typedef PercentageLabel - Displays progress as a percentage
    typedef TextWidget<4>  PercentageLabel;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::OperationWidget 
    //! Create a progress window widget
    //! 
    //! \param[in,out] &c - Console
    ////////////////////////////////////////////////////////////////////////////////
    OperationWidget(Console& c) : base(c), 
                                  Details(c),
                                  Progress(c),
                                  Remaining(c), 
                                  Percentage(c), 
                                  TimeStart(GetTickCount())
    {
      // Position children
      onMoved();

      // Default text
      Remaining.setText("Remaining: Unknown");
      setDetails("Initializing...");
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::cancel
    //! Cancels the activity, clearing time remaining and stopping progress bar
    //! 
    //! \param[in] const *str - Cancellation details string
    ////////////////////////////////////////////////////////////////////////////////
    virtual void cancel(const char* str)
    {
      // Details
      setDetails(str);
      Details.draw();

      // Update operation text
      Remaining.setText("Operation cancelled");
      Remaining.draw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::complete 
    //! Completes the activity, clearing time remaining and completing the progress bar
    //! 
    //! \param[in] const *str - Completion details string
    ////////////////////////////////////////////////////////////////////////////////
    virtual void complete(const char* str)
    {
      // Details
      setDetails(str);
      Details.draw();

      // Update operation text
      setProgress(Progress.getUpperBound());
      Progress.draw();
      Percentage.draw();

      // Remaining
      Remaining.setText("Operation complete");
      Remaining.draw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::setDetails
    //! Sets the operation details text
    //! 
    //! \param[in] const *str - Details text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setDetails(const char* str)
    {
      if (str)
        Details.setText(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::setProgress
    //! Sets the current progress 
    //! 
    //! \param[in] value - Value to set
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setProgress(uint32 value)
    {
      // Set progress
      Progress.setProgress(value);

      // Set percentage text
      Percentage.setTextf("%d%%", static_cast<int32>(100.0f*Progress.getProgress()) );
    }

    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::setProgress
    //! Updates the operation progress and details text
    //! 
    //! \param[in] progress - Progress value
    //! \param[in] const str* - Details text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setProgress(uint32 progress, const char* str)
    {
      // Update progress
      setProgress(progress);
      
      // Calculate time passed / estimated finish
      float timePassed = (GetTickCount() - TimeStart) / 1000.0f,
            timeRemaining = timePassed / Progress.getProgress() - timePassed;

      // Update remaining + redraw.
      setDetails(str);
      setRemaining(static_cast<uint32>(timeRemaining));
      
      // Redraw changed
      Details.draw();
      Remaining.draw();
      Progress.draw();
      Percentage.draw();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::setRange
    //! Sets the progress range and resets the progress to lower bound
    //! 
    //! \param[in] lower - Lower bound
    //! \param[in] higher - Upper bound
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setRange(uint32 lower, uint32 higher)
    {
      Progress.setRange(lower, higher);
      setProgress(lower);
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::onDraw
    //! Draw the entire widget
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Groupbox
      base::onDraw();

      // Details, Progressbar, remaining text
      Details.draw();
      Progress.draw();
      Remaining.draw();
      Percentage.draw();
    }
  
    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::onMoved
    //! Raised after widget is moved so subclasses can move children
    //! 
    //! /pre Position has been updated, but widget not redrawn
    ////////////////////////////////////////////////////////////////////////////////
    void onMoved() override
    {
      // Position children
      Details.setPosition(Position + Coord(border_width, 1));
      Progress.setPosition(Position + Coord(border_width, 3));
      Percentage.setPosition(Position + Coord(width-border_width-4, 3));
      Remaining.setPosition(Position + Coord(border_width, 5));

      // Move caption
      base::onMoved();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // OperationWidget::setRange
    //! Sets the progress range
    //! 
    //! \param[in] seconds - Number of seconds remaining
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setRemaining(uint32 seconds)
    {
      // Calculate number of seconds/minutes
      uint32 minutes = (seconds/60);
      seconds = (seconds%60);

      // Format
      if (minutes > 0)
        Remaining.setTextf("Remaining: %d minutes, %d seconds", minutes, seconds);
      else
        Remaining.setTextf("Remaining: %d seconds", seconds);
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    ProgressBar     Progress;      //!< Progress bar
    PercentageLabel Percentage;    //!< Progress percentage indicator
    TextLabel       Details,       //!< Details text
                    Remaining;     //!< Time remaining label
    uint32          TimeStart;     //!< Start time in milliseconds
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class HistogramWidget - Displays a series of labeled progress bars 
  //! 
  //! \remarks This widget is limited to being used with the console singleton.
  //! 
  //! \tparam WIDTH - Width of the window, in characters
  //! \tparam METERS - Number of categories to meter
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH, uint32 METERS>
  class HistogramWidget : public GroupBoxWidget<WIDTH,METERS+6>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  public:
    //! \typedef base - base class alias
    typedef GroupBoxWidget<width,height>  base;

    //! \enum - Define constants
    enum 
    { 
      meter_count = METERS,                           // Number of meters
      name_width  = 5,                                // Width of name label, in chars
      axis_width  = 2,                                // Width of y-axis, in chars
      bar_width   = (width-name_width-axis_width-2),  // Width of meter bar
      max_value   = 1000*1000,                        // Maximum displayable value
    };

    //! \struct MeterMode - Meter value display options
    struct MeterMode
    { //! \enum t - Underlying type
      enum t
      {
        Both,        // Display frequency + percentage
        Frequency,   // Display frequency
        Percentage,  // Display percentage
      };
    };

    //! \typedef TitleLabel - Title text widget
    typedef TextWidget<client_width>  TitleLabel;

    //! \typedef AxisText - Text used to display X-axis 
    typedef LineText<bar_width-7>  AxisText;

    //! \typedef LabelText - Text used to display X-axis labels
    typedef LineText<8>  LabelText;

    //! \struct AxisLabelWidget - Default constructible axis label 
    struct AxisLabelWidget : public TextWidget<8>
    {
      // ------------------- TYPES & CONSTANTS -------------------
      
      //! \typedef base - Base class alias
      typedef TextWidget<width>  base;

      // --------------------- CONSTRUCTION ----------------------
    
      ////////////////////////////////////////////////////////////////////////////////
      // AxisLabelWidget::AxisLabelWidget
      //! Create an uninitialised widget attached to console singleton
      ////////////////////////////////////////////////////////////////////////////////
      AxisLabelWidget() : base(theConsole)
      {
        //setColour(Console::Yellow|Console::BkBlack);
      }
    };

    //! \struct NameWidget - Default constructible meter name label
    struct NameWidget : public TextWidget<name_width>
    {
      // ------------------- TYPES & CONSTANTS -------------------
      
      //! \typedef base - Base class alias
      typedef TextWidget<width>  base;

      // --------------------- CONSTRUCTION ----------------------
    
      ////////////////////////////////////////////////////////////////////////////////
      // NameWidget::NameWidget
      //! Create an uninitialised widget attached to console singleton
      ////////////////////////////////////////////////////////////////////////////////
      NameWidget() : base(theConsole)
      {
        //setColour(Console::Yellow|Console::BkBlack);
      }
    };

    //! \struct MeterWidget - Displays a meter bar with a value on the RHS
    struct MeterWidget : public ProgressBarWidget<bar_width>   
    {
      // ------------------- TYPES & CONSTANTS -------------------
    public:
      //! \typedef base - Base class alias
      typedef ProgressBarWidget<width>  base;

      //! \enum - Define component lengths
      enum 
      { 
        value_width = 8,                    // Width of value label (Displays up to 99,999,999)
        bar_width   = (width-value_width),  // Width of meter bar
      };

      // --------------------- CONSTRUCTION ----------------------
    public:
      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::MeterWidget
      //! Create an uninitialised widget attached to console singleton
      ////////////////////////////////////////////////////////////////////////////////
      MeterWidget() : base(theConsole), 
                      DisplayMode(MeterMode::Both),
                      ValueToPlayer(1ULL),
                      ProgressChar(0xDB), 
                      Frequency(0UL),
                      Quantity(0ULL)
      {}

      // ------------------------ STATIC -------------------------

      // ---------------------- ACCESSORS ------------------------			
    public:
      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::getAverage
      //! Get the meter average
      //! 
      //! \return float - Average
      ////////////////////////////////////////////////////////////////////////////////
      float getAverage() const
      {
        return static_cast<float>( static_cast<double>(Quantity) 
                                 / static_cast<double>(Frequency));
      }

      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::getHitRate
      //! Get the meter HitRate
      //! 
      //! \return float - Hit rate
      ////////////////////////////////////////////////////////////////////////////////
      float getHitRate() const
      {
        return static_cast<float>( static_cast<double>(UpperBound)
                                 / static_cast<double>(Frequency) );
      }

      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::getPercentage
      //! Get the meter percentage
      //! 
      //! \return float - Percentage
      ////////////////////////////////////////////////////////////////////////////////
      float getPercentage() const
      {
        return static_cast<float>( static_cast<double>(Quantity) 
                                 / static_cast<double>(ValueToPlayer) 
                                 * 100.0 );
      }

      // ----------------------- MUTATORS ------------------------
    public:
      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::bump
      //! Bumps the meter, but does not redraw
      //! 
      //! \param[in] q - Quantity by which to increase the meter
      ////////////////////////////////////////////////////////////////////////////////
      void bump(uint64 q)
      {
        // Bump meter
        ++Frequency;
        Quantity += q;

        // Update text
        base::setProgress(static_cast<uint32>(Quantity));
        setProgressText();
      }

      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::setDisplayChar
      //! Changes the character used to draw the meter
      //! 
      //! \param[in] c - Meter bar character 
      ////////////////////////////////////////////////////////////////////////////////
      void setDisplayChar(uint8 c)
      {
        // Set char. Update text
        ProgressChar = c;
        setProgressText();
      }

      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::setDisplayStyle
      //! Changes the display mode of the meter value
      //! 
      //! \param[in] m - Meter value display mode
      ////////////////////////////////////////////////////////////////////////////////
      void setDisplayStyle(typename MeterMode::t m)
      {
        // Set mode. Update text
        DisplayMode = m;
        setProgressText();
      }

      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::setProgress
      //! Method is disabled
      //! 
      //! \param[in] v - Ignored
      ////////////////////////////////////////////////////////////////////////////////
      void setProgress(uint32 v) override
      {
        // Nothing
      }

      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::setValueToPlayer
      //! Set the value to player
      //! 
      //! \param[in] m - Meter value display mode
      ////////////////////////////////////////////////////////////////////////////////
      void setValueToPlayer(uint64 v)
      {
        // Set and update
        ValueToPlayer = v;
        setProgressText();
      }

    protected:
      ////////////////////////////////////////////////////////////////////////////////
      // MeterWidget::setProgressText
      //! Generates but does not draw the progress bar text
      ////////////////////////////////////////////////////////////////////////////////
      void setProgressText() override
      {
        // Calculate length of completed section in characters
        uint32 bars = static_cast<uint32>(getProgress() * bar_width);

        // Assign bar characters
        std::fill(Text.begin(), Text.begin()+bars, ProgressChar);

        // Generate meter value
        switch (DisplayMode)
        {
        case MeterMode::Both: 
          StringCchPrintf(&Text[bars], width - bars, " %lu (%.01f%%)", Frequency, getPercentage()); 
          break;

        case MeterMode::Frequency:   
          StringCchPrintf(&Text[bars], width - bars, " %lu", Frequency); 
          break;

        case MeterMode::Percentage: 
          StringCchPrintf(&Text[bars], width - bars, " %.01f%%", getPercentage()); 
          break;
        }
      }

      // -------------------- REPRESENTATION ---------------------
    protected:
      typename MeterMode::t DisplayMode;    //!< Value text display mode
      uint8                 ProgressChar;   //!< Whether to use default or alternate character for progress bar
      uint32                Frequency;      //!< Meter frequency
      uint64                Quantity,       //!< Meter quantity
                            ValueToPlayer;  //!< Value-to-player
    };


  protected:    
    //! \struct MeterBar - Holds meter+name widgets
    struct MeterBar
    {
      // --------------------- CONSTRUCTION ----------------------
    public:
      ////////////////////////////////////////////////////////////////////////////////
      // MeterBar::MeterBar 
      //! Create a histogram widget attached to the console singleton
      ////////////////////////////////////////////////////////////////////////////////
      MeterBar() : Visible(true)
      {}

      // -------------------- REPRESENTATION ---------------------
    public:
      MeterWidget  Meter;
      NameWidget   Name;
      bool         Visible;
    };

    //! \typedef MeterArray - Array of 'meter_count' default-constructed MeterBars
    typedef StaticArray<MeterBar,meter_count,single_use> MeterArray;

    //! \typedef LabelArray - Array of 4 default-constructed AxisLabelWidgets
    typedef StaticArray<AxisLabelWidget,4,single_use> LabelArray;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::HistogramWidget 
    //! Create a histogram widget attached to the console singleton
    //! 
    //! \param[in] numGames - Number of games to play
    //! \param[in] VTP - Value to player
    ////////////////////////////////////////////////////////////////////////////////
    HistogramWidget(uint32 numGames, uint64 VTP)
      : base(theConsole), 
        Title(theConsole)
    {
      // Arrange children
      onMoved();

      // Set defaults limit/title
      setTitle("Untitled");
      setAxisLabels(numGames);

      // Generate x-axis text
      setAxisText();

      // Init meters
      for (uint32 i = 0; i < meter_count; ++i)
      {
        // Set alternating meter bar characters
        Meters[i].Meter.setDisplayChar(i % 2 ? 0xDB : 0xB2);

        // Setup range
        Meters[i].Meter.setValueToPlayer(VTP);
        Meters[i].Meter.setRange(0UL, static_cast<uint32>(VTP));
      }
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			
  
    // ----------------------- MUTATORS ------------------------
  public:    
    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::bumpMeter
    //! Increases the value of a meter, but does not redraw 
    //! 
    //! \param[in] meterIdx - Zero-based meter index
    //! \param[in] quantity - Amount by which to bump meter
    ////////////////////////////////////////////////////////////////////////////////
    virtual void bumpMeter(uint32 meterIdx, uint64 quantity)
    {
      // Validate params
      if (meterIdx >= meter_count)
        return;

      // Update value
      Meters[meterIdx].Meter.bump(quantity);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::setAxisLabels
    //! Sets the x-axis labels 
    //! 
    //! \param[in] vmax - Max value
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setAxisLabels(uint32 vmax)
    {
      // Validate params
      if (vmax > max_value)
        return;

      // Set text of x-axis labels
      Labels[0].assign("0");
      for (LabelArray::iterator l = Labels.begin()+1; l != Labels.end(); ++l)
        l->setTextf("%lu", l.getIndex() * vmax / (LabelArray::length-1));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::setLimit
    //! Sets the maximum value of a single meter
    //! 
    //! \param[in] meterIdx - Zero-based meter index
    //! \param[in] vmax - Max value
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setLimit(uint32 meterIdx, uint32 vmax)
    {
      // Validate params
      if (vmax > max_value || meterIdx >= meter_count)
        return;

      // Set meter widget range
      Meters[meterIdx].Meter.setRange(0, vmax);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::setName
    //! Set the name of a meter, but does not redraw
    //! 
    //! \param[in] meterIdx - Zero-based meter index
    //! \param[in] const *name - Meter name
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setName(uint32 meterIdx, const char* name)
    {
      char sz[name_width+1];

      // Validate params
      if (meterIdx >= meter_count)
        return;

      // Set name (right align)
      StringCchPrintf(sz, name_width+1, "% 5s", name);
      Meters[meterIdx].Name.assign(sz);
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // MeterWidget::setStyle
    //! Changes the display mode of a meter 
    //! 
    //! \param[in] meterIdx - Zero-based meter index
    //! \param[in] m - display mode
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setStyle(uint32 meterIdx, typename MeterMode::t m)
    {
      // Validate params
      if (meterIdx >= meter_count)
        return;

      // Set style
      Meters[meterIdx].Meter.setDisplayStyle(m);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::setTitle
    //! Sets the histogram title, but does not redraw
    //! 
    //! \param[in] const *str - Title text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setTitle(const char* str)
    {
      if (str)
        Title.setText(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::showMeter
    //! Shows/hides a meter, but does not redraw
    //! 
    //! \param[in] meterIdx - Zero-based meter index
    //! \param[in] show - Whether to show/hide
    ////////////////////////////////////////////////////////////////////////////////
    virtual void showMeter(uint32 meterIdx, bool show)
    {
      // Validate params
      if (meterIdx >= meter_count)
        return;

      // Show/hide meter
      Meters[meterIdx].Visible = show;
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::drawHistogram
    //! Draws the histogram
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of histogram drawn
    ////////////////////////////////////////////////////////////////////////////////
    void drawHistogram()
    {
      const uint8 chYAxis = 0xB3,       // Y-axis 
                  chYAxisTop = 0xBF,    // Y-axis top
                  chYAxisBit = 0xB4,    // Y-axis label indicator
                  chCorner = 0xC0;      // X/Y-axis corner 
      
      // Draw visible meters/names
      Coord pos = getClientRect().topLeft();
      for (MeterArray::iterator m = Meters.begin(); m != Meters.end(); ++m)
      {
        // Draw visible meters/names
        if (m->Visible)
        {
          m->Name.draw();
          m->Meter.draw();
        }

        // Position on Y-axis
        Owner.setPosition(m->Meter.getPosition() + Coord(1-axis_width,0));

        // Draw Y-axis character, or label indicator
        Owner << (char)(m->Visible ? chYAxisBit : chYAxis);
      }

      // Draw X-axis
      Owner.setPosition(pos + Coord(name_width+1, meter_count+2));
      Owner << Axis.c_str();

      // Draw X-axis labels
      for (LabelArray::iterator l = Labels.begin(); l != Labels.end(); ++l)
        l->draw();
    }
  
    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::onDraw
    //! Draw the entire widget
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Groupbox
      base::onDraw();
      
      // Title + Histogram
      Title.draw();
      drawHistogram();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::onMoved
    //! Raised after widget is moved so subclasses can move children
    //! 
    //! /pre Position has been updated, but widget not redrawn
    ////////////////////////////////////////////////////////////////////////////////
    void onMoved() override
    {
      //! \enum - Define x-axis segment length, in chars
      enum { segment = AxisText::length/3 };

      // Move groupgox
      base::onMoved();

      // Position title
      Coord pos = getClientRect().topLeft();
      Title.setPosition(pos + Coord(1,0));

      // Position meters
      for (MeterArray::iterator m = Meters.begin(); m != Meters.end(); ++m)
      {
        // Start on 3rd line
        uint32 y = m.getIndex()+2;

        // Position name/meter
        m->Name.setPosition(pos + Coord(0, y));
        m->Meter.setPosition(pos + Coord(name_width+axis_width, y));
      }

      // half of average label length
      const uint8 avg = 4;  

      // Move origin
      pos = Coord(pos.X+name_width+axis_width, getClientRect().bottom);

      // Position labels
      Labels[0].setPosition(pos);
      Labels[1].setPosition(pos + Coord(segment-avg, 0));
      Labels[2].setPosition(pos + Coord(2*segment-avg, 0));
      Labels[3].setPosition(pos + Coord(3*segment-avg-2, 0));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Widget::onUpdate
    //! Redraw the animatable widget components
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    virtual void onUpdate()
    {
      // Draw histogram only
      drawHistogram();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // HistogramWidget::setAxisText
    //! Sets the text used to display x-axis 
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setAxisText()
    {
      const uint8 chAxis = 0xC4,        // X-axis character
                  chAxisBit = 0xC2,     // X-axis label character
                  chAxisEnd = 0xBF,     // X-axis end character
                  chCorner = 0xC0;      // X/Y-axis corner character

      //! \enum - Define x-axis segment length, in chars
      enum { segment = AxisText::length/3 };

      // Generate X-axis text
      for (uint32 i = 0; i < AxisText::length-1; ++i)
        switch (i)
        {
        case 0:                    Axis[i] = chCorner;  break;     // X/Y axis corner
        default:                   Axis[i] = chAxis;    break;     // X axis
        case 1*segment:
        case 2*segment:            Axis[i] = chAxisBit; break;     // X-axis label indicator
        case AxisText::length-2:   Axis[i] = chAxisEnd; break;     // final X-axis label indicator
        }
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    AxisText    Axis;        //!< X-axis text
    LabelArray  Labels;      //!< X-axis labels widgets
    MeterArray  Meters;      //!< Meter widgets
    TitleLabel  Title;       //!< Histogram title
  };

  ////////////////////////////////////////////////////////////////////////////////
  // \class ProgressWindowWidget - Small window containing a title and operation widget
  ////////////////////////////////////////////////////////////////////////////////
  template <uint32 WIDTH, uint32 HEIGHT = 14>
  class ProgressWindowWidget : public WindowWidget<WIDTH,HEIGHT>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  public:
    //! \typedef base - Friendly base class alias
    typedef WindowWidget<width,height> base;

    //! \typedef OperationWnd - Progress window widget
    typedef OperationWidget<client_width> OperationWnd;

    // --------------------- CONSTRUCTION ----------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::ProgressWindowWidget 
    //! Create a unit test progress window widget
    //! 
    //! \param[in,out] &c - Console
    //! \param[in] const lower - Progress lower bound
    //! \param[in] const upper - Progress upper bound
    ////////////////////////////////////////////////////////////////////////////////
    ProgressWindowWidget(Console& c, const uint32 lower, const uint32 upper)
      : base(c), 
        Title(c),
        Operation(c)
    {
      // Set progress range
      Operation.setRange(lower, upper);
    }

    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------
  public:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::cancel
    //! Cancels the activity, clearing time remaining and stopping progress bar
    //! 
    //! \param[in] const *str - Cancellation details string
    ////////////////////////////////////////////////////////////////////////////////
    virtual void cancel(const char* str)
    {
      Operation.cancel(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::complete 
    //! Completes the activity, clearing time remaining and completing the progress bar
    //! 
    //! \param[in] const *str - Completion details string
    ////////////////////////////////////////////////////////////////////////////////
    void complete(const char* str)
    {
      Operation.complete(str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::setProgress 
    //! Set progress and details text
    //! 
    //! \param[in] progress - Current progress
    //! \param[in] const *str - Details text
    ////////////////////////////////////////////////////////////////////////////////
    virtual void setProgress(uint32 progress, const char* str)
    {
      Operation.setProgress(progress, str);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::setTitle
    //! Set the title
    //! 
    //! \param[in] const *str - Title
    ////////////////////////////////////////////////////////////////////////////////
    void setTitle(const char* str)
    {
      if (str)
        Title.setText(str);
    }

  protected:
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::onDraw
    //! Draw the entire widget
    //! 
    //! /pre Console cursor has been positioned at widget
    //! /post All components of widget drawn
    ////////////////////////////////////////////////////////////////////////////////
    void onDraw() override
    {
      // Base
      base::onDraw();

      // Draw children
      Title.draw();
      Operation.draw();
    }
  
    ////////////////////////////////////////////////////////////////////////////////
    // ProgressWindowWidget::onMoved
    //! Raised after widget is moved so subclasses can move children
    //! 
    //! /pre Position has been updated, but widget not redrawn
    ////////////////////////////////////////////////////////////////////////////////
    void onMoved() override
    {
      // Move base children
      base::onMoved();

      // Reposition children
      Title.setPosition(Position + Coord(1,4));
      Operation.setPosition(Position + Coord(border_width,6));
    }

    // -------------------- REPRESENTATION ---------------------
  protected:
    TextLabel    Title;        //!< Title label
    OperationWnd Operation;    //!< Progress window
  };

  
} // namespace wtl
#endif // WTL_WIDGET_HPP

