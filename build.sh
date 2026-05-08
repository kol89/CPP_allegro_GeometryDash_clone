g++ src/MyGame.cpp src/include/engine.cpp src/include/colour.cpp src/include/rectangle.cpp src/include/rotateble_rectangle.cpp src/include/level.cpp src/include/block.cpp src/include/spike.cpp src/include/portal.cpp src/include/speed_portal.cpp src/include/orb.cpp src/include/pad.cpp src/include/button.cpp src/include/player.cpp src/include/load_lvl.cpp src/include/json11.cpp \
-o build/linux/program \
-std=c++17 \
$(pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5)
build/linux/program
