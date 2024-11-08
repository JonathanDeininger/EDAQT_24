cmake_minimum_required(VERSION 3.10)

project(AudioPlayer)

# Setzt den C++ Standard auf C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Installation Pfad anpassen, falls nötig
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/mingw_64")

# Findet die benötigten Qt-Module für die Audio-Wiedergabe im Terminal
find_package(Qt6 REQUIRED COMPONENTS Core Multimedia Widgets Sql)

# Quell-Dateien festlegen
set(SOURCE_FILES src/main.cpp src/CommandListener.cpp src/Playlist.cpp src/MediaController.cpp src/DataBase.cpp)

# Header-Dateien festlegen
set(HEADER_FILES src/Playlist.h src/CommandListener.h src/MediaController.h src/DataBase.h)

# Erzeugt das ausführbare Ziel "AudioPlayer"
add_executable(AudioPlayer ${SOURCE_FILES} ${HEADER_FILES})

# Verknüpft das Ziel mit den Qt- und Multimedia-Modulen
target_link_libraries(AudioPlayer Qt6::Core Qt6::Multimedia Qt6::Widgets Qt6::Sql)

# Aktiviert die MOC-Generierung für die QObject-erbenden Klassen
set_target_properties(AudioPlayer PROPERTIES
    AUTOMOC ON
)