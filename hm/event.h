/*
 *  C++ event system
 *  by
 *  Alex Vladimirov
 */

/*
    INSTRUCTIONS ON MACRO USAGE

    1. Define desired event-parameter structures. Events can take
    only one parameter. Define typedefs for pointer-types and
    instantiated templates.

    2. Declare events in a sender-class with the declare_event macro.
    It is allowed to declare events with the same names but different
    parameter types. E.g.:

    class MySender
    {
    declare_event(valueChanged, int)
    declare_event(valueChanged, MyStructEventArgs)

    public:
        MySender();
        // ...
    };

    Note: access scopes are set within the macro, so be aware of
    declaring something in the same scope after those macros.

    3. You can raise events from within the sender-class' methods whether
    by calling Event::raise() or by passing event name, parameter type
    and parameter value to the raise_event macro. It is safe to raise
    event with no handlers attached. E.g.:

    void MySender::some_method()
    {
        // ...
        int a = 24;
        raise_event(valueChanged, int, a);
        // or
        e_valueChanged_int.raise(a);
    }

    4. Declare required event handlers in an observer-class with the
    declare_eventhandler macro. While implementing handlers in the
    observer, an "event name-parameter" pair must be unique for each
    sender-class type. E.g.

    class MyObserver
    {
    public:
        MyObserver();
        // ...

    declare_eventhandler(MyObserver, MySender, valueChanged, int)
    declare_eventhandler(MyObserver, MySender, valueChanged, MyStructEventArgs)
    };

    Note: access scopes are set within the macro, so be aware of
    declaring something in the same scope after those macros.

    5. Implement event handlers in observer-class' source file by using
    implement_eventhandler macro. This macro provides default
    implementations for the handler's constructor and destructor and
    exposes the overridden EventHandler::react() method to implement. E.g.:

    // A definition of Control::valueChanged(int)
    // event handler in the Page class
    implement_eventhandler(Page, Control, valueChanged, int)
    {
        // an implementation of EventHandlerSubclass::react().
    }

    6. Create event handlers' instances in the observer's constructor.
    Use an init_eventhandler macro in the initializer list for each
    event handler instance.

    7. Attach handler instances to the events whether by direct calling
    event providers' operator +=() or by using handle_event macro. E.g.:

    // Handling Control::valueChanged(int) event in the Page class
    void Page::some_method()
    {
        Control *c = new Control;
        handle_event(Control, c, this, valueChanged, int);
        // or
        c->event_valueChanged_int() += this->eh_Control_on_valueChanged_int();
        // ...
    }

    8. When using events in a dynamic link library, Event templates
    should be instantiated before the sender-class definition. E.g.:

    #ifdef MY_LIB
    #define MY_LIB_SPEC __declspec(dllexport)
    #define MY_LIB_TEMPL_SPEC
    #else
    #define MY_LIB_SPEC __declspec(dllimport)
    #define MY_LIB_TEMPL_SPEC extern
    #endif // MY_LIB

    export_event(MY_LIB_TEMPL_SPEC, MY_LIB_SPEC, int);
    // expands in DLL to:
    // template class __declspec(dllexport) std::list<EventHandler<int> *>;
    // template class __declspec(dllexport) std::list<Event<int> *>;
    // template class __declspec(dllexport) Event<int>;
    // expands in DLL client to:
    // extern template class __declspec(dllimport) std::list<EventHandler<int> *>;
    // extern template class __declspec(dllimport) std::list<Event<int> *>;
    // extern template class __declspec(dllimport) Event<int>;

    class MY_DLL_SPEC MySender
    {
    declare_event(valueChanged, int)
    // ...
    };
*/


#ifndef _Event_h_
#define _Event_h_


#include <list>


namespace EventService {


template<class EventArgs>
class Event;


/*
    Template class providing pure virtual interface
    for call-back handlers. Implementing EventHandler
    derived classes as nested classes is suggested.
*/
template<class EventArgs>
class EventHandler
{
    typedef Event<EventArgs> _Event;

public:
    EventHandler();
    virtual void react(const EventArgs &eventArgs) = 0;
    virtual ~EventHandler();

private:
    EventHandler(const EventHandler &);
    EventHandler &operator =(const EventHandler &);

    void addEvent(_Event *e);
    void removeEvent(_Event *e);

    template <class T> friend class Event;
    template <class T> friend class EventProvider;

    std::list<_Event *> events;
};


// TODO: provide EventHandler<void> specialization.


/*
    Template class providing interface for performing
    call-backs and call-back handlers management. Event
    should be declared in private scope to be protected
    from unauthorized rasing.
*/
template<class EventArgs>
class Event
{
    typedef EventHandler<EventArgs> Handler;

public:
    Event() : handlers() { }

    ~Event()
    {
        for (typename std::list<Handler *>::iterator itr = handlers.begin();
            itr != handlers.end(); ++itr)
        {
            (*itr)->removeEvent(this);
        }
    }

    void raise(const EventArgs &eventArgs)
    {
        for (typename std::list<Handler *>::iterator itr = handlers.begin();
            itr != handlers.end(); ++itr)
        {
            (*itr)->react(eventArgs);
        }
    }

    bool addHandler(Handler *handler)
    {
        for (typename std::list<Handler *>::const_iterator itr = handlers.begin();
            itr != handlers.end(); ++itr)
        {
            if (*itr == handler) {
                return false;
            }
        }

        handlers.push_back(handler);
        return true;
    }

    bool removeHandler(Handler *handler)
    {
        for (typename std::list<Handler *>::iterator itr = handlers.begin();
            itr != handlers.end(); ++itr)
        {
            if (*itr == handler) {
                handlers.erase(itr);
                return true;
            }
        }

        return false;
    }

private:
    Event(const Event &);
    Event &operator =(const Event &);

    /*
        No memory management is being performed.
        Handlers' life cycle should be managed by
        their outer class.
    */
    std::list<Handler *> handlers;
};


// TODO: provide Event<void> specialization.


/*
    Template class providing an access to Event's
    addHandler() and removeHandler() methods.
*/
template<class EventArgs>
class EventProvider
{
    typedef EventHandler<EventArgs> Handler;

public:
    EventProvider(Event<EventArgs> *e) :
        m_event(e) { }

    EventProvider &operator +=(Handler *handler)
    {
        if (m_event) {
            if (m_event->addHandler(handler))
                handler->addEvent(m_event);
        }

        return *this;
    }

    EventProvider &operator -=(Handler *handler)
    {
        if (m_event) {
            if (m_event->removeHandler(handler))
                handler->removeEvent(m_event);
        }

        return *this;
    }

private:

    /*
        No memory management is being performed.
        This template class is just a shell, not
        a smart-pointer. Copying is allowed.
    */
    Event<EventArgs> *m_event;
};


template<class EventArgs>
EventHandler<EventArgs>::EventHandler() : events() { }


template<class EventArgs>
EventHandler<EventArgs>::~EventHandler()
{
    for (typename std::list<_Event *>::iterator itr = events.begin();
        itr != events.end(); ++itr)
    {
        (*itr)->removeHandler(this);
    }
}


template<class EventArgs>
void EventHandler<EventArgs>::addEvent(_Event *e)
{
    for (typename std::list<_Event *>::const_iterator itr = events.begin();
        itr != events.end(); ++itr)
    {
        if (*itr == e) {
            return;
        }
    }

    events.push_back(e);
}


template<class EventArgs>
void EventHandler<EventArgs>::removeEvent(_Event *e)
{
    for (typename std::list<_Event *>::iterator itr = events.begin();
        itr != events.end(); ++itr)
    {
        if (*itr == e) {
            events.erase(itr);
            return;
        }
    }
}


} // namespace EventService


/*
    Note: typedefs should be used for pointer- and templated types
*/

#define declare_event(EventName, Args) \
protected: \
    ::EventService::Event<Args> e_##EventName##_##Args; \
public: \
    ::EventService::EventProvider<Args> event_##EventName##_##Args() \
    { return ::EventService::EventProvider<Args>(&e_##EventName##_##Args); }

#define declare_eventhandler(OuterClass, SenderClass, EventName, Args) \
public: \
    class SenderClass##_on_##EventName##_##Args \
        : public ::EventService::EventHandler<Args> \
    { \
    public: \
        SenderClass##_on_##EventName##_##Args(OuterClass *); \
        OuterClass *master() const { return m_master; } \
        void react(const Args &); \
        ~SenderClass##_on_##EventName##_##Args(); \
    private: \
        OuterClass *const m_master; \
    }; \
private: \
    friend class SenderClass##_on_##EventName##_##Args; \
    SenderClass##_on_##EventName##_##Args m_eh_##SenderClass##_on_##EventName##_##Args; \
public: \
    SenderClass##_on_##EventName##_##Args *eh_##SenderClass##_on_##EventName##_##Args() \
    { return &m_eh_##SenderClass##_on_##EventName##_##Args; }

#define init_eventhandler(SenderClass, EventName, Args) \
m_eh_##SenderClass##_on_##EventName##_##Args(this)

#define raise_event(EventName, ArgsType, ArgsVal) \
e_##EventName##_##ArgsType.raise(ArgsVal)

#define implement_eventhandler(OuterClass, SenderClass, EventName, Args) \
OuterClass::SenderClass##_on_##EventName##_##Args::SenderClass##_on_##EventName##_##Args( \
    OuterClass *master) : EventHandler<Args>(), m_master(master) { } \
OuterClass::SenderClass##_on_##EventName##_##Args::~SenderClass##_on_##EventName##_##Args() { } \
void OuterClass::SenderClass##_on_##EventName##_##Args::react(const Args &args)

#define handle_event(SenderClass, SenderPtr, ObserverPtr, EventName, ArgsType) \
SenderPtr->event_##EventName##_##ArgsType() \
    += ObserverPtr->eh_##SenderClass##_on_##EventName##_##ArgsType()

#define export_event(ExternTemplateSpec, SharedSpec, Args) \
ExternTemplateSpec template class SharedSpec std::list< ::EventService::EventHandler<Args> *>; \
ExternTemplateSpec template class SharedSpec std::list< ::EventService::Event<Args> *>; \
ExternTemplateSpec template class SharedSpec ::EventService::Event<Args>


#endif // _Event_h_
