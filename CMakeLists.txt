cmake_minimum_required(VERSION 3.19)
project(CrossLanguageMatch)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Target for generating executable
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cc")
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

# Link to SDL2_tff library (for fonts)
find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS /Library/Frameworks)
target_link_libraries(CrossLanguageMatch LINK_PUBLIC ${SDL2_TTF_LIBRARY})

# Link to boost library
set(BOOST_HEADER_DIRECTORY /Users/alex/Libraries/boost_1_76_0)
include_directories(${BOOST_HEADER_DIRECTORY})
