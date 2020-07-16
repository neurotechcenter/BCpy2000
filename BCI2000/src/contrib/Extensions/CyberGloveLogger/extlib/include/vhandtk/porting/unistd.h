#if defined(_WIN32)
#ifndef _UNISTD_H_
#define _UNISTD_H_
// FAKE UNISTD !
#endif	/* _UNISTD_H_ */
#else
#include "/usr/include/unistd.h"
#endif
