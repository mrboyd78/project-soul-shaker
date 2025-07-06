cmake_minimum_required(VERSION 3.15)

project(SOULShaker VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.0
)
FetchContent_MakeAvailable(juce)

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
