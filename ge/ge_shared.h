#ifndef _Ge_Shared_h_
#define _Ge_Shared_h_


#include "qobjectdefs.h"


#ifdef HEIGHTMAPGE_LIB
#define HMGE_SHARED Q_DECL_EXPORT
#else
#define HMGE_SHARED Q_DECL_IMPORT
#endif // HEIGHTMAPGE_LIB


#endif // _Ge_Shared_h_
