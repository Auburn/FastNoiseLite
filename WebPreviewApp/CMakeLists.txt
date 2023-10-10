cmake_minimum_required(VERSION 3.12)

project(FastNoiseLitePreview VERSION 1.1.0)
SET(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
    imgui_application
    GIT_REPOSITORY https://github.com/auburn/imgui_application
    GIT_TAG 00c4079834123e27850c6ec818b21fdf2797e5b5
)
FetchContent_MakeAvailable(imgui_application)

FetchContent_Declare(
    emscripten-browser-file
    GIT_REPOSITORY https://github.com/Armchair-Software/emscripten-browser-file
    GIT_TAG 2ff8f8f791c0e7177b40e621ee3f758e03b31bf5
)
FetchContent_MakeAvailable(emscripten-browser-file)
# ----------------------------------------------------------

add_executable(${CMAKE_PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${IMGUI_APPLICATION_INCLUDE_DIRECTORIES}
    ${emscripten-browser-file_SOURCE_DIR}
    ../Cpp
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    FNL_VERSION="v${CMAKE_PROJECT_VERSION}"
)

if(EMSCRIPTEN)

    # IMPORTANT NOTE: due to `target_link_options` deduplication,
    # we use "-sWASM=1 -sUSE_SDL=2 ..." which works well, instead of
    # "-s WASM=1 -s USE_SDL=2 ..." (with spaces), which becomes
    # "-s WASM=1 USE_SDL=2 ..." (duplicated but necessary '-s' removed).

    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
        -sWASM=1
        -sUSE_SDL=2

        -sUSE_WEBGL2=1
        #-sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 # Only target for WebGL2 (drop support for WebGL1 to save code size)

        -sEXPORTED_RUNTIME_METHODS=[ccall]
        -sEXPORTED_FUNCTIONS=[_main,_malloc,_free]

        -sALLOW_MEMORY_GROWTH=1
        -sDISABLE_EXCEPTION_CATCHING=1 -sNO_EXIT_RUNTIME=0 -sASSERTIONS=1

        -sNO_FILESYSTEM=1

        -sSINGLE_FILE

        --shell-file ${IMGUI_APPLICATION_SHELL_MINIMAL}
    )

    target_link_libraries(${CMAKE_PROJECT_NAME}
        imgui_application
    )

    set(CMAKE_EXECUTABLE_SUFFIX ".html")

else()

    target_link_libraries(${CMAKE_PROJECT_NAME}
        imgui_application GL
    )

endif()