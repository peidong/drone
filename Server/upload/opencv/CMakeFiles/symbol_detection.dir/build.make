# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.4.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.4.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yangyang/Github/drone/Server/upload/opencv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yangyang/Github/drone/Server/upload/opencv

# Include any dependencies generated for this target.
include CMakeFiles/symbol_detection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/symbol_detection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/symbol_detection.dir/flags.make

CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o: CMakeFiles/symbol_detection.dir/flags.make
CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o: symbol_detection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyang/Github/drone/Server/upload/opencv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o -c /Users/yangyang/Github/drone/Server/upload/opencv/symbol_detection.cpp

CMakeFiles/symbol_detection.dir/symbol_detection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/symbol_detection.dir/symbol_detection.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yangyang/Github/drone/Server/upload/opencv/symbol_detection.cpp > CMakeFiles/symbol_detection.dir/symbol_detection.cpp.i

CMakeFiles/symbol_detection.dir/symbol_detection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/symbol_detection.dir/symbol_detection.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yangyang/Github/drone/Server/upload/opencv/symbol_detection.cpp -o CMakeFiles/symbol_detection.dir/symbol_detection.cpp.s

CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.requires:

.PHONY : CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.requires

CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.provides: CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.requires
	$(MAKE) -f CMakeFiles/symbol_detection.dir/build.make CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.provides.build
.PHONY : CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.provides

CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.provides.build: CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o


# Object files for target symbol_detection
symbol_detection_OBJECTS = \
"CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o"

# External object files for target symbol_detection
symbol_detection_EXTERNAL_OBJECTS =

symbol_detection: CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o
symbol_detection: CMakeFiles/symbol_detection.dir/build.make
symbol_detection: /usr/local/lib/libopencv_xphoto.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_xobjdetect.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_tracking.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_surface_matching.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_structured_light.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_stereo.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_saliency.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_rgbd.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_reg.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_optflow.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_line_descriptor.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_dpm.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_dnn.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_datasets.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_ccalib.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_bioinspired.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_bgsegm.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_aruco.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_videostab.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_superres.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_stitching.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_photo.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_hal.a
symbol_detection: /usr/local/lib/libopencv_text.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_face.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_ximgproc.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_xfeatures2d.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_shape.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_video.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_objdetect.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_calib3d.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_features2d.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_ml.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_highgui.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_videoio.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_imgcodecs.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_imgproc.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_flann.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_core.3.0.0.dylib
symbol_detection: /usr/local/lib/libopencv_hal.a
symbol_detection: /usr/local/share/OpenCV/3rdparty/lib/libippicv.a
symbol_detection: CMakeFiles/symbol_detection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yangyang/Github/drone/Server/upload/opencv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable symbol_detection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/symbol_detection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/symbol_detection.dir/build: symbol_detection

.PHONY : CMakeFiles/symbol_detection.dir/build

CMakeFiles/symbol_detection.dir/requires: CMakeFiles/symbol_detection.dir/symbol_detection.cpp.o.requires

.PHONY : CMakeFiles/symbol_detection.dir/requires

CMakeFiles/symbol_detection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/symbol_detection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/symbol_detection.dir/clean

CMakeFiles/symbol_detection.dir/depend:
	cd /Users/yangyang/Github/drone/Server/upload/opencv && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yangyang/Github/drone/Server/upload/opencv /Users/yangyang/Github/drone/Server/upload/opencv /Users/yangyang/Github/drone/Server/upload/opencv /Users/yangyang/Github/drone/Server/upload/opencv /Users/yangyang/Github/drone/Server/upload/opencv/CMakeFiles/symbol_detection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/symbol_detection.dir/depend

