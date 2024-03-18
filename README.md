# YAMP
YAMP is a cross-platform music player written in C++

<img src="YAMP.png">

# Features
- Supports most audio formats
- Playlists
- Themes
- Unicode support

# Planned Features
- Lyrics

# Building

#### Clone repository
```
git clone https://github.com/mb4c/YAMP
``` 

#### Linux
[Install vcpkg](https://vcpkg.io/en/getting-started)
```
sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel mesa-libGL-devel gtk3-devel curl-devel gstreamer1-devel gstreamer1-plugins-base-tools gstreamer1-doc gstreamer1-plugins-base-devel gstreamer1-plugins-good gstreamer1-plugins-good-extras gstreamer1-plugins-ugly gstreamer1-plugins-bad-free gstreamer1-plugins-bad-free-devel gstreamer1-plugins-bad-free-extras
mkdir build
cmake -B build/ -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake 
cd build
make
```


# License

This project is licensed under the GPLv3 License. See the LICENSE file for details.