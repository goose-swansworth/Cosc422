# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /csse/users/kjr106/Desktop/Cosc422/assignment-1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /csse/users/kjr106/Desktop/Cosc422/assignment-1/build

# Include any dependencies generated for this target.
include CMakeFiles/main.out.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.out.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.out.dir/flags.make

CMakeFiles/main.out.dir/src/main.cpp.o: CMakeFiles/main.out.dir/flags.make
CMakeFiles/main.out.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/main.out.dir/src/main.cpp.o: CMakeFiles/main.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.out.dir/src/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.out.dir/src/main.cpp.o -MF CMakeFiles/main.out.dir/src/main.cpp.o.d -o CMakeFiles/main.out.dir/src/main.cpp.o -c /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/main.cpp

CMakeFiles/main.out.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.out.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/main.cpp > CMakeFiles/main.out.dir/src/main.cpp.i

CMakeFiles/main.out.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.out.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/main.cpp -o CMakeFiles/main.out.dir/src/main.cpp.s

CMakeFiles/main.out.dir/src/shadow.cpp.o: CMakeFiles/main.out.dir/flags.make
CMakeFiles/main.out.dir/src/shadow.cpp.o: ../src/shadow.cpp
CMakeFiles/main.out.dir/src/shadow.cpp.o: CMakeFiles/main.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.out.dir/src/shadow.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.out.dir/src/shadow.cpp.o -MF CMakeFiles/main.out.dir/src/shadow.cpp.o.d -o CMakeFiles/main.out.dir/src/shadow.cpp.o -c /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/shadow.cpp

CMakeFiles/main.out.dir/src/shadow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.out.dir/src/shadow.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/shadow.cpp > CMakeFiles/main.out.dir/src/shadow.cpp.i

CMakeFiles/main.out.dir/src/shadow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.out.dir/src/shadow.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/shadow.cpp -o CMakeFiles/main.out.dir/src/shadow.cpp.s

CMakeFiles/main.out.dir/src/character.cpp.o: CMakeFiles/main.out.dir/flags.make
CMakeFiles/main.out.dir/src/character.cpp.o: ../src/character.cpp
CMakeFiles/main.out.dir/src/character.cpp.o: CMakeFiles/main.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.out.dir/src/character.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.out.dir/src/character.cpp.o -MF CMakeFiles/main.out.dir/src/character.cpp.o.d -o CMakeFiles/main.out.dir/src/character.cpp.o -c /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/character.cpp

CMakeFiles/main.out.dir/src/character.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.out.dir/src/character.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/character.cpp > CMakeFiles/main.out.dir/src/character.cpp.i

CMakeFiles/main.out.dir/src/character.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.out.dir/src/character.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/character.cpp -o CMakeFiles/main.out.dir/src/character.cpp.s

CMakeFiles/main.out.dir/src/models.cpp.o: CMakeFiles/main.out.dir/flags.make
CMakeFiles/main.out.dir/src/models.cpp.o: ../src/models.cpp
CMakeFiles/main.out.dir/src/models.cpp.o: CMakeFiles/main.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.out.dir/src/models.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.out.dir/src/models.cpp.o -MF CMakeFiles/main.out.dir/src/models.cpp.o.d -o CMakeFiles/main.out.dir/src/models.cpp.o -c /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/models.cpp

CMakeFiles/main.out.dir/src/models.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.out.dir/src/models.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/models.cpp > CMakeFiles/main.out.dir/src/models.cpp.i

CMakeFiles/main.out.dir/src/models.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.out.dir/src/models.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/models.cpp -o CMakeFiles/main.out.dir/src/models.cpp.s

CMakeFiles/main.out.dir/src/model_loader.cpp.o: CMakeFiles/main.out.dir/flags.make
CMakeFiles/main.out.dir/src/model_loader.cpp.o: ../src/model_loader.cpp
CMakeFiles/main.out.dir/src/model_loader.cpp.o: CMakeFiles/main.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/main.out.dir/src/model_loader.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.out.dir/src/model_loader.cpp.o -MF CMakeFiles/main.out.dir/src/model_loader.cpp.o.d -o CMakeFiles/main.out.dir/src/model_loader.cpp.o -c /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/model_loader.cpp

CMakeFiles/main.out.dir/src/model_loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.out.dir/src/model_loader.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/model_loader.cpp > CMakeFiles/main.out.dir/src/model_loader.cpp.i

CMakeFiles/main.out.dir/src/model_loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.out.dir/src/model_loader.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/model_loader.cpp -o CMakeFiles/main.out.dir/src/model_loader.cpp.s

CMakeFiles/main.out.dir/src/loadTGA.cpp.o: CMakeFiles/main.out.dir/flags.make
CMakeFiles/main.out.dir/src/loadTGA.cpp.o: ../src/loadTGA.cpp
CMakeFiles/main.out.dir/src/loadTGA.cpp.o: CMakeFiles/main.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/main.out.dir/src/loadTGA.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.out.dir/src/loadTGA.cpp.o -MF CMakeFiles/main.out.dir/src/loadTGA.cpp.o.d -o CMakeFiles/main.out.dir/src/loadTGA.cpp.o -c /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/loadTGA.cpp

CMakeFiles/main.out.dir/src/loadTGA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.out.dir/src/loadTGA.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/loadTGA.cpp > CMakeFiles/main.out.dir/src/loadTGA.cpp.i

CMakeFiles/main.out.dir/src/loadTGA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.out.dir/src/loadTGA.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /csse/users/kjr106/Desktop/Cosc422/assignment-1/src/loadTGA.cpp -o CMakeFiles/main.out.dir/src/loadTGA.cpp.s

# Object files for target main.out
main_out_OBJECTS = \
"CMakeFiles/main.out.dir/src/main.cpp.o" \
"CMakeFiles/main.out.dir/src/shadow.cpp.o" \
"CMakeFiles/main.out.dir/src/character.cpp.o" \
"CMakeFiles/main.out.dir/src/models.cpp.o" \
"CMakeFiles/main.out.dir/src/model_loader.cpp.o" \
"CMakeFiles/main.out.dir/src/loadTGA.cpp.o"

# External object files for target main.out
main_out_EXTERNAL_OBJECTS =

main.out: CMakeFiles/main.out.dir/src/main.cpp.o
main.out: CMakeFiles/main.out.dir/src/shadow.cpp.o
main.out: CMakeFiles/main.out.dir/src/character.cpp.o
main.out: CMakeFiles/main.out.dir/src/models.cpp.o
main.out: CMakeFiles/main.out.dir/src/model_loader.cpp.o
main.out: CMakeFiles/main.out.dir/src/loadTGA.cpp.o
main.out: CMakeFiles/main.out.dir/build.make
main.out: /usr/lib/x86_64-linux-gnu/libOpenGL.so
main.out: /usr/lib/x86_64-linux-gnu/libGLX.so
main.out: /usr/lib/x86_64-linux-gnu/libGLU.so
main.out: /usr/lib/x86_64-linux-gnu/libglut.so
main.out: /usr/lib/x86_64-linux-gnu/libXi.so
main.out: /usr/lib/x86_64-linux-gnu/libIL.so
main.out: /usr/lib/x86_64-linux-gnu/libassimp.so.5.2.0
main.out: /usr/lib/x86_64-linux-gnu/libz.so
main.out: /usr/lib/x86_64-linux-gnu/libdraco.so.4.0.0
main.out: /usr/lib/x86_64-linux-gnu/librt.a
main.out: CMakeFiles/main.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable main.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.out.dir/build: main.out
.PHONY : CMakeFiles/main.out.dir/build

CMakeFiles/main.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.out.dir/clean

CMakeFiles/main.out.dir/depend:
	cd /csse/users/kjr106/Desktop/Cosc422/assignment-1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /csse/users/kjr106/Desktop/Cosc422/assignment-1 /csse/users/kjr106/Desktop/Cosc422/assignment-1 /csse/users/kjr106/Desktop/Cosc422/assignment-1/build /csse/users/kjr106/Desktop/Cosc422/assignment-1/build /csse/users/kjr106/Desktop/Cosc422/assignment-1/build/CMakeFiles/main.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.out.dir/depend

