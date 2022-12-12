

#ifndef LUAINTERFACE_H_
#define LUAINTERFACE_H_

#include "lua.h"

//
#define LUA_INTERFACE_LIBS {"uc", luaopen_uc}


LUAMOD_API int (luaopen_uc) (lua_State *L);



#include "hardware/uart.h"

/* print a string */
#if !defined(lua_writestring)
#define lua_writestring(s,l)   fwrite((s), sizeof(char), (l), stdout)
#endif

/* print a newline and flush the output */
#if !defined(lua_writeline)
#define lua_writeline()        (lua_writestring("\n", 1), fflush(stdout))
#endif

/* print an error message */
#if !defined(lua_writestringerror)
#define lua_writestringerror(s,p) \
        (fprintf(stderr, (s), (p)), fflush(stderr))
#endif

#define lua_writestringWithoutsize(s)   printf("%s", s);

#endif /* LUAINTERFACE_H_ */
