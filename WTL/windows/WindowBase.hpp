//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\WindowBase.hpp
//! \brief Base class for windows
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WINDOW_BASE_HPP
#define WTL_WINDOW_BASE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WindowBase - Base for all window types
  //! 
  //! \tparam ENC - Window character encoding
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC>
  struct WindowBase 
  {
    struct SubClass;

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = WindowBase<ENC>;
    
    //! \alias char_t - Define window character type
    using char_t = encoding_char_t<ENC>;
    
    //! \alias action_t - Define Action type
    using action_t = Action<ENC>;
    
    //! \alias resource_t - Resource identifier type
    using resource_t = ResourceId<ENC>;
    
    //! \alias wndclass_t - Window class type
    using wndclass_t = WindowClass<ENC>;
    
    //! \alias wndmenu_t - Window menu type
    using wndmenu_t = WindowMenu<ENC>;

    //! \alias wndproc_t - Win32 Window procedure type
    using wndproc_t = LRESULT (__stdcall*)(::HWND, uint32, WPARAM, LPARAM);

    //! \alias wtlproc_t - WTL Window procedure type
    using wtlproc_t = LResult (__thiscall*)(WindowMessage, WPARAM, LPARAM);  // = decltype(onMessage);

    //! \alias window_t - Define own type
    using window_t = type;
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

    //! \alias ActionQueue - Define gui command queue type
    using ActionQueue = ActionQueue<ENC>;
    
    //! \struct ActionGroupCollection - Collection of Action groups, indexed by Id
    struct ActionGroupCollection : std::map<CommandGroupId,ActionGroupPtr<ENC>>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias base - Define base type
      using base = std::map<CommandGroupId,ActionGroupPtr<ENC>>;
      
      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // ActionGroupCollection::find const
      //! Find an action within the collection
      //! 
      //! \return ActionPtr<ENC> - Shared Action pointer, possibly empty
      /////////////////////////////////////////////////////////////////////////////////////////
      ActionPtr<ENC>  find(CommandId id) const 
      {
        // Lookup action
        for (const auto& group : *this)
          if (auto cmd = group.second->find(id))
            return cmd;

        // [NOT FOUND] Return empty pointer
        return ActionPtr<ENC>(nullptr);
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ActionGroupCollection::operator +=
      //! Add a group to the collection
      //!
      //! \param[in] *group - Command group
      //! \return ActionGroupCollection& - Reference to self
      //! 
      //! \throw wtl::invalid_argument - [Debug only] Missing group
      /////////////////////////////////////////////////////////////////////////////////////////
      ActionGroupCollection& operator += (ActionGroup<ENC>* group)
      {
        REQUIRED_PARAM(group);

        // Insert/overwrite
        emplace(group->ident(), ActionGroupPtr<ENC>(group));
        return *this;
      }
    };
    
    //! \alias CreateStruct - Define WM_CREATE/WM_NCCREATE creation data
    using CreateStruct = getType<char_t,::CREATESTRUCTA,::CREATESTRUCTW>;

    //! \alias WindowCollection - Window collection type
    using WindowCollection = List<WindowBase*>;
    
    //! \alias WindowHandleCollection - Provides an association between native window handles and WindowBase objects
    using WindowHandleCollection = std::map<::HWND,WindowBase*>;

    //! \alias WindowIdCollection - Provides an association between window Ids and WindowBase objects
    using WindowIdCollection = std::map<WindowId,WindowBase*>;
    
    //! \alias ActiveWindowCollection - Define 'Active Windows' collection type
    using ActiveWindowCollection = WindowHandleCollection;

    //! \alias ChildWindowCollection - Define child window collection type
    struct ChildWindowCollection : WindowIdCollection
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
      // ----------------------------------- REPRESENTATION -----------------------------------
    protected:
      window_t&  Parent;        //!< Parent/owner of collection
      
      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::ChildWindowCollection
      //! Create empty collection
      //! 
      //! \param[in] &parent - Parent/owner of collection
      /////////////////////////////////////////////////////////////////////////////////////////
      ChildWindowCollection(window_t& parent) : Parent(parent)
      {}
      
      // -------------------------------- COPY & MOVE SEMANTICS -------------------------------

      // ----------------------------------- STATIC METHODS -----------------------------------

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::create
      //! Creates a child window and inserts it into the collection
      //! 
      //! \tparam ENC - Window text string encoding
      //! \tparam LEN - Window text buffer capacity
      //!
      //! \param[in,out] &child - Child window object  (Handle must not exist)
      //! \param[in] const& text - Window text
      //! 
      //! \throw wtl::logic_error - Window already exists
      //! \throw wtl::platform_error - Unable to create window
      /////////////////////////////////////////////////////////////////////////////////////////
      template <Encoding ENC, unsigned LEN>
      void create(window_t& child, const CharArray<ENC,LEN>& text)
      {
        // Ensure child doesn't already exist
        if (child.Handle.exists())
          throw logic_error(HERE, "Window already exists");

        // Create child window  (calls 'insert()' if successful)
        child.create(&Parent, text);
      }
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ChildWindowCollection::insert
      //! Inserts an existing child window into the collection
      //!
      //! \param[in,out] &child - Child window object  (Handle must exist)
      //! 
      //! \throw wtl::logic_error - Window does not exist
      /////////////////////////////////////////////////////////////////////////////////////////
      void insert(window_t& child)
      {
        // Ensure child exists
        if (!child.Handle.exists())
          throw logic_error(HERE, "Window does not exist");

        // Add to collection
        this->emplace(child.Ident, &child);
      }
    };
    
    //! \enum WindowType - Define window types
    enum class WindowType
    {
      Library,   //!< Wtl window 
      Native,    //!< Native window
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct SubClass - Represents a subclassed window
    /////////////////////////////////////////////////////////////////////////////////////////
    struct SubClass
    {
      //! \union WindowProc - Window procedure
      union WindowProc
      {
        WindowProc(wndproc_t n) : Native(n)  {}
        WindowProc(wtlproc_t l) : Library(l) {}

        wndproc_t  Native;     //!< Win32
        wtlproc_t  Library;    //!< WTL
      };

      /////////////////////////////////////////////////////////////////////////////////////////
      // SubClass::SubClass
      //! Create SubClass from a WTL or native window
      //! 
      //! \param[in] t - Window type (Whether WTL or native)
      //! \param[in] p - Window procedure
      /////////////////////////////////////////////////////////////////////////////////////////
      SubClass(WindowType t, WindowProc p) : WndProc(p), Type(t)
      {}

      WindowProc  WndProc;    //!< Window procedure
      WindowType  Type;       //!< Window type
    };
    
    //! \alias SubClassCollection - Define subclassed windows collection
    using SubClassCollection = List<SubClass>;
    
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowPropertyImpl - Base class for window properties
    //!
    //! \tparam VALUE - Value type
    //! \tparam TYPE - [optional] Accessibility and representation (Default is mutable reference type)
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename VALUE, PropertyType TYPE>
    struct WindowPropertyImpl : PropertyImpl<VALUE,TYPE>
    {
      friend window_t;    //!< Allow WindowBase to internally set value

      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowPropertyImpl<VALUE,TYPE>;

      //! \alias base - Define base type
      using base = PropertyImpl<VALUE,TYPE>;

      // ----------------------------------- REPRESENTATION -----------------------------------
    protected:
      window_t&  Window;      //!< Owner window

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowPropertyImpl::WindowPropertyImpl
      //! Stores the owner window
      //! 
      //! \param[in,out] &wnd - Owner window
      //! \param[in] &&... args - [optional] Value constructor arguments
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename... ARGS>
      WindowPropertyImpl(window_t& wnd, ARGS&&... args) : base(std::forward<ARGS>(args)...),
                                                          Window(wnd)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      // ----------------------------------- MUTATOR METHODS ----------------------------------
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct ClientRectPropertyImpl - Implements the client rectangle property [Immutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct ClientRectPropertyImpl : WindowPropertyImpl<RectL,PropertyType::ImmutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = ClientRectPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<RectL,PropertyType::ImmutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // ClientRectPropertyImpl::ClientRectPropertyImpl
      //! Create with empty rectangle
      //! 
      //! \param[in] const& wnd - Owner client
      /////////////////////////////////////////////////////////////////////////////////////////
      ClientRectPropertyImpl(window_t& wnd) : base(wnd, default<RectL>())
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // ClientRectPropertyImpl::get const
      //! Get the client rectangle
      //! 
      //! \return argument_t - Current client rectangle
      //! 
      //! \throw wtl::logic_error - Window does not exist
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        RectL rc;
        
        // Ensure exists
        if (!Window.exists())
          throw logic_error(HERE, "Cannot query client rectangle until window exists");
        
        // Query & return client rectangle
        ::GetClientRect(this->Window, (::RECT*)rc);
        return rc;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------
      
      /////////////////////////////////////////////////////////////////////////////////////////
      // ClientRectPropertyImpl::set 
      //! Set the client rectangle
      //! 
      //! \param[in] rc - New client rectangle
      //! 
      //! \throw wtl::logic_error - Window does not exist
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t rc) override
      {
        // Ensure exists
        if (!Window.exists())
          throw logic_error(HERE, "Cannot query client rectangle until window exists");

        // Calculate and set window rectangle from client rectangle + window properties
        if (!::AdjustWindowRectEx((::RECT*)rc, 
                                  enum_cast(this->Window.Style.get()), 
                                  boolean_cast(!this->Window.Menu.empty()), 
                                  enum_cast(this->Window.StyleEx.get())))
          throw platform_error(HERE, "Unable to set client rectangle");
        
        // Ignore base value
      }
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowEnabledPropertyImpl - Implements the window state property [Mutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowEnabledPropertyImpl : WindowPropertyImpl<bool,PropertyType::MutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowEnabledPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<bool,PropertyType::MutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowEnabledPropertyImpl::WindowEnabledPropertyImpl
      //! Create with initial value
      //! 
      //! \param[in] const& wnd - Owner window
      //! \param[in] &&... args - [optional] Value constructor arguments
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename... ARGS>
      WindowEnabledPropertyImpl(window_t& wnd, ARGS&&... args) : base(wnd, std::forward<ARGS>(args)...)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowEnabledPropertyImpl::get const
      //! Get the window state
      //! 
      //! \return argument_t - Window state
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        // [EXISTS] Query window state
        if (Window.exists())
          return boolean_cast(::IsWindowVisible(this->Window));

        // Return cached
        return this->Value;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowEnabledPropertyImpl::set 
      //! Set the window state
      //! 
      //! \param[in] state - Window state
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t state) override
      {
        // Set window state
        if (Window.exists() && !::EnableWindow(Window, boolean_cast(state)))
          throw platform_error(HERE, "Unable to set window state");

        // Update value
        base::set(state);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowFontPropertyImpl - Implements the window font property [Mutable,Reference]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowFontPropertyImpl : WindowPropertyImpl<HFont,PropertyType::MutableRef>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------
      
      //! \alias type - Define own type
      using type = WindowFontPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<HFont,PropertyType::MutableRef>;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowFontPropertyImpl::WindowFontPropertyImpl
      //! Create with initial value of 'npos'
      //! 
      //! \param[in,out] &wnd - Owner window
      //! \param[in] &&... args - [optional] Value constructor arguments
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename... ARGS>
      WindowFontPropertyImpl(window_t& wnd, ARGS&&... args) : base(wnd, std::forward<ARGS>(args)...)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowFontPropertyImpl::set 
      //! Set the window font
      //! 
      //! \param[in] auto font - New font
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(typename base::argument_t font) override
      {
        // Set window font & redraw
        if (Window.exists())
          this->Window.send<WindowMessage::SETFONT>((uintptr_t)font.get(), boolean_cast(true)); 

        // Update value
        base::set(font);
      }
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowIdPropertyImpl - Implements the window Id property [Mutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowIdPropertyImpl : WindowPropertyImpl<WindowId,PropertyType::MutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowIdPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<WindowId,PropertyType::MutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowIdPropertyImpl::WindowIdPropertyImpl
      //! Create with initial value 
      //! 
      //! \param[in] const& wnd - Owner window
      //! \param[in] init - Initial value
      /////////////////////////////////////////////////////////////////////////////////////////
      WindowIdPropertyImpl(window_t& wnd, argument_t init) : base(wnd, init)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowIdPropertyImpl::get const
      //! Get the window Id
      //! 
      //! \return argument_t - Current window Id
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        // [EXISTS] Query window Id
        if (Window.exists())
          return static_cast<WindowId>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_ID) );
        
        // Return cached
        return this->Value;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowIdPropertyImpl::set 
      //! Set the window Id
      //! 
      //! \param[in] id - New window Id
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t id) override
      {
        // [EXISTS] Set window Id
        if (Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_ID, enum_cast(id)))
          throw platform_error(HERE, "Unable to set window Id");

        // Store value
        base::set(id);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowStylePropertyImpl - Implements the window style property [Mutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowStylePropertyImpl : WindowPropertyImpl<WindowStyle,PropertyType::MutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowStylePropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<WindowStyle,PropertyType::MutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowStylePropertyImpl::WindowStylePropertyImpl
      //! Create with initial value 
      //! 
      //! \param[in] const& wnd - Owner window
      //! \param[in] init - Initial value
      /////////////////////////////////////////////////////////////////////////////////////////
      WindowStylePropertyImpl(window_t& wnd, argument_t init) : base(wnd, init)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowStylePropertyImpl::get const
      //! Get the window style
      //! 
      //! \return argument_t - Current window style
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        // [EXISTS] Query window style
        if (Window.exists())
          return enum_cast<WindowStyle>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_STYLE) );
        
        // Return cached
        return this->Value;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowStylePropertyImpl::set 
      //! Set the window style
      //! 
      //! \param[in] style - New window style
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t style) override
      {
        // [EXISTS] Set window style
        if (Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_STYLE, enum_cast(style)))
          throw platform_error(HERE, "Unable to set window style");

        // Store value
        base::set(style);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowStyleExPropertyImpl - Implements the extended window style property [Mutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowStyleExPropertyImpl : WindowPropertyImpl<WindowStyleEx,PropertyType::MutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowStyleExPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<WindowStyleEx,PropertyType::MutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowStyleExPropertyImpl::WindowStyleExPropertyImpl
      //! Create with default value
      //! 
      //! \param[in] const& wnd - Owner window
      //! \param[in] init - Initial value
      /////////////////////////////////////////////////////////////////////////////////////////
      WindowStyleExPropertyImpl(window_t& wnd, argument_t init) : base(wnd, init)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowStyleExPropertyImpl::get const
      //! Get the extended window style
      //! 
      //! \return argument_t - Current extended window style
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        // [EXISTS] Query extended window style
        if (Window.exists())
          return enum_cast<WindowStyleEx>( getFunc<encoding>(::GetWindowLongPtrA,::GetWindowLongPtrW)(this->Window, GWL_EXSTYLE) );

        // Return cached
        return this->Value;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowStyleExPropertyImpl::set 
      //! Set the extended window style
      //! 
      //! \param[in] style - New extended window style
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t style) override
      {
        // [EXISTS] Set extended window style
        if (Window.exists() && !getFunc<encoding>(::SetWindowLongPtrA,::SetWindowLongPtrW)(this->Window, GWL_EXSTYLE, enum_cast(style)))
          throw platform_error(HERE, "Unable to set extended window style");

        // Update value
        base::set(style);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowRectPropertyImpl - Implements the window rectangle property [Mutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowRectPropertyImpl : WindowPropertyImpl<RectL,PropertyType::MutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowRectPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<RectL,PropertyType::MutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowRectPropertyImpl::WindowRectPropertyImpl
      //! Create with initial value
      //! 
      //! \param[in] const& wnd - Owner window
      //! \param[in] &&... args - [optional] Value constructor arguments
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename... ARGS>
      WindowRectPropertyImpl(window_t& wnd, ARGS&&... args) : base(wnd, std::forward<ARGS>(args)...)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowRectPropertyImpl::get const
      //! Get the window rectangle
      //! 
      //! \return argument_t - Current window rectangle
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        // [EXISTS] Query window rectangle
        if (Window.exists())
        {
          RectL rc;
          ::GetWindowRect(this->Window, (::RECT*)rc);
          return rc;
        }

        // Return cached
        return this->Value;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowRectPropertyImpl::set 
      //! Set the window rectangle
      //! 
      //! \param[in] rc - New window rectangle
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t rc) override
      {
        // [EXISTS] 
        if (Window.exists())
        {
          MoveWindowFlags flags = MoveWindowFlags::NoZOrder;

          // [¬RESIZED] Add appropriate flag
          if (this->Value.width() == rc.width() && this->Value.height() == rc.height())
            flags |= MoveWindowFlags::NoSize;

          // [¬MOVED] Add appropriate flag
          if (this->Value.left == rc.left && this->Value.top == rc.top)
            flags |= MoveWindowFlags::NoMove;

          // Set window rect
          if (!::SetWindowPos(Window, default<::HWND>(), rc.left, rc.top, rc.width(), rc.height(), enum_cast(flags)))
            throw platform_error(HERE, "Unable to set window position");
        }

        // Update value
        base::set(rc);
      }
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct WindowVisibilityPropertyImpl - Implements the window visibility property [Mutable,Value]
    /////////////////////////////////////////////////////////////////////////////////////////
    struct WindowVisibilityPropertyImpl : WindowPropertyImpl<Visibility,PropertyType::MutableValue>
    {
      // ---------------------------------- TYPES & CONSTANTS ---------------------------------

      //! \alias type - Define own type
      using type = WindowVisibilityPropertyImpl;

      //! \alias base - Define base type
      using base = WindowPropertyImpl<Visibility,PropertyType::MutableValue>;

      //! \alias argument_t - Inherit argument type
      using argument_t = typename base::argument_t;

      // ----------------------------------- REPRESENTATION -----------------------------------

      // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
    public:
      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowVisibilityPropertyImpl::WindowVisibilityPropertyImpl
      //! Create with initial value
      //! 
      //! \param[in] const& wnd - Owner window
      //! \param[in] &&... args - [optional] Value constructor arguments
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename... ARGS>
      WindowVisibilityPropertyImpl(window_t& wnd, ARGS&&... args) : base(wnd, std::forward<ARGS>(args)...)
      {}

      // ---------------------------------- ACCESSOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowVisibilityPropertyImpl::get const
      //! Get the window visibility
      //! 
      //! \return argument_t - Window visibility
      /////////////////////////////////////////////////////////////////////////////////////////
      argument_t  get() const override
      {
        // [EXISTS] Query window visibility
        if (Window.exists())
        {
          WindowPlacement info;

          // Query window placement
          if (!::GetWindowPlacement(this->Window, &info))
            throw platform_error(HERE, "Unable to query window placement");

          // Extract visibility
          return enum_cast<Visibility>(info.flags);
        }

        // Return cached
        return this->Value;
      }

      // ----------------------------------- MUTATOR METHODS ----------------------------------

      /////////////////////////////////////////////////////////////////////////////////////////
      // WindowVisibilityPropertyImpl::set 
      //! Set the window visibility
      //! 
      //! \param[in] state - Window visibility
      /////////////////////////////////////////////////////////////////////////////////////////
      void set(argument_t state) override
      {
        // Set window visibility
        if (Window.exists() && !::ShowWindow(Window, enum_cast(state)))
          throw platform_error(HERE, "Unable to set window visibility");

        // Update value
        base::set(state);
      }
    };


  public:
    //! \alias ClientRectProperty - Define client rectangle property type  
    using ClientRectProperty = Property<ClientRectPropertyImpl>;

    //! \alias WindowFontProperty - Define window font property type  
    using WindowFontProperty = Property<WindowFontPropertyImpl>;
    
    //! \alias WindowIdProperty - Define window id property type  
    using WindowIdProperty = Property<WindowIdPropertyImpl>;
    
    //! \alias WindowRectProperty - Define window rectangle property type  
    using WindowRectProperty = Property<WindowRectPropertyImpl>;

    //! \alias WindowStyleProperty - Define window style property type 
    using WindowStyleProperty = Property<WindowStylePropertyImpl>;

    //! \alias WindowStyleExProperty - Define extended window style property type 
    using WindowStyleExProperty = Property<WindowStyleExPropertyImpl>;
    
    //! \alias WindowEnabledProperty - Define window visibliity property type 
    using WindowEnabledProperty = Property<WindowEnabledPropertyImpl>;

    //! \alias WindowVisibilityProperty - Define window visibliity property type 
    using WindowVisibilityProperty = Property<WindowVisibilityPropertyImpl>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    //! \var ActiveWindows - Static collection of all existing WTL windows 
    static WindowHandleCollection  ActiveWindows;

    //! \var ActionGroups - Static collection of all Actions groups
    static ActionGroupCollection   ActionGroups;

    //! \var DefaultPosition - Default window creation position
    static const PointL  DefaultPosition;

    //! \var DefaultSize - Default window creation size
    static const SizeL  DefaultSize;
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  public:
    ActionEvent<encoding>            Action;        //!< Raised in response to WM_COMMAND from menu/accelerators
    CreateWindowEvent<encoding>      Create;        //!< Raised in response to WM_CREATE
    CloseWindowEvent<encoding>       Close;         //!< Raised in response to WM_CLOSE
    DestroyWindowEvent<encoding>     Destroy;       //!< Raised in response to WM_DESTROY
    PaintWindowEvent<encoding>       Paint;         //!< Raised in response to WM_PAINT
    ShowWindowEvent<encoding>        Show;          //!< Raised in response to WM_SHOWWINDOW

    ActionQueue                      Actions;       //!< Actions queue
    ChildWindowCollection            Children;      //!< Child window collection
    ClientRectProperty               ClientRect;    //!< Client rectangle property
    WindowEnabledProperty            Enabled;       //!< Window enabled property
    WindowFontProperty               Font;          //!< Window font property
    WindowIdProperty                 Ident;         //!< Child Window Id property
    WindowMenu<encoding>             Menu;          //!< Window menu, possibly empty
    WindowStyleProperty              Style;         //!< Window style property
    WindowStyleExProperty            StyleEx;       //!< Extended window style property
    WindowVisibilityProperty         Visible;       //!< Visibility property
    WindowRectProperty               WindowRect;    //!< Window rectangle property

  protected:
    WindowClass<encoding>&      Class;         //!< Window class reference
    HWnd                        Handle;        //!< Window handle
    SubClassCollection          SubClasses;    //!< Sub-classed windows collection

    // ------------------------------ CONSTRUCTION & DESTRUCTION ----------------------------
  public: 
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::WindowBase
    //! Creates the window object (but not window handle) for an instance of a registered window class
    //! 
    //! \param[in] &cls - Registered window class 
    /////////////////////////////////////////////////////////////////////////////////////////
    WindowBase(wndclass_t& cls) : Class(cls), 
                                  ClientRect(*this),
                                  Children(*this),
                                  Enabled(*this, true),
                                  Font(*this, StockObject::SystemFont),
                                  Ident(*this, zero<WindowId>()),
                                  Handle(default<HWnd>()),
                                  Style(*this, WindowStyle::OverlappedWindow),
                                  StyleEx(*this, WindowStyleEx::None),
                                  Visible(*this, Visibility::ShowNormal),
                                  WindowRect(*this, DefaultPosition, DefaultSize)
    {
      // Accept window creation by default
      Create += new CreateWindowEventHandler<encoding>(this, &WindowBase::onCreate);
      
      // Execute gui commands by default
      Action += new ActionEventHandler<encoding>(this, &WindowBase::onAction);
        
      // Paint window background by default
      Paint += new PaintWindowEventHandler<encoding>(this, &WindowBase::onPaint);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::~WindowBase
    //! Virtual d-tor
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~WindowBase()
    {
    }

    // ----------------------------------- STATIC METHODS -----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::getFocus
    //! Get the window with input focus
    //!
    //! \return window_t* - Window with input focus, or nullptr if focus belongs to another thread
    //!
    //! \throw wtl::domain_error - Input focus belongs to native window on current thread
    /////////////////////////////////////////////////////////////////////////////////////////
    static window_t*  getFocus()
    {
      if (::HWND focus = ::GetFocus())
      {
        // Lookup & return window
        auto wnd = ActiveWindows.find(focus);
        if (wnd != ActiveWindows.end())
          return wnd->second;
        
        // [FAILED] Native window
        throw domain_error(HERE, "Input focus belongs to native window");
      }

      // [N/A] Input focus belongs to another thread
      return nullptr;
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::isUnhandled
    //! Query whether a result is the 'Unhandled' return code
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] res - Message result 
    //! \return bool - True iff result indicates message was unhandled
    /////////////////////////////////////////////////////////////////////////////////////////
    static bool  isUnhandled(WindowMessage msg, LRESULT res)
    {
      switch (msg)
      {
      case WindowMessage::CREATE:         return res == unhandled_result<WindowMessage::CREATE>::value;
      case WindowMessage::DESTROY:        return res == unhandled_result<WindowMessage::DESTROY>::value;
      case WindowMessage::DRAWITEM:       return res == unhandled_result<WindowMessage::DRAWITEM>::value;
      case WindowMessage::GETMINMAXINFO:  return res == unhandled_result<WindowMessage::GETMINMAXINFO>::value;
      case WindowMessage::KILLFOCUS:      return res == unhandled_result<WindowMessage::KILLFOCUS>::value;
      case WindowMessage::SHOWWINDOW:     return res == unhandled_result<WindowMessage::SHOWWINDOW>::value;
      case WindowMessage::SIZE:           return res == unhandled_result<WindowMessage::SIZE>::value;
      case WindowMessage::SETFOCUS:       return res == unhandled_result<WindowMessage::SETFOCUS>::value;
      default:                            return res != 0;
      }
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::WndProc
    //! Class window procedure 
    //!
    //! \param[in] hWnd - Window handle
    //! \param[in] message - Message ident
    //! \param[in] wParam - [optional] Parameter1
    //! \param[in] lParam - [optional] Parameter2
    //! \return LRESULT - Typically zero if handled, non-zero if not    (but not always)
    /////////////////////////////////////////////////////////////////////////////////////////
    static LRESULT WINAPI  WndProc(::HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam)
    {
      WindowBase* wnd(nullptr);   //!< Window object associated with message, if any
       
      try
      {
        // Attempt to lookup window object 
        switch (static_cast<WindowMessage>(message))
        {
        // [CREATE/NCCREATE] Extract instance pointer from ::CreateWindow(..) call parameter data
        case WindowMessage::CREATE:
        case WindowMessage::NCCREATE:
          // Extract instance pointer
          wnd = reinterpret_cast<WindowBase*>( opaque_cast<CreateStruct>(lParam)->lpCreateParams );

          // Temporarily assign a weak handle reference for the duration of creation process
          wnd->Handle = HWnd(hWnd, AllocType::WeakRef);    // Overwritten by strong reference returned from ::CreateWindow over message is processed

          // Add to 'Active Windows' collection
          ActiveWindows[hWnd] = wnd;
          break;

        // [WINDOW EXTENT] Unable to handle on first call in a thread-safe manner
        case WindowMessage::GETMINMAXINFO:
          return getFunc<char_t>(::DefWindowProcA,::DefWindowProcW)(hWnd, message, wParam, lParam);

        // [REMAINDER] Lookup native handle from the 'Active Windows' collection
        default:
          // Lookup window handle
          if (ActiveWindows.find(hWnd) != ActiveWindows.end())
            wnd = ActiveWindows[hWnd];
          break;
        }
        
        // Delegate to instance procedure
        LResult msg = wnd->routeMessage(static_cast<WindowMessage>(message), wParam, lParam);

        // [HANDLED/REFLECTED] Return result
        if (msg.Route != MsgRoute::Unhandled)
          return msg.Result;

        // TODO: Gui Commands

        //// [UNHANDLED GUI-COMMAND] Search for a handler
        //for (window_t* parent = wnd->parent(); parent; parent = parent->parent())
        //{
        //  // Delegate to parent's instance procedure
        //  msg = wnd->routeMessage(static_cast<WindowMessage>(message), wParam, lParam);

        //  // [PARENT HANDLED/REFLECTED] Return result
        //  if (msg.Route != MsgRoute::Unhandled)
        //    return msg.Result;
        //}
      }
      // [ERROR] Exception thrown by handler
      catch (wtl::exception& e)
      {
        cdebug.log(HERE, e);
      }

      // [UNHANDLED/ERROR] Pass back to OS
      LRESULT result = getFunc<char_t>(::DefWindowProcA,::DefWindowProcW)(hWnd, message, wParam, lParam);
      
      // [CREATE/NCCREATE] Cleanup
      switch (auto msg = static_cast<WindowMessage>(message))
      {
      case WindowMessage::CREATE:
      case WindowMessage::NCCREATE:
        // [FAILED] Remove from 'Active Windows' collection
        if (result == -1)
          ActiveWindows.erase(hWnd);
        break;
      }

      // [UNHANDLED/ERROR] Return result
      return result;
    }

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::clientRect const
    //! Get the client rectangle
    //! 
    //! \return RectL - Client rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    /*RectL clientRect() const
    {
      RectL rc;
      ::GetClientRect(Handle, rc);
      return rc;
    }*/
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::data const
    //! Get window data
    //! 
    //! \param[in] atom - Registered class atom
    //! \param[in] instance - Associated instance handle
    //! \return DATA* - Mutable window data pointer
    //! 
    //! \throw wtl::platform_error - Unrecognised window class name
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename DATA>
    DATA* data() const
    {
      return reinterpret_cast<DATA*>( getFunc<char_t>(::GetWindowLongPtrA,::GetWindowLongPtrW)(Handle.get(), GWLP_USERDATA) );
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::exists const
    //! Query whether the window exists
    //! 
    //! \return bool - True iff window exists
    /////////////////////////////////////////////////////////////////////////////////////////
    bool exists() const
    {
      return Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::handle const
    //! Get the shared window handle 
    //! 
    //! \return const HWnd& - Shared window handle
    /////////////////////////////////////////////////////////////////////////////////////////
    const HWnd& handle() const
    {
      return Handle;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::parent const
    //! Get the parent window
    //! 
    //! \return window_t* - Parent window if any, otherwise nullptr
    //!
    //! \throw wtl::domain_error - Parent is native window
    /////////////////////////////////////////////////////////////////////////////////////////
    window_t* parent() const
    {
      // Query parent
      if (::HWND wnd = ::GetParent(Handle))
      {
        // Lookup window in 'Active Windows' collection
        auto pos = ActiveWindows.find(wnd);
        if (pos != ActiveWindows.end())
          pos->second;

        // [ERROR] Parent is native window
        throw domain_error(HERE, "Parent is native window");
      }
      
      // Top level window
      return nullptr;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::windowRect const
    //! Get the window rectangle
    //! 
    //! \return RectL - Window rectangle
    /////////////////////////////////////////////////////////////////////////////////////////
    /*RectL windowRect() const
    {
      RectL rc;
      ::GetWindowRect(Handle, rc);
      return rc;
    }*/
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::operator ::HWND const
    //! Implicit user conversion to native window handle 
    //! 
    //! \return ::HWND - Native handle
    /////////////////////////////////////////////////////////////////////////////////////////
    operator ::HWND() const
    {
      return handle();
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::create
    //! Creates the window (as a child, popup, or overlapped window)
    //! 
    //! \tparam ENC - Window text string encoding
    //! \tparam LEN - Window text buffer capacity
    //!
    //! \param[in,out] *owner - [optional] Parent/owner window   (Required for child windows)
    //! \param[in] const& text - Window text
    //! 
    //! \throw wtl::invalid_argument - Attempting to create a child window without a parent window
    //! \throw wtl::logic_error - Window already exists
    //! \throw wtl::platform_error - Unable to create window
    //! 
    //! \remarks The window handle is first accessible during WM_CREATE (not before during WM_GETMINMAXINFO)
    //! \remarks This is a weak-ref handle assigned by the class window procedure, a strong-ref is returned 
    //! \remarks and saved here if the creation is successful.
    //!
    //! \remarks Child windows are automatically added to the ChildWindowCollection of the parent
    /////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC, unsigned LEN>
    void create(window_t* owner, const CharArray<ENC,LEN>& text)
    {
      // Ensure window does not exist
      if (Handle.exists())
        throw logic_error(HERE, "Window already exists");

      // [CHILD] Create window  
      if (Ident != zero<WindowId>())
      {
        // Require parent window
        if (!owner)
          throw invalid_argument(HERE, "Missing parent window");

        // Require existant parent window
        if (!owner->exists())
          throw logic_error(HERE, "Parent window does not exist");

        // Create as child using window Ident
        Handle = HWnd(Class.Instance, Class.Name, owner->handle(), this, Ident, Style, StyleEx, CharArray<encoding,LEN>(text), WindowRect);

        // Add to parent's collection of child windows
        owner->Children.insert(*this);
      }
      // [POPUP/OVERLAPPED] Create window (possibly with menu)
      else
      {
        ::HWND parent = owner ? (::HWND)owner->handle() : default<::HWND>();          //!< Use parent if any

        // Create as popup/overlapped (Do not supply menu yet to allow client to populate it)
        Handle = HWnd(Class.Instance, Class.Name, parent, this, nullptr, Style, StyleEx, CharArray<encoding,LEN>(text), WindowRect);

        // [MENU] Attach menu if populated during onCreate(..)
        if (!Menu.empty())
          ::SetMenu(Handle, Menu.handle());
      }
    }
      
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::destroy
    //! Destroys the window and menu 
    //!
    //! \throw wtl::platform_error - Unable to destroy window
    /////////////////////////////////////////////////////////////////////////////////////////
    void destroy()
    {
      // Ensure exists
      if (Handle.exists())
      {
        // TODO: Detach menu?
        ::SetMenu(Handle, nullptr);

        // Destroy window 
        Handle.release();

        // NB: Font & Menu handles are released by members
      }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::execute
    //! Executes an Action, adding it to the actions queue
    //! 
    //! \param[in] id - Action id
    //!
    //! \throw wtl::logic_error - Action not found
    /////////////////////////////////////////////////////////////////////////////////////////
    void  execute(CommandId id) 
    { 
      // Lookup action and execute 
      if (auto cmd = ActionGroups.find(id))
        Actions.execute(cmd->clone());
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::find
    //! Find a child window
    //! 
    //! \tparam WINDOW - [optional] Child window type (If unspecified a base window pointer is returned)
    //! \tparam IDENT - [optional] Window id type (If unspecified, WindowId is used)
    //! 
    //! \param[in] child - Child window Id
    //! \return WINDOW& - Reference to child window
    //! 
    //! \throw wtl::domain_error - Mismatched child window type
    //! \throw wtl::logic_error - Missing child window
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename WINDOW = window_t, typename IDENT = WindowId>
    WINDOW& find(IDENT child) 
    {
      // Lookup child window
      auto pos = Children.find(static_cast<WindowId>(child));
      if (pos != Children.end())
      {
        // [FOUND] Convert & return
        if (WINDOW* wnd = dynamic_cast<WINDOW*>(pos->second))
          return *wnd;

        // [ERROR] Incorrect window type
        throw domain_error(HERE, "Mismatched child window type");
      }

      // [ERROR] Unable to find child window
      throw logic_error(HERE, "Missing child window");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::onCreate
    //! Called during window creation to modify window parameters and create child windows
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onCreate(CreateWindowEventArgs<encoding>& args) 
    { 
      // Initialise properties
      /*Font.init(StockObject::SystemFont);   
      Style.init(args.Style);
      StyleEx.init(args.StyleEx);
      WindowRect.init(args.Rect);*/
      
      // [Handled] Accept parameters
      return 0; 
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::onAction
    //! Called in response to a command raised by menu or accelerator (ie. WM_COMMAND)
    //! 
    //! \param[in,out] &args - Message arguments 
    //! \return LResult - Message result and routing
    //! 
    //! \throw wtl::logic_error - Gui command not recognised
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onAction(ActionEventArgs<encoding>& args) 
    { 
      // Execute associated command
      execute(args.Ident);

      // Handled
      return 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::onPaint
    //! Called to paint the client area of the window
    //! 
    //! \param[in,out] args - Message arguments containing drawing data
    //! \return LResult - Message result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult  onPaint(PaintWindowEventArgs<encoding>& args) 
    { 
      // [Handled] Validate client area
      return 0; 
    }
  
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::post
    //! Posts a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- First parameter
    //! \param[in] l - Second parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    template <WindowMessage WM> 
    void post(WPARAM w = 0, LPARAM l = 0)
    {
      post_message<encoding,WM>(Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::routeMessage
    //! Routes messages to an instance's handlers (This is the 'Instance window procedure')
    //!
    //! \param[in] message - Window message identifier
    //! \param[in] w - [optional] First message parameter
    //! \param[in] l - [optional] Second message parameter
    //! \return LResult - Message routing and result 
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual LResult routeMessage(WindowMessage message, WPARAM w, LPARAM l)
    {
      try
      {
        LResult ret;       //!< Message result, defaults to unhandled

        // [EVENT] Raise event associated with message
        switch (message)
        {
        case WindowMessage::CREATE:         ret = Create.raise(CreateWindowEventArgs<encoding>(w,l));             break;
        case WindowMessage::CLOSE:          ret = Close.raise();                                                  break;
        case WindowMessage::DESTROY:        ret = Destroy.raise();                                                break;
        case WindowMessage::SHOWWINDOW:     ret = Show.raise(ShowWindowEventArgs<encoding>(w,l));                 break;

        // [COMMAND] Reflect control events. Raise Gui events.
        case WindowMessage::COMMAND:  
          if (l != 0)
            // [CONTROL] Reflect to sender
            ret = CtrlCommandEventArgs<encoding>(w,l).reflect();
          else
            // [ACTION] Raise event (Default executes the appropriate command object)
            ret = Action.raise(ActionEventArgs<encoding>(w,l));
          break;

        // [NOTIFY] Reflect to sender
        case WindowMessage::NOTIFY:  
          ret = CtrlNotifyEventArgs<encoding>(w,l).reflect();   
          break;

        // [OWNER-DRAW] Reflect to sender
        case WindowMessage::DRAWITEM:       
          // [CONTROL] Reflect to originator control
          if (w != 0)
            ret = OwnerDrawCtrlEventArgs<encoding>(w,l).reflect();

          // [MENU] Raise menu's OwnerDraw event
          else 
            ret = Menu.OwnerDraw.raise(OwnerDrawMenuEventArgs<encoding>(w,l));
          break;
        
        // [OWNER-MEASURE] Reflect to sender
        case WindowMessage::MEASUREITEM: 
          // [CONTROL] Reflect to originator
          if (w != 0)
            ret = OwnerMeasureCtrlEventArgs<encoding>(find(window_id(w)).handle(),w,l).reflect();

          // [MENU] Raise associated menu event
          else 
            ret = Menu.OwnerMeasure.raise(OwnerMeasureMenuEventArgs<encoding>(Handle,w,l));
          break;

        // [PAINT] Avoid instantiating arguments if event is empty (thereby leaving update region invalidated)
        case WindowMessage::PAINT:          
          if (!Paint.empty())
            ret = Paint.raise(PaintWindowEventArgs<encoding>(Handle,w,l));       
          break;
        }

        // [SUB-CLASS] Offer message to each subclass in turn (if any)
        for (auto& wnd : SubClasses)
          switch (wnd.Type)
          {
          // [WTL WINDOW] Delegate to window object 
          case WindowType::Library:
            // Delegate to instance window procedure
            ret = wnd.WndProc.Library(message, w, l);

            // [HANDLED/REFLECTED] Return result & routing
            if (ret.Route == MsgRoute::Handled || ret.Route == MsgRoute::Reflected)
              return ret;
            break;

          // [NATIVE WINDOW] Call window procedure via Win32 API and determine routing from result
          case WindowType::Native:
            // Delegate to native class window procedure and infer routing
            ret.Result = getFunc<char_t>(::CallWindowProcA,::CallWindowProcW)(wnd.WndProc.Native, Handle, enum_cast(message), w, l);
            ret.Route = (isUnhandled(message, ret.Result) ? MsgRoute::Unhandled : MsgRoute::Handled);
          
            // [HANDLED] Return result & routing
            if (ret.Route == MsgRoute::Handled)
              return ret;
          }

        // [UNHANDLED] Return result & routing
        return ret;
      }
      catch (wtl::exception& e)
      {
        cdebug.log(HERE, e);
        
        // [ERROR] Unhandled
        return MsgRoute::Unhandled;
      }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::send
    //! Sends a message to the window
    //! 
    //! \tparam WM - Window Message 
    //!
    //! \param[in] w- First parameter
    //! \param[in] l - Second parameter
    //! \return LResult - Message result & routing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <WindowMessage WM> 
    LResult send(WPARAM w = 0, LPARAM l = 0)
    {
      return send_message<encoding,WM>(Handle, w, l);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::setFont
    //! Set the window font
    //! 
    //! \param[in] const& f - New window font
    //! \param[in] l - Second parameter
    /////////////////////////////////////////////////////////////////////////////////////////
    //void setFont(const HFont& f, bool redraw)
    //{
    //  // Update shared window font
    //  send<WindowMessage::SETFONT>((uintptr_t)(Font=f).get(), boolean_cast(redraw));
    //}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::show
    //! Show or hide the window
    //! 
    //! \param[in] mode - Display method
    /////////////////////////////////////////////////////////////////////////////////////////
    void show(ShowWindowFlags mode)
    {
      ::ShowWindow(Handle, enum_cast(mode));
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::getText
    //! Get the window text
    //! 
    //! \tparam LEN - Output buffer capacity
    //! 
    //! \param[in,out] &txt - On return this contains the window text
    //! \return int32 - Length in characters
    //! 
    //! \throw wtl::platform_error - Unable to get window text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    int32 getText(const CharArray<encoding,LEN>& txt)
    {
      // Lookup window text
      int32 n = getFunc<char_t>(::GetWindowTextA,::GetWindowTextW)(Handle, txt, LEN);
      if (n || !::GetLastError())
        return n;

      // Failed: Error
      throw platform_error(HERE, "Unable to get window text");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::setText
    //! Set the window text
    //! 
    //! \tparam LEN - Input buffer capacity
    //! 
    //! \param[in] const& txt - Window text
    //! 
    //! \throw wtl::platform_error - Unable to set window text
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned LEN>
    void setText(const CharArray<encoding,LEN>& txt)
    {
      // Set window text
      if (getFunc<char_t>(::SetWindowTextA,::SetWindowTextW)(Handle, txt) == FALSE)
        throw platform_error(HERE, "Unable to set window text");
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // WindowBase::update
    //! Updates the window (Sends a WM_PAINT message if any portion of the window is invalidated)
    /////////////////////////////////////////////////////////////////////////////////////////
    void update()
    {
      ::UpdateWindow(Handle);
    }
  };

  
  //! \var WindowBase<ENC>::ActionGroups - Collection of all Action groups 
  template <Encoding ENC>
  typename WindowBase<ENC>::ActionGroupCollection   WindowBase<ENC>::ActionGroups;
  
  //! \var WindowBase<ENC>::ActiveWindows - Collection of all WTL windows 
  template <Encoding ENC>
  typename WindowBase<ENC>::WindowHandleCollection   WindowBase<ENC>::ActiveWindows;

  
  //! \var WindowBase<ENC>::DefaultPosition - Default window creation position
  template <Encoding ENC>
  const PointL  WindowBase<ENC>::DefaultPosition(CW_USEDEFAULT, CW_USEDEFAULT);

  //! \var WindowBase<ENC>::DefaultSize - Default window creation size
  template <Encoding ENC>
  const SizeL  WindowBase<ENC>::DefaultSize(CW_USEDEFAULT, CW_USEDEFAULT);
    
}

#endif // WTL_WINDOW_BASE_HPP
