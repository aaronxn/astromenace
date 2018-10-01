/************************************************************************************

	AstroMenace
	Hardcore 3D space scroll-shooter with spaceship upgrade possibilities.
	Copyright (c) 2006-2018 Mikhail Kurinnoi, Viewizard


	AstroMenace is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AstroMenace is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with AstroMenace. If not, see <https://www.gnu.org/licenses/>.


	Website: https://viewizard.com/
	Project: https://github.com/viewizard/astromenace
	E-mail: viewizard@viewizard.com

*************************************************************************************/

// TODO translate comments

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

// NOTE switch to nested namespace definition (namespace A::B::C { ... }) (since C++17)
namespace viewizard {
namespace astromenace {

//
void GameCameraUpdate(float Time);
//
void InitGameCamera();
//
void ResetGameCamera();
//
float GameCameraGetDeviation();
//
float GameCameraGetSpeed();
//
void GameCameraSetExplosion(const sVECTOR3D &Location, float Power);
// Get camera covered distance.
const sVECTOR3D &GetCameraCoveredDistance();

} // astromenace namespace
} // viewizard namespace

#endif // GAME_CAMERA_H
