#ifndef _Io_Shared_h_
#define _Io_Shared_h_


#ifdef HEIGHTMAPIO_LIB
#define HMIO_SHARED Q_DECL_EXPORT
#else
#define HMIO_SHARED Q_DECL_IMPORT
#endif // HEIGHTMAPIO_LIB


#endif // _Io_Shared_h_
