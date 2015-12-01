//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\RichTextWriter.hpp
//! \brief Provides an RTF stream writer
//! \brief This file is now out of date and needs updating to support the newer stream mechanics
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_RICH_TEXT_WRITER_HPP
#define WTL_RICH_TEXT_WRITER_HPP

#include <wtl/WTL.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  enum class Alignment { Left, Centre, Right, Justify };

  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct RichTextWriter - Writes rich text to an output stream
  //! 
  //! \tparam STREAM - Output stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct RichTextWriter : TextWriter<STREAM>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \typedef base - Base class alias
    typedef TextWriter<STREAM> base;

    //! \typedef delegate_t - Defines signature of output overload
    typedef RichTextWriter& (*delegate_t)(RichTextWriter&);
    
    //! \typedef distance_t - Stream distance type
    typedef typename STREAM::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename STREAM::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename STREAM::position_t position_t;
    
    //! \typedef ColourTable - Colour table type
    typedef std::vector<COLORREF> ColourTable;

    //! \typedef stream_t - Stream type
    typedef STREAM stream_t;

  protected:
    //! Unspecified colour sentinel value
    const COLORREF  COLOUR_NONE = 0xff000000;

    const static UINT BUFFER_SIZE = 256;

    const static COLORREF Black   = RGB(0,0,0);
    const static COLORREF Blue    = RGB(0,0,210);
    const static COLORREF Cyan    = RGB(0,210,210);
    const static COLORREF Green   = RGB(0,210,0);
    const static COLORREF Magenta = RGB(210,0,210);
    const static COLORREF Red     = RGB(255,0,0);
    const static COLORREF Yellow  = RGB(249,192,0);
    const static COLORREF White   = RGB(255,255,255);

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichTextWriter::RichTextWriter 
    //! Create a text writer and initialise the output stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS> explicit
    RichTextWriter(ARGS&&... args) : base(std::forward<ARGS>(args)...),
                                          ForeColour(COLOUR_NONE), 
                                          BackColour(COLOUR_NONE), 
                                          Closed(true),
                                          Bold(false), 
                                          Italic(false), 
                                          Underline(false),
                                          Buffered(0)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(RichTextWriter);        //!< Copy semantics determined by stream type
    ENABLE_MOVE(RichTextWriter);        //!< Move semantics determined by stream type
    ENABLE_POLY(RichTextWriter);        //!< Can be polymorphic
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::close()
    //! Appends the RTF footer and closes the writer
    //!
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void close()
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Writer footer
      writeFooter();

      // Flush+Close output
      flush();
      Stream.close();
      Closed = true;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::flush()
    //! Flushes the output stream
    //!
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void flush()
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Flush output stream
      Stream.flush();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::open
    //! Initialises the writer
    //!
    //! \param[in] const* font - Font name
    //! \param[in] size - Font size in points
    //! \param[in] const& cols - List of colours
    //!
    //! \throw wtl::logic_error - [Debug only] Writer is already open
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  open(const char* font, UINT size, const std::list<COLORREF>& cols)
    {
      // Check stream is closed
      if (!Closed)
        throw wtl::logic_error(HERE, "Writer is already open");
      Closed = false;

      // Copy colours
      for (COLORREF c : cols)
        Colours.push_back(c);

      // Write header
      writeHeader(font);
      setFontSize(size);
    }


    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::resetParagraph
    //! Resets paragraph properties
    //!
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  resetParagraph()
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Write
      writeString("\\pard ");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setAlignment
    //! Sets paragraph alignment
    //!
    //! \param[in] al - Alignment
    //!
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  setAlignment(Alignment al)
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // set alignment
      switch (al)
      {
      case Alignment::Left:      writeString("\\ql ");  break;
      case Alignment::Centre:    writeString("\\qc ");  break;
      case Alignment::Right:     writeString("\\qr ");  break;
      case Alignment::Justify:   writeString("\\qj ");  break;
      }
    }
    
    


    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setBackColour
    //! Sets the background colour
    //!
    //! \param[in] c - The colour
    //!
    //! \throw wtl::invalid_argument - Unrecognised colour
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   setBackColour(COLORREF c)
    {
      CHR buf[10];

      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Skip if colour already set
      if (BackColour == c)
        return;

      // Lookup colour ID
      for (UINT id = 0; id < Colours.size(); id++)
        if (Colours[id] == c)
        {  // Write colour change
          StringCchPrintfA(buf, 10, "\\cb%d ", id + 1);
          writeString(buf);
          BackColour = c;
          return;
        }

      // Unknown colour
      throw wtl::invalid_argument(HERE, "No such colour in the colour table");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setBold
    //! Sets or clears bold text
    //! 
    //! \param[in] b - True to set, False to clear
    //!
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  setBold(bool b)
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Skip if bold already set
      if (Bold == b)
        return;

      // Set/Clear Bold
      Bold = b;
      writeString(Bold ? "\\b " : "\\b0 ");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setForeColour
    //! Sets the text colour
    //!
    //! \param[in] c - The colour
    //!
    //! \throw wtl::invalid_argument - Unrecognised colour
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   setForeColour(COLORREF c)
    {
      CHR buf[10];

      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Skip if colour already set
      if (ForeColour == c)
        return;

      // Lookup colour ID
      for (UINT id = 0; id < Colours.size(); id++)
        if (Colours[id] == c)
        {  // Write colour change
          StringCchPrintfA(buf, 10, "\\cf%d ", id + 1);
          writeString(buf);
          ForeColour = c;
          return;
        }

      // Unknown colour
      throw wtl::invalid_argument(HERE, "No such colour in the colour table");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setFontSize
    //! Sets the font size
    //! \param[in] size - The size in points
    //!
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   setFontSize(UINT size)
    {
      CHR buf[10];

      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Set size
      StringCchPrintfA(buf, 10, "\\fs%d ", size * 2);
      writeString(buf);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setItalic
    //! Sets or clears italic text
    //! 
    //! \param[in] i - True to set, False to clear
    //! 
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   setItalic(bool i)
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Skip if italic already set
      if (Italic == i)
        return;

      // Set/Clear Italic
      Italic = i;
      writeString(Italic ? "\\i " : "\\i0 ");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setLeftMargin
    //!
    //! Sets size of left margin for current paragraph
    //! \param[in] size - size in twips
    //! 
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   setLeftMargin(UINT twips)
    {
      CHR buf[10];

      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Set margin size
      StringCchPrintfA(buf, 10, "\\li%d ", twips);
      writeString(buf);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setUnderline
    //! Sets or clears Underline text
    //! 
    //! \param[in] u - True to set, False to clear
    //! 
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   setUnderline(bool u)
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Skip if Underline already set
      if (Underline == u)
        return;

      // Set/Clear Underline
      Underline = u;
      writeString(Underline ? "\\ul " : "\\ul0 ");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::startParagraph
    //! Starts a new paragraph
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  startParagraph()
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Write
      writeString("\\par ");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::write
    //! Writes a character to the stream
    //! 
    //! \param[in] ch - The character
    //! 
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void   write(element_t ch)
    {
      element_t buf[10];

      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Escape/Convert char:
      switch (ch)
      {
      case '{':
      case '}':
        writeChar('\\');
        writeChar(ch);
        break;

      case '\\': writeString("\\\\");     break;
      case '\n': writeString("\\line ");  break;
      case '\t': writeString("\\tab ");   break;

      default:
        // ANSI char:
        if (ch <= 0x7f)
          writeChar(ch);
        else
        {  // UNICODE char:
          StringCchPrintfA(buf, 10, "\\u%u?", (uint32_t)ch);
          writeString(buf);
        }
        break;
      }
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::write
    //! Writes text to the stream 
    //! 
    //! \param[in] const* str - The text
    //! 
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  write(const element_t* str)
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");
    
      // Write characters
      for (const element_t* chr = str; *chr; ++chr)
        write(*chr);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::writeLn
    //! Writes text to the stream followed by a line break
    //! 
    //! \param[in] const* str - The text
    //! 
    //! \throw wtl::logic_error - [Debug only] Writer has been closed
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  writeLn(const element_t* str)
    {
      // Check stream is open
      if (Closed)
        throw wtl::logic_error(HERE, "Writer is closed");

      // Write str + CRLF
      write(str);
      writeString("\\line ");
    }

    // ------------------------------ PROTECTED METHODS -----------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::flush
    //! Flushes the output stream after a fixed number of characters has been written
    //! 
    //! \param[in] b - Number of characters written.
    //////////////////////////////////////////////////////////////////////////////////////////
    void  flush(int b)
    {
      // Query whether buffer should be flushed
      if ((Buffered += b) > BUFFER_SIZE)
      {
        Buffered = 0;
        flush();
      }
    }

    // ------------------------------- PRIVATE METHODS ------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setCodePage
    //! Sets the code page.
    //! 
    //! \param[in] cp - Code page
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  setCodePage(UINT cp)
    {
      element_t buf[10];
      StringCchPrintfA(buf, 10, "\\ansicpg%d ", cp);
      writeString(buf);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setDefaultFont
    //! Sets default font
    //! 
    //! \param[in] font - Font table index
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  setDefaultFont(UINT font)
    {
      element_t buf[10];
      StringCchPrintfA(buf, 10, "\\deff%d ", font);
      writeString(buf);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::setLanguage
    //! Sets the language.
    //! 
    //! \param[in] lang - Decimal language ID
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  setLanguage(UINT lang)
    {
      element_t buf[10];
      StringCchPrintfA(buf, 10, "\\deflang%d ", lang);
      writeString(buf);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::writeChar
    //! Writes character to the stream verbatim
    //! 
    //! \param[in] chr - The char
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  writeChar(element_t chr)
    {
      // Write character + flush
      Stream.write(chr);
      flush(1);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::writeString
    //! Writes string to the stream verbatim
    //! 
    //! \param[in] const* str - The text
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  writeString(const element_t* str)
    {
      int len = strlen(str);

      // Write text + flush
      Stream.write(str, len);
      flush(len);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::writeColour
    //! Writes an RTF colour table colour definition
    //! 
    //! \param[in] c - The colour to define
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  writeColour(COLORREF c)
    {
      element_t buf[64];
      StringCchPrintfA(buf, 64, "\\red%d\\green%d\\blue%d;", GetRValue(c), GetGValue(c), GetBValue(c));
      writeString(buf);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::writeFooter
    //! Writes RTF footer
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  writeFooter()
    {
      // End paragraph
      writeString("\\par\n}");
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // RtfWriter::writeHeader
    //! Writes RTF header
    //! 
    //! \param[in] font - Font name
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    //////////////////////////////////////////////////////////////////////////////////////////
    void  writeHeader(const char* font)
    {
      // Header
      writeString("{\\rtf1\\ansi");

      // Set codepage + language
      setCodePage(1250);
      setDefaultFont(0);
      setLanguage(2057);      // English UK  (0x0809 == 2057)

      // Font table
      writeString("{\\fonttbl{\\f0\\fswiss\\fcharset0 ");
      write(font);
      writeString(";}}\n");

      // Colour table
      writeString("{\\colortbl");
      writeString(";");  // Define colour 0 as 'auto-colour'
      for (COLORREF c : Colours)
        writeColour(c);
      writeString("}\n");


      // Unknown previous formatting: //\\sa200\\sl276\\slmult1
      // 
      // \saN == spacing after
      // \slN == space between lines
      // \smult == Line spacing multiple
      // \pard == reset paragraph formatting

      // Set view: 0==None, 1==Page Layout view, 2==Outline view, 3==Master Document view, 4==Normal view, 5==Online Layout view
      writeString("\\viewkind4");

      // Set unicode destination (not sure)
      writeString("\\uc1 ");

      // Reset paragraph
      resetParagraph();
    }

    
    /////////////////////////////////////////////////////////////////////////////////////////
    // RichTextWriter::operator=
    //! Copy-assign from a text writer of different type
    //! 
    //! \param[in,out] const& r - Text writer of different type
    //! \return RichTextWriter& - Reference to self
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    RichTextWriter& operator=(const RichTextWriter<U>& r)
    {
      // Avoid self-assignment, and copy stream
      if (static_cast<void*>(this) != static_cast<const void*>(&r))
        Stream = r.Stream;

      return *this;
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    bool         Closed;        // Whether writer is closed
    ColourTable  Colours;       // Colours in the RTF colour table
    COLORREF     ForeColour,    // Current forecolour
                 BackColour;    // Current backcolour
    bool         Bold,          // Whether in bold state
                 Italic,        // Whether in italic state
                 Underline,     // Whether in underline state
                 Superscript;   // Whether in superscript state
    UINT         Buffered;      // Number of characters written but not yet flushed
  };


  // --------------------------- ARRAY --------------------------
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes elements of an input range to a stream as rich text using a custom delimiter character 
  //!
  //! \tparam STREAM - Stream type
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in,out] &w - Rich Text writer
  //! \param[in] const& range - Delimited input range
  //! \return RichTextWriter<STREAM>& : Reference to writer 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename ITERATOR>
  RichTextWriter<STREAM>& operator<< (RichTextWriter<STREAM>& w, const delimited_range_t<ITERATOR>& range) 
  {
    delimit_range(w, range.First, range.Last, range.Delimiter);
    return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes pipe delimited array elements to a stream as rich text
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //! \tparam D - Whether array can be resized
  //! 
  //! \param[in,out] &w - Rich Text writer
  //! \param[in] const &a - Array  
  //! \return RichTextWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L, bool D>
  RichTextWriter<STREAM>& operator<< (RichTextWriter<STREAM>& w, const Array<E,L,D>& a)
  {
    // Default to pipe separator
    return w << delimited_range(a, '|');
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Write character array to a stream as text
  //!
  //! \tparam STREAM - Stream type
  //! \tparam ENCODING - Character encoding
  //! \tparam LENGTH - Character array capacity
  //! 
  //! \param[in,out] &w - Text writer
  //! \param[in] const &p - Character array 
  //! \return RichTextWriter<STREAM>& : Reference to 'w'
  //! 
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, Encoding ENCODING, unsigned LENGTH>
  RichTextWriter<STREAM>& operator << (RichTextWriter<STREAM>& w, const CharArray<ENCODING,LENGTH>& p) 
  {
    w.write(p.c_str(), p.size());
    return w;
  }

  // --------------------------- COMMON --------------------------

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes all elements of a statically allocated array to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam LENGTH - Array length
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] const (&)[] str - String
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, unsigned LENGTH>
  RichTextWriter<STREAM>& operator << (RichTextWriter<STREAM>& w, const typename STREAM::element_t (&str)[LENGTH])
  {
     w.write(str);
     return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a null terminated string to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] const* str - Null terminated string
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  RichTextWriter<STREAM>& operator << (RichTextWriter<STREAM>& w, const typename STREAM::element_t* str)
  {
     w.write(str);
     return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a boolean to a stream as text using the character 'Y' or 'N'
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] val - Boolean
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  RichTextWriter<STREAM>& operator << (RichTextWriter<STREAM>& w, bool val)
  {
     w.write(val ? 'Y' : 'N');
     return w;
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a character to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] ch - Character
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  RichTextWriter<STREAM>& operator << (RichTextWriter<STREAM>& w, char ch)
  {
     w.write(ch);
     return w;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes any integral or floating point type to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Integral or floating-point type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] val - Integral or floating point value 
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  std::enable_if_t<std::is_integral<U>::value 
                || std::is_floating_point<U>::value, RichTextWriter<STREAM>&>
  /*RichTextWriter<STREAM>&*/ operator << (RichTextWriter<STREAM>& w, U val)
  {
    w.writef(format_spec_t<U>::value, val);
    return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes the numeric representation of any enumeration type to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam U - Enumeration type
  //!
  //! \param[in,out] &w - Text writer
  //! \param[in] val - Enumeration value
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  enable_if_enum_t<U,RichTextWriter<STREAM>&> operator << (RichTextWriter<STREAM>& w, U val)
  {
    // Write as a numeral accoring to underlying type
    return w << static_cast<std::underlying_type_t<U>>(val);
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::endl
  //! Writes a carriage return & line feed (0x0d, 0x0a) to a stream
  //!
  //! \param[in,out] &w - Text writer
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
	RichTextWriter<STREAM>& endl(RichTextWriter<STREAM>& w)
	{	
    return w << "\r\n";
	}
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Executes a unary text stream operator overload
  //! 
  //! \param[in,out] &w - Text writer
  //! \param[in] *fn - Function overload
  //! \return RichTextWriter<STREAM>& - Reference to 'w'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  RichTextWriter<STREAM>& operator<< (RichTextWriter<STREAM>& w, typename RichTextWriter<STREAM>::delegate_t fn)  
  { 
    return fn(w); 
  }

} //namespace wtl
#endif // WTL_RICH_TEXT_WRITER_HPP
