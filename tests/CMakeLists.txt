project(Tests)

add_executable(SampleUnitTests SampleUnitTests.cpp)
target_link_libraries(SampleUnitTests gtest gtest_main)
