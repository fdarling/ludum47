CC  = gcc
CXX = g++
#CC  = emcc
#CXX = em++
TARGET_RAW = game
ifeq ($(CC),emcc)
EMCC_FLAGS = -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'
CFLAGS = $(EMCC_FLAGS) -O2
LIBS   = $(EMCC_FLAGS) --preload-file assets
TARGET = $(TARGET_RAW).html
else
CFLAGS = `pkg-config --cflags sdl2 SDL2_image SDL2_gfx box2d` -MMD -MP -O2 -g -Wall -Wextra
LIBS   = `pkg-config --libs sdl2 SDL2_image SDL2_gfx box2d`
TARGET = game
endif
SOURCES = \
	src/globals.cpp \
	src/MakeFixture.cpp \
	src/LoadTexture.cpp \
	src/ContactListener.cpp \
	src/DrawPhysics.cpp \
	src/Player.cpp \
	src/Bullet.cpp \
	src/Spring.cpp \
	src/Ladder.cpp \
	src/World.cpp \
	src/GameObject.cpp \
	src/Point.cpp \
	src/Line.cpp \
	src/Camera.cpp \
	src/main.cpp
BUILD_DIR = make_build
OBJECTS += $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(SOURCES))))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CFLAGS)

-include $(OBJECTS:.o=.d)

clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(TARGET_RAW) $(TARGET_RAW).exe $(TARGET_RAW).html $(TARGET_RAW).wasm $(TARGET_RAW).js $(TARGET_RAW).data
	rm -rf ${BUILD_DIR}

.PHONY: all clean
