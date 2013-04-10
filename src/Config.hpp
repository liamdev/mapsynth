#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

//Default file paths.
const std::string RES_DIR = "res/";
const std::string OBJ_OUTPUT_PATH = "output.obj";
const std::string SVG_OUTPUT_PATH = "output.svg";
const std::string DETAILS_OUTPUT_PATH = "mapeval.txt";

//Default tile values.
const float DEFAULT_WINDOW_SIZE = 120.0f;
const float DEFAULT_OFFSET = 8.0f;

//Default map values.
const unsigned int DEFAULT_OUTPUT_WIDTH = 8;
const unsigned int DEFAULT_OUTPUT_HEIGHT = 8;

//Map modification values.
const float MAX_INTERSECTION_MERGE_RADIUS = 10.0f;
const float JUNCTION_EQ_DISTANCE = 5.0f;

//Stroke width for SVG grid output.
const float DEFAULT_GRID_LINE_WIDTH = 1.0f;

#endif
