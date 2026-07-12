cmake_minimum_required(VERSION 3.16)
project(gebouw VERSION 0.2 LANGUAGES CXX)

# === C++ standaard ===
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Qt automatische tools ===
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# === Qt6 zoeken ===
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# === pigpiod_if2 zoeken (Pi-only) ===
find_library(PIGPIOD_IF2_LIB pigpiod_if2)
if(PIGPIOD_IF2_LIB)
    message(STATUS "pigpiod_if2 gevonden: ${PIGPIOD_IF2_LIB}")
    set(HAS_PIGPIO TRUE)
else()
    message(WARNING "pigpiod_if2 niet gevonden - PigpioGpio compileert als stub")
    set(HAS_PIGPIO FALSE)
endif()

# === Bron-files per laag =====================================================
set(INFRA_SOURCES
    src/infra/MockGpio.cpp
    src/infra/PigpioGpio.cpp
    src/infra/Servo.cpp
    src/infra/HallSensor.cpp
)

set(DOMAIN_SOURCES
    src/domain/Deur.cpp
    src/domain/Schuifdeur.cpp
    src/domain/Draaideur.cpp
    src/domain/SleutelSlot.cpp
    src/domain/CodeSlot.cpp
    src/domain/HerkenningsSlot.cpp
    src/domain/IdKaart.cpp
    src/domain/KaartSlot.cpp
    src/domain/SlotException.cpp
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/Drukbox.cpp
)

# Qt-resource (Gebouw.png) - door AUTORCC verwerkt
set(GEBOUW_RESOURCES
    resources.qrc
)

set(GEBOUW_SOURCES
    src/main.cpp
    ${INFRA_SOURCES}
    ${DOMAIN_SOURCES}
    ${UI_SOURCES}
    ${GEBOUW_RESOURCES}
)

add_executable(gebouw ${GEBOUW_SOURCES})

target_include_directories(gebouw PRIVATE
    src
    src/infra
    src/domain
    src/ui
)

target_link_libraries(gebouw PRIVATE
    Qt6::Core
    Qt6::Widgets
)

if(HAS_PIGPIO)
    target_link_libraries(gebouw PRIVATE ${PIGPIOD_IF2_LIB})
    target_compile_definitions(gebouw PRIVATE HAS_PIGPIO=1)
endif()

# === Unit-tests ==============================================================
enable_testing()
add_subdirectory(tests)
