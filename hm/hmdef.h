#ifndef _Hmdef_h_
#define _Hmdef_h_


#define DISABLE_COPY(Class) \
Class(const Class &) = delete; \
Class &operator=(const Class &) = delete;


#define DISABLE_MOVE(Class) \
Class(Class &&) = delete; \
Class &operator=(Class &&) = delete;


#endif // _Hmdef_h_
