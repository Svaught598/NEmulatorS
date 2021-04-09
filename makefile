
EXE  = NES
NES_SRCS =   	\
	6502.cpp 	\
	bus.cpp 	\
	main.cpp 	\
	system.cpp 	\
	gui.cpp 	\
	cart.cpp	\
	mappers.cpp	\
	ppu.cpp
NES_OBJS = $(addsuffix .o, $(basename $(notdir $(NES_SRCS))))

UNAME_S := $(shell uname -s)

.DEFAULT_GOAL := help
CXXFLAGS = -I../ -I../../
CXXFLAGS += -g -Wall -Wformat -lm -lstdc++ -Wshadow -lpthread -std=c++17
LIBS = 

##############################################
# General Usage ##############################
##############################################

help: ## displays this message!
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'


build: ## builds the project!
	@echo "Building project..."
	$(MAKE) main
	@echo "All done!"


valgrind: ## builds the project with compiler flags & runs valgrind
	$(MAKE) main
	valgrind --leak-check=full --gen-suppressions=all --show-reachable=yes --log-file="val.log" -q -s ./NES -t


clean: ## cleans up the intermediate files
	@echo "Cleaning up..."
	rm -f $(EXE) $(NES_OBJS) 
	@echo "All done!"

##############################################
# Imgui Stuff ################################
##############################################

IMG_SRCS = \
	./Imgui/imgui_impl_glfw.cpp \
	./Imgui/imgui_impl_opengl3.cpp \
	./Imgui/imgui.cpp \
	./Imgui/imgui_demo.cpp \
	./Imgui/imgui_draw.cpp \
	./Imgui/imgui_widgets.cpp
IMG_OBJS = $(addsuffix .o, $(basename $(notdir $(IMG_SRCS))))


# Using OpenGL loader: glew
# (This assumes a system-wide installation)
 CXXFLAGS += -DIMGUI_IMPL_OPENGL_LOADER_GLEW
 LIBS += -lGLEW

 ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##############################################
# Build Rules ################################
##############################################


%.o: ./Imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


%.o: ./src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


main: $(EXE)
	@echo Build complete for NES!


$(EXE): $(NES_OBJS) $(IMG_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
