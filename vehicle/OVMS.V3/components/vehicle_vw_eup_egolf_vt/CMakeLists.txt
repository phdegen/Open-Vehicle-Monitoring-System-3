set(srcs)
set(include_dirs)

if (CONFIG_OVMS_VEHICLE_VW_EUP_EGOLF)
  list(APPEND srcs "src/BCM_pids.cpp" "src/EVC_pids.cpp" "src/vehicle_vw_eup_egolf.cpp")
  list(APPEND include_dirs "src")
endif ()

# requirements can't depend on config
idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${include_dirs}
                       PRIV_REQUIRES "main"
                       WHOLE_ARCHIVE)
