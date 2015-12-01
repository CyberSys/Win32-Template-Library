//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\XmlReader.hpp
//! \brief Provides a DOM XML reader
//! \brief This file is now out of date and needs updating to support the newer stream mechanics
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_XML_READER_HPP
#define WTL_XML_READER_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/SFINAE.hpp>       //!< enable_if_floating_t
#include "pugixml/pugixml.hpp"        //!< pugixml

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
	//! \struct XmlReader - Non-validating DOM XML reader. This class is now out of date and needs updating.
  //!
  //! \tparam STREAM - Stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct XmlReader 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef XPathNode - Single node returned by an x-path query
    typedef pugi::xpath_node  XPathNode;

    //! \typedef XPathNodeSet - Set of nodes returned by an x-path query
    typedef pugi::xpath_node_set  XPathNodeSet;
    
    //! \typedef XPathQuery - XPath query
    typedef pugi::xpath_query  XPathQuery;
    
    //! \typedef XmlNode - Single DOM query
    typedef pugi::xml_node  XmlNode;

    //! \typedef stream_t - Stream type
    typedef STREAM  stream_t;

    // ------------------------------------ CONSTRUCTION ------------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::XmlReader 
    //! Create an xml reader, initialises then reads the entire input stream as single xml fragment
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    //! 
    //! \throw wtl::domain_error - Unable to parse xml
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS> explicit
    XmlReader(ARGS&&... args) : Stream(std::forward<ARGS>(args)...)
    {
      // Parse xml directly from input stream
      pugi::xml_parse_result res = Document.load_buffer_inplace(Stream.buffer(), Stream.remaining(), pugi::parse_default | pugi::parse_fragment);

      // Query result
      if (!res)
        throw wtl::domain_error(HERE, "Unable to parse xml: %s", res.description());
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::~XmlReader
    //! Can be polymorphic
    //////////////////////////////////////////////////////////////////////////////////////////
    virtual ~XmlReader() = default;

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::evaluate
    //! Evaluates an x-path query as text
    //!
    //! \param[in] &query - XPath query
    //! \return std::string - String value
    //////////////////////////////////////////////////////////////////////////////////////////
    std::string evaluate(const XPathQuery& query) const
    { 
      // Execute query
      return query.evaluate_string(Document);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::selectNode
    //! Execute a query that returns a single node
    //! 
    //! \param[in] const *query - XPath 1.0 query to evaluate
    //! \return XPathNode - Result node
    //////////////////////////////////////////////////////////////////////////////////////////
    XPathNode selectNode(const char* query) const
    {
      return Document.select_single_node(XPathQuery(query));
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::selectNodes
    //! Execute a query that returns a set of nodes
    //! 
    //! \param[in] const *query - XPath 1.0 query to evaluate
    //! \return XPathNodeSet - Set of result nodes
    //////////////////////////////////////////////////////////////////////////////////////////
    XPathNodeSet selectNodes(const char* query) const
    {
      return Document.select_nodes(XPathQuery(query));
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::getString
    //! Retrieve the text-value from the first node returned by an x-path v1.0 query
    //! 
    //! \tparam ENCODING - Character encoding
    //! \tparam LENGTH - Capacity of output character array 
    //!
    //! \param[in] const* query - XPath 1.0 query to evaluate
    //! \param[in,out] &str - Character array to populate
    //! \return bool - True if query succeeded, otherwise False
    //! 
    //! \throw wtl::length_error - [Debug only] Char array buffer overflow
    //////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENCODING, unsigned LENGTH>
    bool getString(const char* query, CharArray<ENCODING,LENGTH>& str) const
    { 
      return getString(XPathQuery(query), str);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::getString
    //! Retrieve the text-value from the first node returned by an x-path v1.0 query
    //! 
    //! \tparam ENCODING - Character encoding
    //! \tparam LENGTH - Capacity of output character array 
    //!
    //! \param[in] const& query - XPath 1.0 query to evaluate
    //! \param[in,out] &str - Character array to populate
    //! \return bool - True if query succeeded, otherwise False
    //! 
    //! \throw wtl::length_error - [Debug only] Char array buffer overflow
    //////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENCODING, unsigned LENGTH>
    bool getString(const XPathQuery& query, CharArray<ENCODING,LENGTH>& str) const
    { 
      // Execute query
      std::string sz = query.evaluate_string(Document);
      
      //// Validate
      //CHECKED_LENGTH(!n || (strlen(n.node().value()) < CharArray<LENGTH,char>::length));

      // Copy string into character array if node is valid
      return sz.length() > 0 ? (str.assign(sz.c_str()), true) : false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::getValue
    //! Reinterpret the text-value from the first node returned by an x-path v1.0 query
    //! 
    //! \tparam T - Value type
    //!
    //! \param[in] const *key - XPath query
    //! \param[in,out] &value - Value to populate
    //! \return bool - True if query succeeded, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    //template <typename KEY, typename T>
    //bool getValue(KEY key, T& value) const;  // Undefined
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::getValue<bool>
    //! Reinterpret the text-value from the first node returned by an x-path v1.0 query as boolean
    //! 
    //! \param[in] const *key - XPath query
    //! \param[in,out] &value - Boolean to populate
    //! \return bool - True if query succeeded, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename KEY>
    bool getValue(KEY key, bool value) const
    {
      CharArray<32> str;
      
      // Evaluate
      if (getString(key, str) && !str.empty())
      {
        // Letters
        switch (str[0])
        {
        case 'y': case 'Y': 
          return (value = true);

        case 'n': case 'N': 
          return (value = false, true);
        }

        // Words
        if (str == "true")
          return (value = true);
        if (str == "false")
          return (value = false, true);
      }

      return (value = false);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::getValue
    //! Evaluates an X-Path query as a string and parses the result as a floating point value
    //! 
    //! \param[in] const *key - Query that returns a string
    //! \param[in,out] &value - Output value
    //! \return bool - True if query succeeded, otherwise False
    //! 
    //! \throw pugi::pugi_error - Query does not result in a string
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename KEY, typename T, typename = enable_if_floating_t<T>>
    bool getValue(KEY key, T& value) const
    {
      CharArray<32> str;
      
      // Convert string, if present, to int and return true; otherwise return false
      return getString(key, str) && !str.empty() ? (value = atof(str.c_str()), true) : false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::getValue
    //! Evaluates an X-Path query as a string and parses the result as an integral or enumeration value
    //! 
    //! \param[in] const *key - Query that returns a string
    //! \param[in,out] &value - Output value
    //! \return bool - True if query succeeded, otherwise False
    //! 
    //! \throw pugi::pugi_error - Query does not result in a string
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename KEY, typename T, typename = enable_if_numeric_t<T>>
    bool getValue(KEY key, T& value) const
    {
      CharArray<32> str;
      
      // Convert string, if present, to int and return true; otherwise return false
      return getString(key, str) && !str.empty() ? (value = atoi(str.c_str()), true) : false;
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::print
    //! Prints document to the debug console 
    //////////////////////////////////////////////////////////////////////////////////////////
    void print()
    {
      try
      {
        // Select all elements
        if (auto root = selectNode("/").node())
        {
          std::ostringstream stream;

          // Generate document with line breaks
          root.print(stream, " ");
          cdebug_s << stream.str();
        }
      }
      catch (std::exception& e) {
        cdebug_s << Cons::Warning << "Unable to print invalid xml: "
                 << Cons::Yellow << e.what();
      }
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // XmlReader::root const
    //! Get the root node
    //! 
    //! \return XmlNode - Document root node
    //////////////////////////////////////////////////////////////////////////////////////////
    XmlNode root() const
    {
      return Document.root();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    pugi::xml_document Document;      //!< PugiXml DOM Document
    stream_t           Stream;        //!< Output stream
  };


  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a formatted xml fragment to the debug console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in,out] &reader - Xml reader
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  inline Console& operator<< (Console& c, const pugi::xml_node& node)
  { 
    static constexpr Cons colDelimiter = Cons::Grey,              //!< Delimiter colour
                          colElement = Cons::Yellow,              //!< Element colour
                          colAttribute = Cons::Yellow,            //!< Attribute name colour
                          colComment = Cons::Grey,                //!< Comment colour
                          colText = Cons::White;                  //!< Text and attribute value colour

    // Skip comments, text
    switch (node.type())
    {
    // [DOCUMENT] Print entire fragment
    case pugi::node_document:
      // Print children
      for (auto n = node.first_child(); n; n = n.next_sibling())
        c << n << endl;
      break;

    // [ELEMENT] Print element, attributes, and children
    case pugi::node_element:
      // Indent opening tag
      for (auto n = node.parent(); n; n = n.parent())
        c << ' ';

      // Open tag
      c << colDelimiter << '<' 
        << colElement   << node.name();

      // Print attributes
      for (auto attr = node.first_attribute(); attr; attr = attr.next_attribute())
        //cdebug << ' ' << name_value_pair(attr.name(), attr.value());
        c << ' ' << colAttribute << attr.name() 
                 << colDelimiter << "='" 
                 << colText      << attr.value() 
                 << colDelimiter << '\'';

      // [NO CHILDREN, NO TEXT] Close tag (CRLF)
      if (!node.first_child())
        c << colDelimiter << "/>";
      else
      {
        // Finish tag
        c << colDelimiter << '>';

        // Print children
        bool hasChildren = false;
        for (auto n = node.first_child(); n; n = n.next_sibling())
          switch (n.type())
          {
          // [ELEMENT/COMMENT] Print nodes
          case pugi::node_element:
          case pugi::node_comment:
            // Print initial CRLF
            if (!hasChildren) 
              c << endl;

            // Print Child + CRLF
            c << n << endl;
            hasChildren = true;
            break;

          // [TEXT] Print text
          case pugi::node_pcdata:
          case pugi::node_cdata:
            c << n;
            break;
          }
        
        // Indent closing tag
        if (hasChildren)
          for (auto n = node.parent(); n; n = n.parent())
            c << ' ';

        // Close tag (CRLF)
        c << colDelimiter << "</" 
          << colElement   << node.name() 
          << colDelimiter << '>';
      }
      break;

    // [TEXT] Print in white
    case pugi::node_pcdata:
    case pugi::node_cdata:
      return c << colText << node.text().get();
        
    // [COMMENT] Print in grey
    case pugi::node_comment:
      return c << colComment << "<!--" << node.text().get() << "-->";
        
    // [NULL] Drop
    case pugi::node_null:
      break;

    // [PROCESSING INSTRUCTION] Drop
    case pugi::node_pi:
    case pugi::node_declaration:
    case pugi::node_doctype:
      break;
    }

    return c;
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Writes a formatted xml fragment to the debug console
  //! 
  //! \param[in,out] &c - Debug console
  //! \param[in,out] &reader - Xml reader
  //! \return Console& - Reference to 'c'
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  Console& operator<< (Console& c, const XmlReader<STREAM>& reader)
  { 
    // Print document from root
    return c << static_cast<const pugi::xml_node&>(reader.root()) << endl;
  }
  


} //namespace wtl
#endif // WTL_XML_READER_HPP

