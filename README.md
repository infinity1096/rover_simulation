# rover_simulation

## Installiation instructions: 

1. Follow the ROS tutorial and install ROS melodic http://wiki.ros.org/melodic/Installation. 
  *Be sure to install the "ros-melodic-desktop-full" version, since gazebo is needed for simulation
  
2. Now you should have created "\~/catkin_ws/". Move the "rover" folder to "\~/catkin_ws/src/".

```
  cd <rover_simulation_folder>
  cp -r rover ~/catkin_ws/src/ 
```

3. Source the setup.sh file to add the project folder to ros.
```
  source ~/catkin_ws/devel/setup.sh
```

4. Run gazebo once by either calling "gazebo" in command line or the desktop icon. you should have the folder "~/.gazebo/models"
```
  gazebo
```

5. Move the folder "desset5" to "~/.gazebo/models/"
```
  cp -r desset5 ~/.gazebo/models/
```
6. Install dependencies for gazebo to communicate with ROS
```
  sudo apt install ros-melodic-gazebo-ros-control ros-melodic-effort-controllers
```
7. Run the simulation demo, control rover movement by arrow keys.
```
  roslaunch rover start.launch
```

## Sensors:
### 1. Depth camera
  topic:
  ```
  /rover/body_camera/
  raw image: /rover/body_camera/color/image_raw
  point cloud: /rover/body_camera/depth/points
  ```
### 2. Ground camera
  topic:
  ```
  /rover/ground_camera
  raw image: /rover/ground_camera/image_raw
  ```
### 3. imu
  topic:
  ```
  /rover/imu
  ```
### 4. GPS
  topic:
  ```
  /rover/gps
  position: /rover/gps/position
  velocity: /rover/gps/velocity
  ```
    
