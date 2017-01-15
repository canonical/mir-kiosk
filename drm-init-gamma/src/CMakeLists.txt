include_directories(${DRM_INCLUDE_DIRS})

add_executable(drm-init-gamma drm-init-gamma.cpp)
target_link_libraries(drm-init-gamma ${DRM_LDFLAGS} ${DRM_LIBRARIES})

install(
  TARGETS drm-init-gamma
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
