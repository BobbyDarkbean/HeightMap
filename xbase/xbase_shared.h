#ifndef _Xbase_Shared_h_
#define _Xbase_Shared_h_


#include "qobjectdefs.h"


#ifdef HEIGHTMAPX_LIB
#define HMX_SHARED Q_DECL_EXPORT
#else
#define HMX_SHARED Q_DECL_IMPORT
#endif // HEIGHTMAPX_LIB


#endif // _Xbase_Shared_h_
