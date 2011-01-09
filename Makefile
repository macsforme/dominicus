PROGRAM = dominicus
GCC = g++
CFLAGS = `freetype-config --cflags` -D GL_GLEXT_PROTOTYPES
LIBS = -lGL -lSDL `freetype-config --libs`

HEADERS = \
	-I src/core \
	-I  src/drawing \
	-I  src/drawing/cursor \
	-I  src/drawing/HUD \
	-I  src/drawing/rendering \
	-I  src/drawing/HUD/console \
	-I  src/drawing/controlbox \
	-I  src/drawing/HUD/infobox \
	-I  src/drawing/rendering/cameras \
	-I  src/drawing/rendering/ship \
	-I  src/drawing/rendering/terrain \
	-I  src/drawing/text \
	-I  src/drawing/texture \
	-I  src/geometry \
	-I  src/input \
	-I  src/math \
	-I  src/platform \
	-I  src/platform/linux \
	-I  src/state \
	-I  src/UI \
	-I  src/windowing

SOURCES = \
	./src/input/InputHandler.cpp \
	./src/input/Mouse.cpp \
	./src/input/KeyTrap.cpp \
	./src/input/Keyboard.cpp \
	./src/UI/Console.cpp \
	./src/state/Terrain.cpp \
	./src/state/Ship.cpp \
	./src/state/ShipControl.cpp \
	./src/platform/linux/Platform.cpp \
	./src/platform/linux/main.cpp \
	./src/core/GamePrefs.cpp \
	./src/core/ProgramLog.cpp \
	./src/core/dominicusMain.cpp \
	./src/core/SystemInfo.cpp \
	./src/core/BuildVersion.cpp \
	./src/windowing/GameWindow.cpp \
	./src/drawing/DrawingMaster.cpp \
	./src/drawing/rendering/RenderingMaster.cpp \
	./src/drawing/rendering/cameras/WorldViewCamera.cpp \
	./src/drawing/rendering/cameras/Camera.cpp \
	./src/drawing/rendering/cameras/FirstPersonCamera.cpp \
	./src/drawing/rendering/cameras/BackCamera.cpp \
	./src/drawing/rendering/terrain/TerrainRenderer.cpp \
	./src/drawing/rendering/ship/ShipRenderer.cpp \
	./src/drawing/text/FontManager.cpp \
	./src/drawing/text/TextBlock.cpp \
	./src/drawing/texture/Texture.cpp \
	./src/drawing/texture/BMPImage.cpp \
	./src/drawing/HUD/DrawHUDContainerUtility.cpp \
	./src/drawing/HUD/DrawHUD.cpp \
	./src/drawing/HUD/infobox/DrawInfoBox.cpp \
	./src/drawing/HUD/HUDArrangement.cpp \
	./src/drawing/HUD/console/DrawConsole.cpp \
	./src/drawing/HUD/DrawHUDEnclosure.cpp \
	./src/drawing/cursor/DrawCursor.cpp \
	./src/drawing/controlbox/DrawControlBox.cpp \
	./src/drawing/ShaderTools.cpp \
	./src/geometry/DiamondSquare.cpp

all: $(PROGRAM)

$(PROGRAM): $(SOURCES)
	$(GCC) $(CFLAGS) $(LIBS) $(HEADERS) $(SOURCES) -o $(PROGRAM)
