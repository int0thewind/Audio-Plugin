cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(SoftVelvet VERSION 0.1.0)

set(PROJECT_PLUGIN_NAME ${PROJECT_NAME}Plugin)
set(PROJECT_APP_NAME ${PROJECT_NAME}App)

add_subdirectory(packages/JUCE)

# Plugin CMake
juce_add_plugin(
        ${PROJECT_PLUGIN_NAME}

        FORMATS AU Standalone

        PLUGIN_MANUFACTURER_CODE Juce
        PLUGIN_CODE Dem0

        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE

        COPY_PLUGIN_AFTER_BUILD TRUE
        VST3_COPY_DIR $ENV{HOME}/Library/Audio/Plug-Ins/VST
        AU_COPY_DIR $ENV{HOME}/Library/Audio/Plug-Ins/Components

        VST3_CATEGORIES Filter
        AU_MAIN_TYPE kAudioUnitType_Effect
)
juce_enable_copy_plugin_step(${PROJECT_PLUGIN_NAME})
target_sources(
        ${PROJECT_PLUGIN_NAME}
        PUBLIC
        main.cpp
        PRIVATE
        src/dlog.cpp
        src/AudioPluginProcessor.cpp
        src/processors/BaseAudioProcessor.cpp
        src/processors/VelvetNoiseFilter.cpp
        src/processors/LowShelfFilter.cpp
)
target_compile_definitions(
        ${PROJECT_PLUGIN_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(
        ${PROJECT_PLUGIN_NAME}
        PRIVATE
        juce::juce_core
        juce::juce_events
        juce::juce_audio_basics
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_dsp
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
juce_generate_juce_header(${PROJECT_PLUGIN_NAME})

# APP CMake Target
juce_add_gui_app(
        ${PROJECT_APP_NAME}
        VERSION ${PROJECT_VERSION}
        PRODUCT_NAME ${PROJECT_APP_NAME})
target_sources(
        ${PROJECT_APP_NAME}
        PUBLIC
        main.cpp
        PRIVATE
        src/dlog.cpp
        src/AudioPluginProcessor.cpp
        src/MainApplication.cpp
        src/MainComponent.cpp
        src/processors/BaseAudioProcessor.cpp
        src/processors/VelvetNoiseFilter.cpp
        src/processors/LowShelfFilter.cpp
)
target_compile_definitions(
        ${PROJECT_APP_NAME}
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_APP_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_APP_NAME},JUCE_VERSION>")
target_link_libraries(
        ${PROJECT_APP_NAME}
        PRIVATE
        juce::juce_core
        juce::juce_events
        juce::juce_audio_basics
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
juce_generate_juce_header(${PROJECT_APP_NAME})
