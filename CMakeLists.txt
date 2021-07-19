cmake_minimum_required(VERSION 3.19)
project(CrossLanguageMatch)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Target for generating executable
include_directories(include)
file(GLOB SOURCES "src/*.cc")
add_executable(CrossLanguageMatch ${SOURCES} assets/)

# Target for copying assets into build directory.
# By using custom target, we can ensure modifications get copied as well.
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )
add_dependencies(CrossLanguageMatch copy_assets)

# Link to SDL2 library
find_library(SDL2_LIBRARY
        NAMES SDL2
        HINTS /Library/Frameworks)
target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_LIBRARY})

# Link to SDL2_image library (allows loading non-BMP images)
find_library(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS /Library/Frameworks)
target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_IMAGE_LIBRARY})

# Link to SDL2_mixer library (simplifies audio)
find_library(SDL2_MIXER_LIBRARY
        NAMES SDL2_mixer
        HINTS /Library/Frameworks)
target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_MIXER_LIBRARY})

# Link to SDL2_tff library (for fonts)
find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS /Library/Frameworks)
target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_TTF_LIBRARY})

# Link to fmt library
# https://github.com/fmtlib/fmt/releases/tag/8.0.1
set(FMT_BUILD_DIRECTORY /Users/alex/Libraries/fmt-8.0.1/build)
set(FMT_HEADER_DIRECTORY /Users/alex/Libraries/fmt-8.0.1/include)
find_library(FMT_LIBRARY
        NAMES fmt
        HINTS ${FMT_BUILD_DIRECTORY})
include_directories(${FMT_HEADER_DIRECTORY})
target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${FMT_LIBRARY})