cmake_minimum_required(VERSION 3.19)
project(CrossLanguageMatch)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Target for generating executable
file(GLOB_RECURSE SOURCES "src/*.cc")
add_executable(CrossLanguageMatch ${SOURCES} assets/)
target_include_directories(CrossLanguageMatch PUBLIC include)

# Target for copying assets into build directory.
# By using custom target, we can ensure modifications get copied as well.
add_custom_target(
        copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(CrossLanguageMatch copy_assets)

if (EMSCRIPTEN)

    set(USE_FLAGS "-O3 -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_BOOST_HEADERS=1 --preload-file assets --use-preload-plugins")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} -s ASYNCIFY -s EXPORTED_FUNCTIONS=_main,_persist_file -s EXPORTED_RUNTIME_METHODS=ccall")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

else ()

    find_library(
            SDL2_LIBRARY
            NAMES SDL2
            HINTS /Library/Frameworks
    )
    target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_LIBRARY})
    find_library(
            SDL2_TTF_LIBRARY
            NAMES SDL2_ttf
            HINTS /Library/Frameworks/SDL2_ttf
    )
    include_directories(/Library/Frameworks/SDL2_ttf.framework/Headers)
    target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_TTF_LIBRARY})
    set(BOOST_HEADER_DIRECTORY /Users/alex/Libraries/boost_1_76_0)
    target_include_directories(CrossLanguageMatch PUBLIC ${BOOST_HEADER_DIRECTORY})

endif ()

