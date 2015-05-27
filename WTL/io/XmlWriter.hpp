//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\XmlWriter.hpp
//! \brief Provides xml encoding for output streams
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_XML_WRITER_HPP
#define WTL_XML_WRITER_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{  
  //! \enum ElementType - Defines element types
  enum class ElementType { Opening, Closing, Orphan };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct XmlElement - Helper object for writing Xml elements
  //! 
  //! \tparam TYPE - Element type
  //! \tparam ARGS... - Attribute name/value pair types
  //////////////////////////////////////////////////////////////////////////////////////////
  template <ElementType TYPE, typename... ARGS>
  struct XmlElement
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef storage_t - Attributes collection
    typedef std::tuple<ARGS...> storage_t;

    //! \var Count - Number of attribute name/value pairs
    static constexpr uint32_t  Count = sizeof...(ARGS);   
    
    //! \var HasAttributes - Whether element has any attributes
    static constexpr bool  HasAttributes = Count != 0;

    //! \var Type - Element type
    static constexpr ElementType  Type = TYPE;

    // ------------------------------------ CONSTRUCTION ------------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlElement::XmlElement
    //! Define an xml element and attributes
    //! 
    //! \param[in] const* name - Element name (without brackets)
    //! \param[in] &&... args - [optional] Attribute name/value pairs
    //////////////////////////////////////////////////////////////////////////////////////////
    XmlElement(const char* name, ARGS&&... args) : Name(name), 
                                                   Attributes(std::forward<ARGS>(args)...)
    {}

    // ----------------------------------- REPRESENTATION -----------------------------------

    const char*  Name;          //!< Element name
    storage_t    Attributes;    //!< Attribute name/value pair storage
  };

  //! \typedef XmlOrphanElement - Orphan Xml element
  template <typename... ARGS>
  using XmlOrphanElement = XmlElement<ElementType::Orphan, ARGS...>;

  //! \typedef XmlOpeningElement - Opening Xml element
  template <typename... ARGS>
  using XmlOpeningElement = XmlElement<ElementType::Opening, ARGS...>;
  
  //! \typedef XmlClosingElement - Closing Xml element
  template <typename... ARGS>
  using XmlClosingElement = XmlElement<ElementType::Closing, ARGS...>;
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::xml_opening_element 
  //! Create an opening xml element and specify attributes
  //! 
  //! \param[in] const& name - Element name (without brackets)
  //! \param[in] &&... args - [optional] Attribute name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  XmlOpeningElement<ARGS...>  xml_opening_element(const char* name, ARGS&&... args)
  {
    //return XmlOpeningElement<ARGS...>(name, std::forward<ARGS>(args)...);
    return XmlElement<ElementType::Opening, ARGS...>(name, std::forward<ARGS>(args)...);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::xml_closing_element
  //! Create a closing xml element and specify attributes
  //! 
  //! \param[in] const& name - Element name (without brackets)
  //! \param[in] &&... args - [optional] Attribute name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  XmlClosingElement<ARGS...>  xml_closing_element(const char* name, ARGS&&... args)
  {
    //return XmlClosingElement<ARGS...>(name, std::forward<ARGS>(args)...);
    return XmlElement<ElementType::Closing, ARGS...>(name, std::forward<ARGS>(args)...);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::xml_orphan_element
  //! Create an orphan xml element and specify attributes
  //! 
  //! \param[in] const& name - Element name (without brackets)
  //! \param[in] &&... args - [optional] Attribute name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename... ARGS>
  XmlOrphanElement<ARGS...>  xml_orphan_element(const char* name, ARGS&&... args)
  {
    //return XmlOrphanElement<ARGS...>(name, std::forward<ARGS>(args)...);
    return XmlElement<ElementType::Orphan, ARGS...>(name, std::forward<ARGS>(args)...);
  }

    

  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct XmlWriter - Writes xml elements and attributes to an output stream
  //! 
  //! \tparam STREAM - Output stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct XmlWriter : TextWriter<STREAM>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \typedef stream_t - Stream type
    typedef STREAM stream_t;
    
    //! \typedef base - Base class alias
    typedef TextWriter<STREAM> base;

    //! \typedef delegate_t - Defines signature of output overload
    typedef XmlWriter& (*delegate_t)(XmlWriter&);
    
    //! \typedef distance_t - Stream distance type
    typedef typename stream_t::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename stream_t::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename stream_t::position_t position_t;
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlWriter::XmlWriter 
    //! Create an xml writer and initialise the output stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    explicit XmlWriter(ARGS&&... args) : base(std::forward<ARGS>(args)...)
    {}
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlWriter::~XmlWriter
    //! Can be polymorphic
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~XmlWriter()
    {}

    
    ENABLE_COPY(XmlWriter);    //!< Copy semantics determined by stream type
	
	  // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
  };

  



  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes elements of an input range to an xml ouput stream as text using a custom delimiter character 
  //!
  //! \tparam STREAM - Stream type
  //! \tparam ITERATOR - Iterator type
  //!
  //! \param[in,out] &w - Xml writer
  //! \param[in] const& range - Delimited input range
  //! \return XmlWriter<STREAM>& : Reference to writer 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename ITERATOR>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, const delimited_range_t<ITERATOR>& range) 
  {
    delimit_range(w, range.First, range.Last, range.Delimiter);
    return w;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes pipe delimited array elements to a stream as text
  //!
  //! \tparam STREAM - Stream type
  //! \tparam E - Array element type
  //! \tparam L - Array capacity
  //! \tparam D - Whether array can be resized
  //! 
  //! \param[in,out] &w - Xml writer
  //! \param[in] const &a - Array  
  //! \return XmlWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename E, unsigned L, bool D>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, const Array<E,L,D>& a)
  {
    // Default to pipe separator
    return w << delimited_range(a, '|');
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a null terminated string to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //!
  //! \param[in,out] &w - Xml writer
  //! \param[in] const* str - Null terminated string
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, const typename STREAM::element_t* str)
  {
     w.write(str);
     return w;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Write character array to a stream as text
  //!
  //! \tparam STREAM - Stream type
  //! \tparam ENCODING - Character encoding
  //! \tparam LENGTH - Character array capacity
  //! 
  //! \param[in,out] &w - Xml writer
  //! \param[in] const &p - Character array 
  //! \return XmlWriter<STREAM>& : Reference to 'w'
  //! 
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, Encoding ENCODING, unsigned LENGTH>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, const CharArray<ENCODING,LENGTH>& p) 
  {
    w.write(p.c_str(), p.size());
    return w;
  }


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes all elements of a statically allocated array to a stream as text
  //! 
  //! \tparam STREAM - Stream type
  //! \tparam LENGTH - Array length
  //!
  //! \param[in,out] &w - Xml writer
  //! \param[in] const (&)[] str - String
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, unsigned LENGTH>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, const typename STREAM::element_t (&str)[LENGTH])
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
  //! \param[in,out] &w - Xml writer
  //! \param[in] val - Boolean
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, bool val)
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
  //! \param[in,out] &w - Xml writer
  //! \param[in] ch - Character
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, char ch)
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
  //! \param[in,out] &w - Xml writer
  //! \param[in] val - Integral or floating point value 
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  std::enable_if_t<std::is_integral<U>::value 
                || std::is_floating_point<U>::value, XmlWriter<STREAM>&>
  /*XmlWriter<STREAM>&*/ operator << (XmlWriter<STREAM>& w, U val)
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
  //! \param[in,out] &w - Xml writer
  //! \param[in] val - Enumeration value
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename U>
  enable_if_enum_t<U,XmlWriter<STREAM>&> operator << (XmlWriter<STREAM>& w, U val)
  {
    // Write as a numeral accoring to underlying type
    return w << static_cast<std::underlying_type_t<U>>(val);
  }

  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::endl
  //! Writes a carriage return & line feed (0x0d, 0x0a) to a stream
  //!
  //! \param[in,out] &w - Xml writer
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
	XmlWriter<STREAM>& endl(XmlWriter<STREAM>& w)
	{	
    return w << "\r\n";
	}
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Executes a unary text stream operator overload
  //! 
  //! \param[in,out] &w - Xml writer
  //! \param[in] *fn - Function overload
  //! \return XmlWriter<STREAM>& - Reference to 'w'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, typename XmlWriter<STREAM>::delegate_t fn)  
  { 
    return fn(w); 
  }
  

  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes a name-value pair to an xml stream as an element attribute name and value
  //!
  //! \tparam STREAM - Stream type
  //! \tparam VALUE - Value type
  //! 
  //! \param[in,out] &w - Xml writer
  //! \param[in] const& attribute - Name-Value pair representing an attribute name & value
  //! \return XmlWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, typename VALUE>
  XmlWriter<STREAM>& operator << (XmlWriter<STREAM>& w, const NameValuePair<VALUE>& attribute)
  {
    // Append: {space}{name}='{value}'
    return w << " " << attribute.Name << "='" << attribute.Value << "'";
  }
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct element_unpacker - Writes a name/value pair tuple to an output stream
  //! 
  //! \tparam IDX - Zero-based iteration index
  //! \tparam ARGS - Name/value pairs
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned IDX, unsigned COUNT, typename... ARGS>
  struct element_unpacker
  {
    //////////////////////////////////////////////////////////////////////////////////////////
	  //! element_unpacker::unpack
    //! Unpacks attributes and writes them as a sequence of name/value pairs
    //! 
    //! \param[in,out] &w - Xml writer
    //! \param[in] const& element - Xml element
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename STREAM, ElementType TYPE>
    static void unpack(XmlWriter<STREAM>& w, const XmlElement<TYPE,ARGS...>& element)
    {
      w << std::get<IDX>(element.Attributes);
      element_unpacker<IDX+1,COUNT,ARGS...>::unpack<STREAM,TYPE>(w, element);
    }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct element_unpacker - Base case 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned COUNT, typename... ARGS>
  struct element_unpacker<COUNT, COUNT, ARGS...>
  {
    template <typename STREAM, ElementType TYPE>
    static void unpack(XmlWriter<STREAM>& w, const XmlElement<TYPE,ARGS...>& element)
    { /*no-op*/ }
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes an xml element and attributes to an xml stream 
  //!
  //! \tparam STREAM - Stream type
  //! \tparam TYPE - Element type
  //! \tparam ARGS... - Attribute name/value pair types
  //! 
  //! \param[in,out] &w - Xml writer
  //! \param[in] const& element - Xml element
  //! \return XmlWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, ElementType TYPE, typename... ARGS>
  std::enable_if_t<XmlElement<TYPE,ARGS...>::HasAttributes, XmlWriter<STREAM>&>
  /*XmlWriter<STREAM>&*/ operator << (XmlWriter<STREAM>& w, const XmlElement<TYPE,ARGS...>& element)
  {
    static_assert(sizeof...(ARGS) != 0, "Invalid parameter pack size");

    // Open tag
    w << (TYPE == ElementType::Closing ? "</" : "<");
      
    // Write name + attributes
    w << (element.Name);
    element_unpacker<0,sizeof...(ARGS),ARGS...>::unpack(w, element);    //for_each<0U,ostream_iterator<XmlWriter>&,ARGS...>(element.Attributes, ostream_iterator<XmlWriter>(*this));

    // Close
    w << (TYPE == ElementType::Orphan ? "/>" : ">");
    return w;
  }

    
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator << 
  //! Writes an xml element with no attributes to an xml stream 
  //!
  //! \tparam STREAM - Stream type
  //! \tparam TYPE - Element type
  //! \tparam ARGS... - Empty pack
  //! 
  //! \param[in,out] &w - Xml writer
  //! \param[in] const& element - Xml element
  //! \return XmlWriter<STREAM>& : Reference to 'w'
  //!
  //! \throw wtl::length_error - [Debug only] Insufficient stream buffer space
  //! \throw wtl::out_of_range - [Debug only] Stream position out of bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM, ElementType TYPE, typename... ARGS>
  std::enable_if_t<!XmlElement<TYPE,ARGS...>::HasAttributes, XmlWriter<STREAM>&>
  /*XmlWriter<STREAM>&*/ operator << (XmlWriter<STREAM>& w, const XmlElement<TYPE,ARGS...>& element)
  {
    static_assert(sizeof...(ARGS) == 0, "Invalid parameter pack size");

    // Open tag
    w << (TYPE == ElementType::Closing ? "</" : "<");
      
    // Write name 
    w << (element.Name);
  
    // Close
    w << (TYPE == ElementType::Orphan ? "/>" : ">");
    return w;
  }


  
} //namespace wtl
#endif // WTL_XML_WRITER_HPP
