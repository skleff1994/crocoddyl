# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jpfleging/libraries/crocoddyl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jpfleging/libraries/crocoddyl/benchmark

# Utility rule file for distdir.

# Include the progress variables for this target.
include CMakeFiles/distdir.dir/progress.make

CMakeFiles/distdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/jpfleging/libraries/crocoddyl/benchmark/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating dist directory..."
	cd /home/jpfleging/libraries/crocoddyl && rm -f /tmp/crocoddyl.tar && /home/jpfleging/libraries/crocoddyl/cmake/git-archive-all.sh --prefix crocoddyl-1.5.0/ crocoddyl.tar && cd /home/jpfleging/libraries/crocoddyl/benchmark/ && ( test -d crocoddyl-1.5.0 && find crocoddyl-1.5.0/ -type d -print0 | xargs -0 chmod a+w || true ) && rm -rf crocoddyl-1.5.0/ && /bin/tar xf /home/jpfleging/libraries/crocoddyl/crocoddyl.tar && echo 1.5.0 > /home/jpfleging/libraries/crocoddyl/benchmark/crocoddyl-1.5.0/.version && /home/jpfleging/libraries/crocoddyl/cmake/gitlog-to-changelog > /home/jpfleging/libraries/crocoddyl/benchmark/crocoddyl-1.5.0/ChangeLog && rm -f /home/jpfleging/libraries/crocoddyl/crocoddyl.tar

distdir: CMakeFiles/distdir
distdir: CMakeFiles/distdir.dir/build.make

.PHONY : distdir

# Rule to build all files generated by this target.
CMakeFiles/distdir.dir/build: distdir

.PHONY : CMakeFiles/distdir.dir/build

CMakeFiles/distdir.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/distdir.dir/cmake_clean.cmake
.PHONY : CMakeFiles/distdir.dir/clean

CMakeFiles/distdir.dir/depend:
	cd /home/jpfleging/libraries/crocoddyl/benchmark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jpfleging/libraries/crocoddyl /home/jpfleging/libraries/crocoddyl /home/jpfleging/libraries/crocoddyl/benchmark /home/jpfleging/libraries/crocoddyl/benchmark /home/jpfleging/libraries/crocoddyl/benchmark/CMakeFiles/distdir.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/distdir.dir/depend

