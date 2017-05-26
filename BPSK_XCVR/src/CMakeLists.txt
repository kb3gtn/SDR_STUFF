add_library(PRBS_GENS PRBS_GENS.cpp)
target_compile_options(PRBS_GENS PRIVATE -std=c++14)
target_include_directories( PRBS_GENS PUBLIC $(CMAKE_CURRENT_SOURCE_DIR) )

add_library(SYMBOL_MAPPERS SYMBOL_MAPPERS.cpp)
target_compile_options(SYMBOL_MAPPERS PRIVATE -std=c++14)
target_include_directories( SYMBOL_MAPPERS PUBLIC $(CMAKE_CURRENT_SOURCE_DIR) )

add_library(RESAMPLERS RESAMPLERS.cpp)
target_compile_options( RESAMPLERS PRIVATE -std=c++14)
target_include_directories( RESAMPLERS PUBLIC $(CMAKE_CURRENT_SOURCE_DIR) )

add_library(FILTERS FILTERS.cpp)
target_compile_options( FILTERS PRIVATE -std=c++14)
target_include_directories( FILTERS PUBLIC $(CMAKE_CURRENT_SOURCE_DIR) )

