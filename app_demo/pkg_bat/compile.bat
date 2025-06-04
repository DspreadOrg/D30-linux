@echo off  

cd ..
cd build
make

cd ..
cd release\bin

arm-none-linux-gnueabihf-strip.exe LVGL_UI  