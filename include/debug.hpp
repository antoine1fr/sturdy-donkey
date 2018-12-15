#include "build.hpp"

#if defined(STURDY_DONKEY_MACOS)

# include <sys/kdebug_signpost.h>

inline void signpost_start(int a, int b, int c, int d, int e)
{
	kdebug_signpost_start(a, b, c, d, e);
}

inline void signpost_end(int a, int b, int c, int d, int e)
{
	kdebug_signpost_end(a, b, c, d, e);
}

#else

inline void signpost_start(int, int, int, int, int)
{
}

inline void signpost_end(int, int, int, int, int)
{
}

#endif // defined(STURDY_DONKEY_MACOS)

