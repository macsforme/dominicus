PROGRAM = dominicus
GCC = g++
CFLAGS = `freetype-config --cflags` -D GL_GLEXT_PROTOTYPES -g -Wall -Werror
LIBS = -lGL -lSDL `freetype-config --libs`

HEADERS = -I src

SOURCES = \
	./src/platform/linux/Platform.cpp \
	./src/platform/linux/main.cpp \
	./src/logic/Schemes.cpp \
	./src/logic/GameLogic.cpp \
	./src/logic/cameras/FollowCamera.cpp \
	./src/logic/cameras/Camera.cpp \
	./src/logic/cameras/TestCameras.cpp \
	./src/logic/UILayoutAuthority.cpp \
	./src/graphics/3dgraphics/renderers/ShipRenderer.cpp \
	./src/graphics/3dgraphics/renderers/TerrainRenderer.cpp \
	./src/graphics/3dgraphics/renderers/WaterRenderer.cpp \
	./src/graphics/texture/Texture.cpp \
	./src/graphics/2dgraphics/cursor/DrawCursor.cpp \
	./src/graphics/2dgraphics/ui/DrawGrayOut.cpp \
	./src/graphics/2dgraphics/ui/DrawRadar.cpp \
	./src/graphics/2dgraphics/ui/BaseUIElement.cpp \
	./src/graphics/2dgraphics/ui/components/DrawLabel.cpp \
	./src/graphics/2dgraphics/ui/components/DrawButton.cpp \
	./src/graphics/2dgraphics/ui/components/DrawTexture.cpp \
	./src/graphics/2dgraphics/ui/components/DrawField.cpp \
	./src/graphics/2dgraphics/ui/components/DrawContainer.cpp \
	./src/graphics/2dgraphics/controlbox/DrawControlBox.cpp \
	./src/graphics/2dgraphics/splash/DrawSplash.cpp \
	./src/graphics/GameGraphics.cpp \
	./src/graphics/BaseDrawNode.cpp \
	./src/graphics/text/FontManager.cpp \
	./src/graphics/text/TextBlock.cpp \
	./src/input/Keyboard.cpp \
	./src/input/Mouse.cpp \
	./src/input/InputHandler.cpp \
	./src/geometry/DiamondSquare.cpp \
	./src/state/Collisions.cpp \
	./src/state/Ship.cpp \
	./src/state/World.cpp \
	./src/state/GameState.cpp \
	./src/core/GameSystem.cpp \
	./src/core/dominicusMain.cpp \
	./src/core/MainLoopMember.cpp

all: $(PROGRAM)

$(PROGRAM): $(SOURCES)
	$(GCC) $(CFLAGS) $(LIBS) $(HEADERS) $(SOURCES) -o $(PROGRAM)
