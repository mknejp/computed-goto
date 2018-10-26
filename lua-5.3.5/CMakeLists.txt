add_library(lua-vm-switch OBJECT
  src/lvm.c
  src/lvm.h
)
target_include_directories(lua-vm-switch PUBLIC src)

add_library(lua-vm-goto OBJECT
  src/lvm.c
  src/lvm.h
)
target_include_directories(lua-vm-goto PUBLIC src)
target_compile_definitions(lua-vm-goto PRIVATE USE_COMPUTED_GOTO=1)

add_library(lua-core OBJECT
  src/lapi.c
  src/lapi.h
  src/lcode.c
  src/lcode.h
  src/lctype.c
  src/lctype.h
  src/ldebug.c
  src/ldebug.h
  src/ldo.c
  src/ldo.h
  src/ldump.c
  src/lfunc.c
  src/lfunc.h
  src/lgc.c
  src/lgc.h
  src/llex.c
  src/llex.h
  src/llimits.h
  src/lmem.c
  src/lmem.h
  src/lobject.c
  src/lobject.h
  src/lopcodes.c
  src/lopcodes.h
  src/lparser.c
  src/lparser.h
  src/lprefix.h
  src/lstate.c
  src/lstate.h
  src/lstring.c
  src/lstring.h
  src/ltable.c
  src/ltable.h
  src/ltm.c
  src/ltm.h
  src/lua.h
  src/lua.hpp
  src/lundump.c
  src/lundump.h
  src/lzio.c
  src/lzio.h
)
target_include_directories(lua-core PUBLIC src)

add_library(lua-lib OBJECT
  src/lauxlib.c
  src/lauxlib.h
  src/lbaselib.c
  src/lbitlib.c
  src/lcorolib.c
  src/ldblib.c
  src/liolib.c
  src/lmathlib.c
  src/loslib.c
  src/lstrlib.c
  src/ltablib.c
  src/lutf8lib.c
  src/loadlib.c
  src/linit.c
)
target_include_directories(lua-lib PUBLIC src)

add_executable(lua-switch src/lua.c $<TARGET_OBJECTS:lua-vm-switch> $<TARGET_OBJECTS:lua-core> $<TARGET_OBJECTS:lua-lib>)
add_executable(lua-goto src/lua.c $<TARGET_OBJECTS:lua-vm-goto> $<TARGET_OBJECTS:lua-core> $<TARGET_OBJECTS:lua-lib>)

add_executable(luac-switch src/luac.c $<TARGET_OBJECTS:lua-vm-switch> $<TARGET_OBJECTS:lua-core> $<TARGET_OBJECTS:lua-lib>)
add_executable(luac-goto src/luac.c $<TARGET_OBJECTS:lua-vm-goto> $<TARGET_OBJECTS:lua-core> $<TARGET_OBJECTS:lua-lib>)
