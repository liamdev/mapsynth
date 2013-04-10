# Example makefile for compilation

CC=g++
CFLAGS=-Wall -std=c++11
OBJS=Building.o City.o FileOperations.o FileTools.o Main.o MapFragment.o MapLUT.o MathTools.o ModelExporter.o Patch.o PatchMap.o Path.o RoadGraph.o SVGExporter.o XMLTools.o
LIBS=-lboost_filesystem
OBJ_DIR=bin/
OUT_OBJS=$(addprefix $(OBJ_DIR), $(OBJS))
DEFS=
INC=#-I/home/liam/Dropbox/Documents/c++/
OUT=mapsynth

$(OUT): $(OUT_OBJS)
	$(CC) $(CFLAGS) $(OUT_OBJS) $(LIBS) $(DEFS) -o $(OUT)

$(OBJ_DIR)%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@

clean:
	-rm $(OUT_OBJS)
