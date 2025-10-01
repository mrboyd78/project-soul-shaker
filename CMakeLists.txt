cmake_minimum_required(VERSION 3.15)

project(SOULShaker VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.0
)
FetchContent_MakeAvailable(juce)

# --- chowdsp_wdf single-header fetch & include ---
set(CHOWDSP_WDF_URL
    "https://raw.githubusercontent.com/chowdhury-dsp/chowdsp_wdf/main/single_include/chowdsp_wdf/chowdsp_wdf.h")

set(CHOWDSP_WDF_EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external/chowdsp_wdf")
set(CHOWDSP_WDF_HEADER "${CHOWDSP_WDF_EXTERNAL_DIR}/chowdsp_wdf.h")

if(NOT EXISTS "${CHOWDSP_WDF_HEADER}")
  message(STATUS "chowdsp_wdf: header not found locally. Creating ${CHOWDSP_WDF_EXTERNAL_DIR} and attempting download...")
  file(MAKE_DIRECTORY "${CHOWDSP_WDF_EXTERNAL_DIR}")
  file(DOWNLOAD
       "${CHOWDSP_WDF_URL}"
       "${CHOWDSP_WDF_HEADER}"
       SHOW_PROGRESS
       STATUS _chowdsp_wdf_download_status
       LOG)
  list(GET _chowdsp_wdf_download_status 0 _chowdsp_wdf_code)
  list(GET _chowdsp_wdf_download_status 1 _chowdsp_wdf_msg)
  if(NOT _chowdsp_wdf_code EQUAL 0)
    message(WARNING "chowdsp_wdf: failed to download header. Code=${_chowdsp_wdf_code} Msg=${_chowdsp_wdf_msg}")
    # do not fatal; allow fallback to JUCE ladder
  else()
    message(STATUS "chowdsp_wdf: downloaded header to ${CHOWDSP_WDF_HEADER}")
  endif()
else()
  message(STATUS "chowdsp_wdf: header already present at ${CHOWDSP_WDF_HEADER}")
endif()

juce_add_plugin(SOULShaker
    COMPANY_NAME "MyCompany"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    PLUGIN_MANUFACTURER_CODE Comp
    PLUGIN_CODE Aiap
    FORMATS VST3 AU AAX
    PRODUCT_NAME "S.O.U.L. Shaker"
    NEEDS_WEBVIEW2 TRUE
    
    # Source files
    SOURCES
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

target_link_libraries(SOULShaker PRIVATE
    juce::juce_audio_utils
    juce::juce_gui_extra
)

# If header exists, add its directory to includes and define a compile-time flag
if(EXISTS "${CHOWDSP_WDF_HEADER}")
  target_include_directories(SOULShaker PRIVATE "${CHOWDSP_WDF_EXTERNAL_DIR}")
  target_compile_definitions(SOULShaker PRIVATE HAVE_CHOWDSP_WDF_SINGLE_HEADER=1)
endif()
