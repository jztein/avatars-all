CC=g++
CXXFLAGS = -O2 -g -Wall -Wextra

OPENCV_INC_DIR=/usr/local/Cellar/opencv/2.4.2/include
OPENCV_LIB_DIR=/usr/local/Cellar/opencv/2.4.2/lib

CPPFLAGS=-I$(OPENCV_INC_DIR) -I/usr/local/include
LDFLAGS=-L$(OPENCV_LIB_DIR) -L/usr/local/lib

#LDLIBS =  -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video \
#          -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect \
#          -lopencv_contrib -lopencv_legacy -lopencv_gpu
#
LDLIBS =  -lopencv_core -lopencv_imgproc -lopencv_features2d -lopencv_highgui

OBJ=hairDrawer.o

EXECUTABLE=hairDrawer

CLEAN=clean

.PHONY: all
all: $(EXECUTABLE)
$(TARGET):  $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm $(OBJ) $(EXECUTABLE)