//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\controls\edit\EditLinesCollection.hpp
//! \brief Abstracts the text of an edit control as an array of lines
//! \date 25 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EDIT_LINES_COLLECTION_HPP
#define WTL_EDIT_LINES_COLLECTION_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>     //!< Encoding
#include <wtl/utils/String.hpp>              //!< String

/////////////////////////////////////////////////////////////////////////////////////////
//! \namespace wtl - Windows template library
/////////////////////////////////////////////////////////////////////////////////////////
namespace wtl 
{
  //! Forward declaration
  template <Encoding ENC>
  struct Edit;

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct EditLinesCollection - Abstracts the text of an edit control as an array of lines
  //! 
  //! \tparam ENC - Window encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct EditLinesCollection 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------

    //! \alias type - Define own type
    using type = EditLinesCollection<ENC>;

    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias size_t - Define line length type
    using size_t = uint32_t;
    
    //! \alias value_t - Define line text type
    using value_t = String<ENC>;
    
    //! \alias window_t - Inherit window type
    using window_t = Edit<ENC>;
    
    //! \struct LineProxy - Proxy for individual lines
    struct LineProxy
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
      //! \alias type - Define own type
      using type = LineProxy;

      // ----------------------------------- REPRESENTATION -----------------------------------
    private:
      const window_t&  Control;       //!< Edit control
      const uint32_t   LineIndex;     //!< Zero-based line index
      
      // ---------------------------------- CREATE & DESTROY ----------------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // LineProxy::LineProxy
      //! Create proxy for a particular line
      //! 
      //! \param[in] const& ctrl - Edit control
      //! \param[in] idx - Zero-based line index
      /////////////////////////////////////////////////////////////////////////////////////////
      LineProxy(const window_t& ctrl, uint32_t idx) : Control(ctrl), 
                                                      LineIndex(idx)
      {}

      // ------------------------------------ COPY & MOVE -------------------------------------
    
      // ----------------------------------- STATIC METHODS -----------------------------------
    
      // ---------------------------------- ACCESSOR METHODS ----------------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // LineProxy::index() const
      //! Query line index
      //! 
      //! \return uint32_t - Zero-based line index
      /////////////////////////////////////////////////////////////////////////////////////////
      uint32_t index() const && noexcept  
      {
        return this->LineIndex;
      }

      /////////////////////////////////////////////////////////////////////////////////////////
      // LineProxy::value_t() const &&
      //! Get text as dynamic string
      //! 
      //! \return value_t - Line text as dynamic string
      //! 
      //! \throw wtl::platform_error - Unable to retrieve window text
      /////////////////////////////////////////////////////////////////////////////////////////
      operator value_t() const &&
      {
        // Convert line idx -> char idx
        uint32_t chrIdx = Edit_LineIndex(this->Control.handle(),this->LineIndex);

        //! Query line length
        if (uint32_t len = Edit_LineLength(this->Control.handle(), chrIdx))
        {
          std::vector<char_t> txt(len+1);
       
          //! Copy line into buffer
          if (Edit_GetLine(this->Control.handle(), this->LineIndex, txt.data(), len) != len)
            throw platform_error(HERE, "Unable to copy text from line ", len);

          // Null terminate & return
          txt[len] = char_t();
          return {txt.begin(), txt.end()};
        }

        // Empty: Return blank
        return value_t();
      }
    
      /////////////////////////////////////////////////////////////////////////////////////////
      // LineProxy::size() const
      //! Query line length
      //! 
      //! \return uint32_t - Length of line in characters, excluding the null terminator
      //! 
      //! \throw wtl::platform_error - Unable to retrieve window text
      //!
      //! \remarks For multiline edit controls, the return value excludes the carriage-return character at the end of the line.
      /////////////////////////////////////////////////////////////////////////////////////////
      uint32_t size() const &&
      {
        return Edit_LineLength(this->Control.handle(), this->LineIndex);
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      //! Prevent assignment: Proxy does not provide write access
      type& operator=(const type&) = delete;

      //! Prevent assignment: Proxy does not provide write access
      template <typename U>
      type& operator=(U&&) = delete;
    };

    //! \alias proxy_t - Define line proxy type
    using proxy_t = LineProxy;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    window_t&  Control;     //!< Associated edit window

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::EditLinesCollection
    //! Create collection for edit control
    //! 
    //! \param[in,out] &ctrl - Edit control
    /////////////////////////////////////////////////////////////////////////////////////////
    EditLinesCollection(window_t& ctrl) : Control(ctrl)
    {}

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::at const
    //! Retrieve a line of text with bounds checking
    //! 
    //! \param[in] idx - Zero-based line index, or -1 for current line
    //! \return value_t - Specified line text 
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::out_of_range - Index out of range
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    value_t at(int32_t idx) const
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "Edit control does not exist");

      // Verify index
      if (idx < -1 || idx >= static_cast<int32_t>(size()))
        throw out_of_range(HERE, "Index ", idx, " is out of range");

      // Create proxy for specified line, otherwise locate line containing caret
      return this->operator[](idx);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::bottom const
    //! Retrieve the last line of text 
    //! 
    //! \return proxy_t - Proxy providing line text
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    proxy_t bottom() const
    {
      // Retrieve bottom line
      return this->operator[](size()-1);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::size() const
    //! Query whether collection is empty
    //! 
    //! \return bool - True iff empty
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const 
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "Edit control does not exist");

      return size() == 0;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::size() const
    //! Query number of lines
    //! 
    //! \return uint32_t - Number of lines  (Never less than 1)
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    /////////////////////////////////////////////////////////////////////////////////////////
    uint32_t size() const 
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "Edit control does not exist");

      return Edit_GetLineCount(this->Control.handle());
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::top const
    //! Retrieve the top line of text 
    //! 
    //! \return proxy_t - Proxy providing line text
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    proxy_t top() const
    {
      // Retrieve top line
      return this->operator[](0);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::operator[] const
    //! Retrieve a line of text without bounds checking
    //! 
    //! \param[in] idx - Zero-based line index, or -1 for current line
    //! \return proxy_t - Proxy providing line text
    //! 
    //! \throw wtl::logic_error - Edit control does not exist
    //! \throw wtl::platform_error - Unable to retrieve window text
    /////////////////////////////////////////////////////////////////////////////////////////
    proxy_t operator[](int32_t idx) const
    {
      //! Ensure exists
      if (!this->Control.exists())
        throw logic_error(HERE, "Edit control does not exist");

      // Create proxy for specified line, otherwise locate line containing caret
      return proxy_t(this->Control, idx >= 0 ? idx : Edit_LineFromChar(this->Control.handle(), -1));
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // EditLinesCollection::clear() 
    //! Clear all lines
    //! 
    //! \throw wtl::platform_error - Unable to set window text
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear() const
    {
      static const value_t EMPTY;

      //! Replace text with nothing
      this->Control.Text = EMPTY;
    }
  };

      
} // namespace wtl

#endif // WTL_EDIT_LINES_COLLECTION_HPP
