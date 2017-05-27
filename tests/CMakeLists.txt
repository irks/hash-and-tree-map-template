find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(aisdiMapsTests test_main.cpp TreeMapTests.cpp HashMapTests.cpp)
target_link_libraries(aisdiMapsTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(boostUnitTestsRun aisdiMapsTests)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      --build-config "$<CONFIGURATION>"
      DEPENDS aisdiMapsTests)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      DEPENDS aisdiMapsTests)
endif()
