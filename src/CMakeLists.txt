ADD_EXECUTABLE(Ci40-ew17-uart-to-ds main.c)
FIND_LIBRARY(LIB_AWA libawa.so ${STAGING_DIR}/usr/lib)
FIND_LIBRARY(LIB_LMC libletmecreate_core.so ${STAGING_DIR}/usr/lib)
TARGET_LINK_LIBRARIES(Ci40-ew17-uart-to-ds ${LIB_AWA} ${LIB_LMC})

INSTALL(TARGETS Ci40-ew17-uart-to-ds RUNTIME DESTINATION bin)

