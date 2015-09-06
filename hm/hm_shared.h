#ifndef _Hm_Shared_h_
#define _Hm_Shared_h_


#ifdef OS_WIN
#define DECLSPEC_EXPORT __declspec(dllexport)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_EXPORT
#define DECLSPEC_IMPORT
#endif // OS_WIN


#ifdef HEIGHTMAP_LIB
#define HM_SHARED DECLSPEC_EXPORT
#else
#define HM_SHARED DECLSPEC_IMPORT
#endif // HEIGHTMAP_LIB


#if !defined(HEIGHTMAP_LIB) && defined(OS_WIN)
#define HM_TEMPLATE extern
#else
#define HM_TEMPLATE
#endif // !defined(HEIGHTMAP_LIB) && defined(OS_WIN)


#endif // _Hm_Shared_h_
