//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\LogFileWriter.hpp
//! \brief Rich Text log file writer
//! \brief This file is now out of date and needs updating to support the newer stream mechanics
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LOG_FILE_WRITER_HPP
#define WTL_LOG_FILE_WRITER_HPP

#include <wtl/WTL.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct LogFileWriter - Writes debug console output to disc as Rtf
  //! 
  //! \tparam STREAM - Stream type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename STREAM>
  struct LogFileWriter : RichTextWriter<STREAM>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \typedef base - Defines base type
    typedef RichTextWriter<STREAM> base;
    
    //! \typedef distance_t - Stream distance type
    typedef typename STREAM::distance_t distance_t;
    
    //! \typedef element_t - Stream element type
    typedef typename STREAM::element_t element_t;
    
    //! \typedef position_t - Stream position type
    typedef typename STREAM::position_t position_t;
    
    //! \typedef stream_t - Log stream type
    typedef STREAM stream_t;
    
  protected:
    //! \enum Colours - Defines extra console colours
    enum Colours 
    { 
      FOREGROUND_BLACK = 0,
      FOREGROUND_PURPLE = FOREGROUND_RED|FOREGROUND_BLUE,
      FOREGROUND_YELLOW = FOREGROUND_RED|FOREGROUND_GREEN,
      FOREGROUND_WHITE = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,
      FOREGROUND_CYAN = FOREGROUND_BLUE|FOREGROUND_GREEN,
      FOREGROUND_GREY = FOREGROUND_INTENSITY|FOREGROUND_BLACK,
    };

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // LogFileWriter::LogFileWriter 
    //! Create a log file writer and initialise the output stream
    //! 
    //! \tparam ARGS... - Arguments parameter pack type
    //! 
    //! \param[in,out] &&... args - Stream constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS> explicit
    LogFileWriter(ARGS&&... args) : base(std::forward<ARGS>(args)...)
    {
      // Define unique colours
      static constexpr std::list<COLORREF> colours
      {
        White,
        Red,
        Green,
        Blue,
        Magenta,
        Yellow,
        Cyan,
        Black,
      };

      // Init writer
      base::open("FixedSys", 10, colours);

      // Set colours
      base::setBackColour(White);
      base::setForeColour(Black);

      // Set properties
      base::setAlignment(Alignment::Left);
    }
      
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------

    ENABLE_COPY(LogFileWriter);        //!< Copy semantics determined by stream type
    ENABLE_MOVE(LogFileWriter);        //!< Move semantics determined by stream type
    ENABLE_POLY(LogFileWriter);        //!< Can be polymorphic
	
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // LogFileWriter::close
    //! Closes the output stream
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    /////////////////////////////////////////////////////////////////////////////////////////
    void close()
    {
      base::close();
    }

    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LogFileWriter::flush
    //! Flushes the output stream
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    /////////////////////////////////////////////////////////////////////////////////////////
    void flush()
    {
      base::flush();
    }

    
    /////////////////////////////////////////////////////////////////////////////////////////
    // LogFileWriter::Write 
    //! Write to the log file
    //! 
    //! \param[in] const& txt - Output string
    //! \param[in] attributes - Attributes
    //! 
    //! \throw wtl::domain_error - I/O error occurred
    /////////////////////////////////////////////////////////////////////////////////////////
    void  write(const char* txt, Cons attributes)
    {
      // Set colour
      switch (attributes & FOREGROUND_WHITE)
      {
      case FOREGROUND_RED:    setForeColour(Red);     break;
      case FOREGROUND_GREEN:  setForeColour(Green);   break;
      case FOREGROUND_BLUE:   setForeColour(Blue);    break;
      case FOREGROUND_YELLOW: setForeColour(Yellow);  break;
      case FOREGROUND_CYAN:   setForeColour(Cyan);    break;
      case FOREGROUND_PURPLE: setForeColour(Magenta); break;
      case FOREGROUND_WHITE:  setForeColour(Black);   break;
      }

      // Bold/Normal
      base::setBold((attributes & FOREGROUND_INTENSITY) != 0);

      // Write
      base::write(txt);
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
  };
  
  //! \typedef log_writer_t - Log-file writer type
  typedef LogFileWriter<LocalFileStream<char>> logfile_writer_t;

  
  //! \val logFile - Debug log-file stream writer
  extern logfile_writer_t logFile;
  
} //namespace wtl
#endif // WTL_LOG_FILE_WRITER_HPP

