add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE inc)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
