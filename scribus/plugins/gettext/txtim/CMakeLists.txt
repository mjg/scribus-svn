include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

set(TXT_GT_PLUGIN_SOURCES
txtim.cpp
)

set(SCRIBUS_TXT_GT_PLUGIN "txtim")

add_library(${SCRIBUS_TXT_GT_PLUGIN} MODULE ${TXT_GT_PLUGIN_SOURCES})

link_libraries (${LIBXML2_LIBRARIES})

target_link_libraries(${SCRIBUS_TXT_GT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_TXT_GT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_TXT_GT_PLUGIN}
  LIBRARY
  DESTINATION ${PLUGINDIR}/gettext
  PERMISSIONS ${PLUGIN_PERMISSIONS}
)

# set_target_properties(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")
