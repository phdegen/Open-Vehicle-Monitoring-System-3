set(srcs)
set(include_dirs)

if (CONFIG_OVMS_VEHICLE_NISSANLEAF_ZE1)
  list(APPEND srcs "src/BCM_pids.cpp" "src/EVC_pids.cpp" "src/vehicle_nissanleaf_ze1.cpp")
  list(APPEND include_dirs "src")
endif ()

# requirements can't depend on config
idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${include_dirs}
                       PRIV_REQUIRES "main"
                       WHOLE_ARCHIVE)
