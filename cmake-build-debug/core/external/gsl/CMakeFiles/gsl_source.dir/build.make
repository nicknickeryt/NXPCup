# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/NXP_Cup/NXP_2019

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/NXP_Cup/NXP_2019/cmake-build-debug

# Utility rule file for gsl_source.

# Include the progress variables for this target.
include core/external/gsl/CMakeFiles/gsl_source.dir/progress.make

core/external/gsl/CMakeFiles/gsl_source: core/external/gsl/CMakeFiles/gsl_source-complete


core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-install
core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-mkdir
core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download
core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-patch
core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure
core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-build
core/external/gsl/CMakeFiles/gsl_source-complete: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/CMakeFiles
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/CMakeFiles/gsl_source-complete
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-done

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-install: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E echo_append
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-install

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/tmp
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E make_directory D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-mkdir

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-gitinfo.txt
core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -P D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/tmp/gsl_source-gitclone.cmake
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-patch: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No patch step for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E echo_append
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-patch

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure: core/external/gsl/gsl/tmp/gsl_source-cfgcmd.txt
core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-skip-update
core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No configure step for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E echo_append
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-build: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No build step for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E echo_append
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-build && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E touch D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-build

core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-skip-update: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:/NXP_Cup/NXP_2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Skipping update step for 'gsl_source'"
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/gsl/src/gsl_source && "C:/Program Files/JetBrains/CLion 2019.3.2/bin/cmake/win/bin/cmake.exe" -E echo_append

gsl_source: core/external/gsl/CMakeFiles/gsl_source
gsl_source: core/external/gsl/CMakeFiles/gsl_source-complete
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-install
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-mkdir
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-download
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-patch
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-configure
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-build
gsl_source: core/external/gsl/gsl/src/gsl_source-stamp/gsl_source-skip-update
gsl_source: core/external/gsl/CMakeFiles/gsl_source.dir/build.make

.PHONY : gsl_source

# Rule to build all files generated by this target.
core/external/gsl/CMakeFiles/gsl_source.dir/build: gsl_source

.PHONY : core/external/gsl/CMakeFiles/gsl_source.dir/build

core/external/gsl/CMakeFiles/gsl_source.dir/clean:
	cd D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl && $(CMAKE_COMMAND) -P CMakeFiles/gsl_source.dir/cmake_clean.cmake
.PHONY : core/external/gsl/CMakeFiles/gsl_source.dir/clean

core/external/gsl/CMakeFiles/gsl_source.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/NXP_Cup/NXP_2019 D:/NXP_Cup/NXP_2019/core/external/gsl D:/NXP_Cup/NXP_2019/cmake-build-debug D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl D:/NXP_Cup/NXP_2019/cmake-build-debug/core/external/gsl/CMakeFiles/gsl_source.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : core/external/gsl/CMakeFiles/gsl_source.dir/depend

