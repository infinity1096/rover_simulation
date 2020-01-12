cmake_minimum_required(VERSION 2.8.3)

project(rover)
set( CMAKE_CXX_FLAGS "-std=c++11" )
set(OpenCV_DIR /usr/share/OpenCV/)
find_package(catkin REQUIRED COMPONENTS 
  OpenCV REQUIRED
  roscpp
  std_msgs
  geometry_msgs
  nav_msgs
  sensor_msgs
  cv_bridge
  tf
  image_transport
  )

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)





foreach(dir config launch meshes urdf)
	install(DIRECTORY ${dir}/
		DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

add_executable(rover_base_controller src/rover_base_controller.cpp)
target_link_libraries(rover_base_controller ${catkin_LIBRARIES})
add_executable(rover_odom_publisher src/rover_odom_publisher.cpp)
target_link_libraries(rover_odom_publisher ${catkin_LIBRARIES})
add_executable(rover_video_saver src/rover_image_saver.cpp)
target_link_libraries(rover_video_saver ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})


