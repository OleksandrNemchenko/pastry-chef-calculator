#   Copyright (C) 2015-2020 Virgil Security Inc.
#
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
#
#       (1) Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#       (2) Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#
#       (3) Neither the name of the copyright holder nor the names of its
#       contributors may be used to endorse or promote products derived from
#       this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
#   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#   DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
#   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
#   IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.
#
#   Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

include(GNUInstallDirs)

# ---------------------------------------------------------------------------
#   Project
# ---------------------------------------------------------------------------

project(pastry-chef-calc VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
#   Qt
# ---------------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Qml CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

add_subdirectory(logger)

add_executable(pastry-chef-calc)

execute_process(
        COMMAND git rev-parse HEAD
        OUTPUT_VARIABLE COMPILE_GIT_HASH
)

string(STRIP ${COMPILE_GIT_HASH} COMPILE_GIT_HASH)

target_compile_definitions(pastry-chef-calc
        PRIVATE
        COMPILE_GIT_HASH="${COMPILE_GIT_HASH}"
        )

target_precompile_headers(pastry-chef-calc
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/app.h
        )

target_sources(pastry-chef-calc
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/app.h
        ${CMAKE_CURRENT_LIST_DIR}/include/database.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tables/metaInfo.h
        ${CMAKE_CURRENT_LIST_DIR}/include/tables/units.h

        ${CMAKE_CURRENT_LIST_DIR}/src/app.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/database.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/tables/metaInfo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/tables/units.cpp

        ${CMAKE_CURRENT_LIST_DIR}/resources.qrc
        )

target_include_directories(pastry-chef-calc
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        )

target_link_libraries(pastry-chef-calc
        PRIVATE

        Qt5::Core
        Qt5::Qml
        Qt5::Quick
        Qt5::Sql
        Qt5::Widgets

        avn_logger_txt_cout
        avn_logger_txt_file
        )
