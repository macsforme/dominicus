PROGRAM = dominicus
GCC = g++
CFLAGS = `freetype-config --cflags` -D GL_GLEXT_PROTOTYPES -g -Wall -Werror
LIBS = -lGL -lSDL `freetype-config --libs`

HEADERS = -I src

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
	./src/core/MainLoopMember.cpp \
	./src/windowing/GameWindow.cpp \
	./src/drawing/DrawingMaster.cpp \
	./src/drawing/rendering/RenderingMaster.cpp \
	./src/drawing/rendering/cameras/WorldViewCamera.cpp \
	./src/drawing/rendering/cameras/Camera.cpp \
	./src/drawing/rendering/cameras/FirstPersonCamera.cpp \
	./src/drawing/rendering/cameras/BackCamera.cpp \
	./src/drawing/rendering/terrain/TerrainRenderer.cpp \
	./src/drawing/rendering/ship/ShipRenderer.cpp \
	./src/drawing/rendering/sky/SkyRenderer.cpp \
	./src/drawing/text/FontManager.cpp \
	./src/drawing/text/TextBlock.cpp \
	./src/drawing/texture/Texture.cpp \
	./src/drawing/texture/TextureUtils.cpp \
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
