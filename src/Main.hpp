/****************************************************************************
File name: Main.hpp

Description: Program entry point. Runs the basic operation: read city files, synthesise maps, export.

Author:		Liam de Valmency
Date:		24th April 2013

This source code is licensed under the MIT license.
See LICENSE.txt for more information.
****************************************************************************/


#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdlib>
#include <iostream>

#include <boost/foreach.hpp>
#define forEach BOOST_FOREACH

#include "City.hpp"
#include "Config.hpp"
#include "FileTools.hpp"
#include "PatchMap.hpp"

int main(int argc, char* argv[]);

#endif