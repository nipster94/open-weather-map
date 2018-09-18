# OpenWeatherMap

**LAYOUT:**
- OpenWeatherMap 
  - forms/:                 UI file
  - src/:                   Source files
  - includes/:              Header files
  - lists/:                 API Data captured in the process	
  - CMakeLists.txt:         CMake project configuration file
- README.txt:             readme file (this file)

**USAGE:**
This package is design to communicate with the **OpenWeatherMap**. This application will also support an **UI** which will collect data from the user and display/save grabed data. 

This application will grab data from the floowing APIs,
   1) \ref<http://openweathermap.org/current>         ---> Results will be displayed in the UI
   2) \ref<http://openweathermap.org/forecast5>       ---> Results will be written in to a excle file
   3) \ref<http://openweathermap.org/api/pollution/co>---> Results will be written in to a excle file

**HOW TO RUN THIS PACKAGE**
1. Clone the pacakge from the repository
   - git clone https://bitbucket.org/Nipun_cg/openweathermap/
2. Run the following commands.
   - cd ~/OpenWeatherMap/
   - mkdir build
   - cd build
   - cmake ..
   - make
   - ./OpenWeatherMap

**REFERENCE**
1. QT Documents (QNetwork and QMainWindow classes)
   - \ref <http://doc.qt.io/archives/qt-4.8/qtnetwork-module.html>
2. OpenWeatherMap server
   - \ref<http://openweathermap.org/current>
   - \ref<http://openweathermap.org/forecast5>
   - \ref<http://openweathermap.org/api/pollution/co>
3. Doxygen (For documentation)
   - \ref<https://www.stack.nl/~dimitri/doxygen/manual/index.html>
   

