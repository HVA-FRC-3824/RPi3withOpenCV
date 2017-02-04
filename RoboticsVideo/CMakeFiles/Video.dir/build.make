# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/RPi3withOpenCV/RoboticsVideo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/RPi3withOpenCV/RoboticsVideo

# Include any dependencies generated for this target.
include CMakeFiles/Video.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Video.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Video.dir/flags.make

CMakeFiles/Video.dir/Video.cpp.o: CMakeFiles/Video.dir/flags.make
CMakeFiles/Video.dir/Video.cpp.o: Video.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/RPi3withOpenCV/RoboticsVideo/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Video.dir/Video.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Video.dir/Video.cpp.o -c /home/pi/RPi3withOpenCV/RoboticsVideo/Video.cpp

CMakeFiles/Video.dir/Video.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Video.dir/Video.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/RPi3withOpenCV/RoboticsVideo/Video.cpp > CMakeFiles/Video.dir/Video.cpp.i

CMakeFiles/Video.dir/Video.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Video.dir/Video.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/RPi3withOpenCV/RoboticsVideo/Video.cpp -o CMakeFiles/Video.dir/Video.cpp.s

CMakeFiles/Video.dir/Video.cpp.o.requires:
.PHONY : CMakeFiles/Video.dir/Video.cpp.o.requires

CMakeFiles/Video.dir/Video.cpp.o.provides: CMakeFiles/Video.dir/Video.cpp.o.requires
	$(MAKE) -f CMakeFiles/Video.dir/build.make CMakeFiles/Video.dir/Video.cpp.o.provides.build
.PHONY : CMakeFiles/Video.dir/Video.cpp.o.provides

CMakeFiles/Video.dir/Video.cpp.o.provides.build: CMakeFiles/Video.dir/Video.cpp.o

# Object files for target Video
Video_OBJECTS = \
"CMakeFiles/Video.dir/Video.cpp.o"

# External object files for target Video
Video_EXTERNAL_OBJECTS =

Video: CMakeFiles/Video.dir/Video.cpp.o
Video: CMakeFiles/Video.dir/build.make
Video: /usr/local/lib/libopencv_xphoto.so.3.1.0
Video: /usr/local/lib/libopencv_xobjdetect.so.3.1.0
Video: /usr/local/lib/libopencv_ximgproc.so.3.1.0
Video: /usr/local/lib/libopencv_xfeatures2d.so.3.1.0
Video: /usr/local/lib/libopencv_tracking.so.3.1.0
Video: /usr/local/lib/libopencv_text.so.3.1.0
Video: /usr/local/lib/libopencv_surface_matching.so.3.1.0
Video: /usr/local/lib/libopencv_structured_light.so.3.1.0
Video: /usr/local/lib/libopencv_stereo.so.3.1.0
Video: /usr/local/lib/libopencv_saliency.so.3.1.0
Video: /usr/local/lib/libopencv_rgbd.so.3.1.0
Video: /usr/local/lib/libopencv_reg.so.3.1.0
Video: /usr/local/lib/libopencv_plot.so.3.1.0
Video: /usr/local/lib/libopencv_optflow.so.3.1.0
Video: /usr/local/lib/libopencv_line_descriptor.so.3.1.0
Video: /usr/local/lib/libopencv_fuzzy.so.3.1.0
Video: /usr/local/lib/libopencv_face.so.3.1.0
Video: /usr/local/lib/libopencv_dpm.so.3.1.0
Video: /usr/local/lib/libopencv_dnn.so.3.1.0
Video: /usr/local/lib/libopencv_datasets.so.3.1.0
Video: /usr/local/lib/libopencv_ccalib.so.3.1.0
Video: /usr/local/lib/libopencv_bioinspired.so.3.1.0
Video: /usr/local/lib/libopencv_bgsegm.so.3.1.0
Video: /usr/local/lib/libopencv_aruco.so.3.1.0
Video: /usr/local/lib/libopencv_videostab.so.3.1.0
Video: /usr/local/lib/libopencv_videoio.so.3.1.0
Video: /usr/local/lib/libopencv_video.so.3.1.0
Video: /usr/local/lib/libopencv_superres.so.3.1.0
Video: /usr/local/lib/libopencv_stitching.so.3.1.0
Video: /usr/local/lib/libopencv_shape.so.3.1.0
Video: /usr/local/lib/libopencv_photo.so.3.1.0
Video: /usr/local/lib/libopencv_objdetect.so.3.1.0
Video: /usr/local/lib/libopencv_ml.so.3.1.0
Video: /usr/local/lib/libopencv_imgproc.so.3.1.0
Video: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
Video: /usr/local/lib/libopencv_highgui.so.3.1.0
Video: /usr/local/lib/libopencv_flann.so.3.1.0
Video: /usr/local/lib/libopencv_features2d.so.3.1.0
Video: /usr/local/lib/libopencv_core.so.3.1.0
Video: /usr/local/lib/libopencv_calib3d.so.3.1.0
Video: /usr/local/lib/libopencv_text.so.3.1.0
Video: /usr/local/lib/libopencv_face.so.3.1.0
Video: /usr/local/lib/libopencv_ximgproc.so.3.1.0
Video: /usr/local/lib/libopencv_xfeatures2d.so.3.1.0
Video: /usr/local/lib/libopencv_shape.so.3.1.0
Video: /usr/local/lib/libopencv_video.so.3.1.0
Video: /usr/local/lib/libopencv_objdetect.so.3.1.0
Video: /usr/local/lib/libopencv_calib3d.so.3.1.0
Video: /usr/local/lib/libopencv_features2d.so.3.1.0
Video: /usr/local/lib/libopencv_ml.so.3.1.0
Video: /usr/local/lib/libopencv_highgui.so.3.1.0
Video: /usr/local/lib/libopencv_videoio.so.3.1.0
Video: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
Video: /usr/local/lib/libopencv_imgproc.so.3.1.0
Video: /usr/local/lib/libopencv_flann.so.3.1.0
Video: /usr/local/lib/libopencv_core.so.3.1.0
Video: CMakeFiles/Video.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Video"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Video.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Video.dir/build: Video
.PHONY : CMakeFiles/Video.dir/build

CMakeFiles/Video.dir/requires: CMakeFiles/Video.dir/Video.cpp.o.requires
.PHONY : CMakeFiles/Video.dir/requires

CMakeFiles/Video.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Video.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Video.dir/clean

CMakeFiles/Video.dir/depend:
	cd /home/pi/RPi3withOpenCV/RoboticsVideo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/RPi3withOpenCV/RoboticsVideo /home/pi/RPi3withOpenCV/RoboticsVideo /home/pi/RPi3withOpenCV/RoboticsVideo /home/pi/RPi3withOpenCV/RoboticsVideo /home/pi/RPi3withOpenCV/RoboticsVideo/CMakeFiles/Video.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Video.dir/depend

