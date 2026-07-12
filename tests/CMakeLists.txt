# =============================================================================
# tests/CMakeLists.txt - Unit-test target voor Google Test.
# =============================================================================
# Tests draaien automatisch via:
#   cd build && ctest --output-on-failure
# of bij elke deploy via scripts/pi-update.sh.
#
# Belangrijk: we linken hier alleen domain/infra-source die GEEN Qt
# nodig heeft. HallSensor.cpp / MainWindow etc blijven uit de test-build.
# Test-files gebruiken indien nodig hun eigen stub-subklasse (zie Sensor_test.cpp).
# =============================================================================

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()
include(GoogleTest)

add_executable(unit_tests
    MockGpio_test.cpp
    Sensor_test.cpp
    Deur_test.cpp
    Schuifdeur_test.cpp
    Servo_test.cpp
    SleutelSlot_test.cpp
    CodeSlot_test.cpp
    HerkenningsSlot_test.cpp
    IdKaart_test.cpp
    KaartSlot_test.cpp

    # Source files (zonder Qt-afhankelijkheid)
    ../src/infra/MockGpio.cpp
    ../src/infra/Servo.cpp
    ../src/infra/HallSensor.cpp
    ../src/domain/Deur.cpp
    ../src/domain/Schuifdeur.cpp
    ../src/domain/Draaideur.cpp
    ../src/domain/SleutelSlot.cpp
    ../src/domain/CodeSlot.cpp
    ../src/domain/HerkenningsSlot.cpp
    ../src/domain/IdKaart.cpp
    ../src/domain/KaartSlot.cpp
)

target_include_directories(unit_tests PRIVATE
    ../src
    ../src/infra
    ../src/domain
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Ontdek alle TEST(...) macros automatisch zodat ctest ze ziet.
gtest_discover_tests(unit_tests)
