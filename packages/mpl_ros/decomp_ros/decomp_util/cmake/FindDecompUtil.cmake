# - Try to find DECOMP_UTIL
# Will define
# DECOMP_UTIL_FOUND - If Succeful
# DECOMP_UTIL_INCLUDE_DIRS - The DECOMP_UTIL include directories
# DECOMP_UTIL_LIBRARIES - The DECOMP_UTIL libraty

find_path(DECOMP_UTIL_INCLUDE_DIR
  NAMES "decomp_util/data_type.h"
  PATHS /usr/include /usr/local/include
  )

find_library(DECOMP_UTIL_GEOMETRY_LIBRARY
  NAMES geometry_utils
  PATHS /usr/lib /usr/local/lib
  )
find_library(DECOMP_UTIL_ELLIPSOID_LIBRARY
  NAMES ellipsoid_utils
  PATHS /usr/lib /usr/local/lib
  )
find_library(DECOMP_UTIL_DECOMP_LIBRARY
  NAMES ellipse_decomp
  PATHS /usr/lib /usr/local/lib
  )
find_library(DECOMP_UTIL_ITERATIVE_LIBRARY
  NAMES iterative_decomp
  PATHS /usr/lib /usr/local/lib
  )
find_library(SEED_DECOMP_LIBRARY
  NAMES seed_decomp
  PATHS /usr/lib /usr/local/lib
  )


set(DECOMP_UTIL_INCLUDE_DIRS "${DECOMP_UTIL_INCLUDE_DIR}")
set(DECOMP_UTIL_LIBRARIES "${DECOMP_UTIL_GEOMETRY_LIBRARY};${DECOMP_UTIL_ELLIPSOID_LIBRARY};${DECOMP_UTIL_ITERATIVE_LIBRARY};${DECOMP_UTIL_DECOMP_LIBRARY};${SEED_DECOMP_LIBRARY}" )


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DECOMP_UTIL DEFAULT_MSG DECOMP_UTIL_GEOMETRY_LIBRARY DECOMP_UTIL_DECOMP_LIBRARY DECOMP_UTIL_ELLIPSOID_LIBRARY DECOMP_UTIL_ITERATIVE_LIBRARY SEED_DECOMP_LIBRARY DECOMP_UTIL_INCLUDE_DIR)
mark_as_advanced(DECOMP_UTIL_GEOMETRY_LIBRARY DECOMP_UTIL_DECOMP_LIBRARY DECOMP_UTIL_ELLIPSOID_LIBRARY DECOMP_UTIL_ITERATIVE_LIBRARY SEED_DECOMP_LIBRARY DECOMP_UTIL_INCLUDE_DIR)
