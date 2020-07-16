#include <stdio.h>
/* prevent linker errors when linking against newer versions of msvc runtime */
# if _MSC_VER >= 1900
#  pragma comment(lib, "legacy_stdio_definitions.lib")
extern "C" { FILE __iob_func[3] = { *stdin, *stdout, *stderr }; }
# endif
