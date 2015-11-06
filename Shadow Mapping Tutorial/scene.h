//////////////////////////////////////////////////////////////////////////////////////////
//	scene.h
//	Draw scene for shadow mapping
//	Downloaded from: www.paulsprojects.net
//	Created:	16th September 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SCENE_H
#define SCENE_H

#include "ParserObj.h"

void DrawScene(float angle, bool drawHairball);
void DrawModel(ParserObj model);

#endif	//SCENE_H