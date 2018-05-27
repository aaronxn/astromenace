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
	along with AstroMenace. If not, see <http://www.gnu.org/licenses/>.


	Web Site: http://www.viewizard.com/
	Project: https://github.com/viewizard/astromenace
	E-mail: viewizard@viewizard.com

*************************************************************************************/

// NOTE in future, use std::filesystem::create_directory() instead of mkdir() (since C++17)

#include "../core/base.h"
#include "../build_config.h"

#ifdef __unix
#include <sys/stat.h> // mkdir
#endif // unix


/*
 * Get configuration path for current platform.
 */
const std::string &GetConfigPath()
{
	static std::string ConfigPath{};

	if (!ConfigPath.empty())
		return ConfigPath;

	// by some reason, SDL use XDG_CONFIG_DATA for preferences, so,
	// we are forced to use own code instead of SDL_GetPrefPath() for unix
#ifdef __unix
	// XDG_CONFIG_HOME
	if (const char *tmpEnvCH = std::getenv("XDG_CONFIG_HOME"))
		ConfigPath = tmpEnvCH;
	else {
		if (const char *tmpEnvH = std::getenv("HOME")) {
			ConfigPath = tmpEnvH;
			ConfigPath += "/.config";

			struct stat st;
			if (stat(ConfigPath.c_str(), &st) == -1) {
				mkdir(ConfigPath.c_str(), 0755);
			}
		} else
			return ConfigPath;
	}

	ConfigPath += "/astromenace";

	struct stat st;
	if (stat(ConfigPath.c_str(), &st) == -1) {
		mkdir(ConfigPath.c_str(), 0755);
	}

	ConfigPath += "/";
#else
	if (char *base_path = SDL_GetPrefPath("Viewizard", "AstroMenace")) {
		ConfigPath = base_path;
		SDL_free(base_path);
	}
#endif // unix

	return ConfigPath;
}
