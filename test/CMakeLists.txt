include_directories(catch)

add_executable(unittest test.cpp timezone.cpp)
target_link_libraries(unittest icu_collate Threads::Threads)
