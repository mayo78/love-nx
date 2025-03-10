/**
 * Copyright (c) 2006-2019 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

// LOVE
#include "common/config.h"
#include "common/version.h"
#include "common/deprecation.h"
#include "common/runtime.h"

#include "love.h"

// C++
#include <string>
#include <sstream>

#ifdef LOVE_WINDOWS
#include <windows.h>
#endif // LOVE_WINDOWS

#ifdef LOVE_ANDROID
#include <SDL.h>
extern "C"
{
#include "luajit.h"
}
#endif // LOVE_ANDROID

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

#ifdef LOVE_LEGENDARY_ACCELEROMETER_AS_JOYSTICK_HACK
#include <SDL_hints.h>
#endif // LOVE_LEGENDARY_ACCELEROMETER_AS_JOYSTICK_HACK

// Libraries.
#ifdef LOVE_ENABLE_LUASOCKET
#	include "libraries/luasocket/luasocket.h"
#endif
#ifdef LOVE_ENABLE_ENET
#	include "libraries/enet/lua-enet.h"
#endif
#ifdef LOVE_ENABLE_LUA53
#	include "libraries/lua53/lutf8lib.h"
#endif

// For love::graphics::setGammaCorrect.
#ifdef LOVE_ENABLE_GRAPHICS
#	include "graphics/Graphics.h"
#endif

// For love::audio::Audio::setMixWithSystem.
#ifdef LOVE_ENABLE_AUDIO
#	include "audio/Audio.h"
#endif

// Scripts
#include "scripts/nogame.lua.h"
#include "scripts/boot.lua.h"

// All modules define a c-accessible luaopen
// so let's make use of those, instead
// of addressing implementations directly.
extern "C"
{
#if defined(LOVE_ENABLE_AUDIO)
	extern int luaopen_love_audio(lua_State*);
#endif
#if defined(LOVE_ENABLE_DATA)
	extern int luaopen_love_data(lua_State*);
#endif
#if defined(LOVE_ENABLE_EVENT)
	extern int luaopen_love_event(lua_State*);
#endif
#if defined(LOVE_ENABLE_FILESYSTEM)
	extern int luaopen_love_filesystem(lua_State*);
#endif
#if defined(LOVE_ENABLE_FONT)
	extern int luaopen_love_font(lua_State*);
#endif
#if defined(LOVE_ENABLE_GRAPHICS)
	extern int luaopen_love_graphics(lua_State*);
#endif
#if defined(LOVE_ENABLE_IMAGE)
	extern int luaopen_love_image(lua_State*);
#endif
#if defined(LOVE_ENABLE_JOYSTICK)
	extern int luaopen_love_joystick(lua_State*);
#endif
#if defined(LOVE_ENABLE_KEYBOARD)
	extern int luaopen_love_keyboard(lua_State*);
#endif
#if defined(LOVE_ENABLE_MATH)
	extern int luaopen_love_math(lua_State*);
#endif
#if defined(LOVE_ENABLE_MOUSE)
	extern int luaopen_love_mouse(lua_State*);
#endif
#if defined(LOVE_ENABLE_PHYSICS)
	extern int luaopen_love_physics(lua_State*);
#endif
#if defined(LOVE_ENABLE_SOUND)
	extern int luaopen_love_sound(lua_State*);
#endif
#if defined(LOVE_ENABLE_SYSTEM)
	extern int luaopen_love_system(lua_State*);
#endif
#if defined(LOVE_ENABLE_TIMER)
	extern int luaopen_love_timer(lua_State*);
#endif
#if defined(LOVE_ENABLE_THREAD)
	extern int luaopen_love_thread(lua_State*);
#endif
#if defined(LOVE_ENABLE_TOUCH)
	extern int luaopen_love_touch(lua_State*);
#endif
#if defined(LOVE_ENABLE_VIDEO)
	extern int luaopen_love_video(lua_State*);
#endif
#if defined(LOVE_ENABLE_WINDOW)
	extern int luaopen_love_window(lua_State*);
#endif
	extern int luaopen_love_nogame(lua_State*);
	extern int luaopen_love_boot(lua_State*);
}

static const luaL_Reg modules[] = {
#if defined(LOVE_ENABLE_AUDIO)
	{ "love.audio", luaopen_love_audio },
#endif
#if defined(LOVE_ENABLE_DATA)
	{ "love.data", luaopen_love_data },
#endif
#if defined(LOVE_ENABLE_EVENT)
	{ "love.event", luaopen_love_event },
#endif
#if defined(LOVE_ENABLE_FILESYSTEM)
	{ "love.filesystem", luaopen_love_filesystem },
#endif
#if defined(LOVE_ENABLE_FONT)
	{ "love.font", luaopen_love_font },
#endif
#if defined(LOVE_ENABLE_GRAPHICS)
	{ "love.graphics", luaopen_love_graphics },
#endif
#if defined(LOVE_ENABLE_IMAGE)
	{ "love.image", luaopen_love_image },
#endif
#if defined(LOVE_ENABLE_JOYSTICK)
	{ "love.joystick", luaopen_love_joystick },
#endif
#if defined(LOVE_ENABLE_KEYBOARD)
	{ "love.keyboard", luaopen_love_keyboard },
#endif
#if defined(LOVE_ENABLE_MATH)
	{ "love.math", luaopen_love_math },
#endif
#if defined(LOVE_ENABLE_MOUSE)
	{ "love.mouse", luaopen_love_mouse },
#endif
#if defined(LOVE_ENABLE_PHYSICS)
	{ "love.physics", luaopen_love_physics },
#endif
#if defined(LOVE_ENABLE_SOUND)
	{ "love.sound", luaopen_love_sound },
#endif
#if defined(LOVE_ENABLE_SYSTEM)
	{ "love.system", luaopen_love_system },
#endif
#if defined(LOVE_ENABLE_THREAD)
	{ "love.thread", luaopen_love_thread },
#endif
#if defined(LOVE_ENABLE_TIMER)
	{ "love.timer", luaopen_love_timer },
#endif
#if defined(LOVE_ENABLE_TOUCH)
	{ "love.touch", luaopen_love_touch },
#endif
#if defined(LOVE_ENABLE_VIDEO)
	{ "love.video", luaopen_love_video },
#endif
#if defined(LOVE_ENABLE_WINDOW)
	{ "love.window", luaopen_love_window },
#endif
	{ "love.nogame", luaopen_love_nogame },
	{ "love.boot", luaopen_love_boot },
	{ 0, 0 }
};

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
int w__openConsole(lua_State *L);
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

#ifdef LOVE_LEGENDARY_ACCELEROMETER_AS_JOYSTICK_HACK
int w__setAccelerometerAsJoystick(lua_State *L);
#endif

#ifdef LOVE_ANDROID
static int w_print_sdl_log(lua_State *L)
{
	int nargs = lua_gettop(L);

	lua_getglobal(L, "tostring");

	std::string outstring;

	for (int i = 1; i <= nargs; i++)
	{
		// Call tostring(arg) and leave the result on the top of the stack.
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);

		const char *s = lua_tostring(L, -1);
		if (s == nullptr)
			return luaL_error(L, "'tostring' must return a string to 'print'");

		if (i > 1)
			outstring += "\t";

		outstring += s;

		lua_pop(L, 1); // Pop the result of tostring(arg).
	}

	SDL_Log("[LOVE] %s", outstring.c_str());
	return 0;
}
#endif

const char *love_version()
{
	// Do not refer to love::VERSION here, the linker
	// will patch it back up to the executable's one..
	return LOVE_VERSION_STRING;
}

const char *love_codename()
{
	return love::VERSION_CODENAME;
}

static int w_love_getVersion(lua_State *L)
{
	lua_pushinteger(L, love::VERSION_MAJOR);
	lua_pushinteger(L, love::VERSION_MINOR);
	lua_pushinteger(L, love::VERSION_REV);
	lua_pushstring(L, love::VERSION_CODENAME);
	//lua_pushstring(L, "lol! wrong");
	return 4; // 5;
}

//static int testFunc(lua_State *L) {
//	lua_pushstring(L, "Success!");
//	return 1;
//}

static int w_love_isVersionCompatible(lua_State *L)
{
	std::string version;

	if (lua_type(L, 1) == LUA_TSTRING)
	{
		version = luaL_checkstring(L, 1);

		// Convert major.minor to major.minor.revision.
		if (std::count(version.begin(), version.end(), '.') < 2)
			version.append(".0");
	}
	else
	{
		int major = (int) luaL_checkinteger(L, 1);
		int minor = (int) luaL_checkinteger(L, 2);
		int rev   = (int) luaL_optinteger(L, 3, 0);

		// Convert the numbers to a string, since VERSION_COMPATIBILITY is an
		// array of version strings.
		std::stringstream ss;
		ss << major << "." << minor << "." << rev;

		version = ss.str();
	}

	for (int i = 0; love::VERSION_COMPATIBILITY[i] != nullptr; i++)
	{
		std::string v(love::VERSION_COMPATIBILITY[i]);

		// Convert major.minor to major.minor.revision.
		if (std::count(v.begin(), v.end(), '.') < 2)
			v.append(".0");

		if (version.compare(v) != 0)
			continue;

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushboolean(L, false);
	return 1;
}

static int w__setGammaCorrect(lua_State *L)
{
#ifdef LOVE_ENABLE_GRAPHICS
	love::graphics::setGammaCorrect((bool) lua_toboolean(L, 1));
#endif
	return 0;
}

static int w__setAudioMixWithSystem(lua_State *L)
{
	bool success = false;

#ifdef LOVE_ENABLE_AUDIO
	bool mix = love::luax_checkboolean(L, 1);
	success = love::audio::Audio::setMixWithSystem(mix);
#endif

	love::luax_pushboolean(L, success);
	return 1;
}

static int w_love_setDeprecationOutput(lua_State *L)
{
	bool enable = love::luax_checkboolean(L, 1);
	love::setDeprecationOutputEnabled(enable);
	return 0;
}

static int w_love_hasDeprecationOutput(lua_State *L)
{
	love::luax_pushboolean(L, love::isDeprecationOutputEnabled());
	return 1;
}

static int w_deprecation__gc(lua_State *)
{
	love::deinitDeprecation();
	return 0;
}

//This example shows how to get info for the preselected user account. See libnx acc.h.

static int getNickname(lua_State *L)
{
    Result rc=0;

    AccountUid userID={0};
    AccountProfile profile;
    AccountUserData userdata;
    AccountProfileBase profilebase;

    char nickname[0x21];

    memset(&userdata, 0, sizeof(userdata));
    memset(&profilebase, 0, sizeof(profilebase));

    rc = accountInitialize(AccountServiceType_Application);
    if (R_FAILED(rc)) {
        printf("accountInitialize() failed: 0x%x\n", rc);
    }

    if (R_SUCCEEDED(rc)) {
        rc = accountGetPreselectedUser(&userID);

        if (R_FAILED(rc)) {
            printf("accountGetPreselectedUser() failed: 0x%x, using pselShowUserSelector..\n", rc);

            /* Create player selection UI settings */
            PselUserSelectionSettings settings;
            memset(&settings, 0, sizeof(settings));

            rc = pselShowUserSelector(&userID, &settings); //i ditn thigk this works or even matters since nobodies gonna use applet mode

            if (R_FAILED(rc)) {
                printf("pselShowUserSelector() failed: 0x%x\n", rc);
            }
        }

        if (R_SUCCEEDED(rc)) {
            printf("Current userID: 0x%lx 0x%lx\n", userID.uid[1], userID.uid[0]);

            rc = accountGetProfile(&profile, userID);

            if (R_FAILED(rc)) {
                printf("accountGetProfile() failed: 0x%x\n", rc);
            }
        }

        if (R_SUCCEEDED(rc)) {
            rc = accountProfileGet(&profile, &userdata, &profilebase);//userdata is otional, see libnx acc.h.

            if (R_FAILED(rc)) {
                printf("accountProfileGet() failed: 0x%x\n", rc);
            }

            if (R_SUCCEEDED(rc)) {
                memset(nickname,  0, sizeof(nickname));
                strncpy(nickname, profilebase.nickname, sizeof(nickname)-1);//Copy the nickname elsewhere to make sure it's NUL-terminated.

                printf("Nickname: %s\n", nickname);//Note that the print-console doesn't support UTF-8. The nickname is UTF-8, so this will only display properly if there isn't any non-ASCII characters. To display it properly, a print method which supports UTF-8 should be used instead.

                //You can also use accountProfileGetImageSize()/accountProfileLoadImage() to load the icon for use with a JPEG library, for displaying the user profile icon. See libnx acc.h.
            }

            accountProfileClose(&profile);
        }

        accountExit();
    }
	lua_pushstring(L, nickname);
	return 1;
}
//if someone can do this for me please do so :)
//nvm originally i was trying to get the vrersion file from the github website but i realized if i turned off https protection on my own site and jsut hosted the file there then i could just get the file contents easily with socket.https sooooooooooooooooooooo yeah
//static int getFileFromUrl(lua_State *L) {
//	lua_pushstring(L, "idk");
//	return 1;
//}


int luaopen_love(lua_State *L)
{
	love::luax_insistpinnedthread(L);

	love::luax_insistglobal(L, "love");

	// Set version information.
	lua_pushstring(L, love::VERSION);
	lua_setfield(L, -2, "_version");

	lua_pushnumber(L, love::VERSION_MAJOR);
	lua_setfield(L, -2, "_version_major");
	lua_pushnumber(L, love::VERSION_MINOR);
	lua_setfield(L, -2, "_version_minor");
	lua_pushnumber(L, love::VERSION_REV);
	lua_setfield(L, -2, "_version_revision");

	lua_pushstring(L, love::VERSION_CODENAME);
	lua_setfield(L, -2, "_version_codename");

#ifdef LOVE_ANDROID
	luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_OFF);
	lua_register(L, "print", w_print_sdl_log);
#endif

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK
	lua_pushcfunction(L, w__openConsole);
	lua_setfield(L, -2, "_openConsole");
#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

#ifdef LOVE_LEGENDARY_ACCELEROMETER_AS_JOYSTICK_HACK
	lua_pushcfunction(L, w__setAccelerometerAsJoystick);
	lua_setfield(L, -2, "_setAccelerometerAsJoystick");
#endif

	lua_pushcfunction(L, w__setGammaCorrect);
	lua_setfield(L, -2, "_setGammaCorrect");

	// Exposed here because we need to be able to call it before the audio
	// module is initialized.
	lua_pushcfunction(L, w__setAudioMixWithSystem);
	lua_setfield(L, -2, "_setAudioMixWithSystem");

	
	//#ifdef LOVE_ENABLE_NXUTIL
	lua_pushcfunction(L, getNickname);
	lua_setfield(L, -2, "getNickname");
	
	//lua_pushcfunction(L, getFileFromUrl);
	//lua_setfield(L, -2, "getFileFromUrl");
	//#endif

	lua_newtable(L);

	for (int i = 0; love::VERSION_COMPATIBILITY[i] != nullptr; i++)
	{
		lua_pushstring(L, love::VERSION_COMPATIBILITY[i]);
		lua_rawseti(L, -2, i+1);
	}

	lua_setfield(L, -2, "_version_compat");

	lua_pushcfunction(L, w_love_getVersion);
	lua_setfield(L, -2, "getVersion");

	//lua_pushcfunction(L, testFunc);
	//lua_setfield(L, -2, "testFunc");

	lua_pushcfunction(L, w_love_isVersionCompatible);
	lua_setfield(L, -2, "isVersionCompatible");

#ifdef LOVE_WINDOWS_UWP
	lua_pushstring(L, "UWP");
#elif LOVE_WINDOWS
	lua_pushstring(L, "Windows");
#elif defined(LOVE_MACOSX)
	lua_pushstring(L, "OS X");
#elif defined(LOVE_IOS)
	lua_pushstring(L, "iOS");
#elif defined(LOVE_ANDROID)
	lua_pushstring(L, "Android");
#elif defined(LOVE_NX)
	lua_pushstring(L, "NX");
#elif defined(LOVE_LINUX)
	lua_pushstring(L, "Linux");
#else
	lua_pushstring(L, "Unknown");
#endif
	lua_setfield(L, -2, "_os");

	{
		love::initDeprecation();

		// Any old data that we can attach a metatable to, for __gc. We want to
		// call deinitDeprecation when love is garbage collected.
		lua_newuserdata(L, sizeof(int));

		luaL_newmetatable(L, "love_deprecation");
		lua_pushcfunction(L, w_deprecation__gc);
		lua_setfield(L, -2, "__gc");
		lua_setmetatable(L, -2);

		lua_setfield(L, -2, "_deprecation");

		lua_pushcfunction(L, w_love_setDeprecationOutput);
		lua_setfield(L, -2, "setDeprecationOutput");

		lua_pushcfunction(L, w_love_hasDeprecationOutput);
		lua_setfield(L, -2, "hasDeprecationOutput");
	}

	// Preload module loaders.
	for (int i = 0; modules[i].name != nullptr; i++)
		love::luax_preload(L, modules[i].func, modules[i].name);

	// Necessary for Data-creating methods to work properly in Data subclasses.
	love::luax_require(L, "love.data");
	lua_pop(L, 1);

#ifdef LOVE_ENABLE_LUASOCKET
	love::luasocket::__open(L);
#endif
#ifdef LOVE_ENABLE_ENET
	love::luax_preload(L, luaopen_enet, "enet");
#endif
#ifdef LOVE_ENABLE_LUA53
	love::luax_preload(L, luaopen_luautf8, "utf8");
#endif

	return 1;
}

#ifdef LOVE_LEGENDARY_CONSOLE_IO_HACK

bool love_openConsole(const char *&err)
{
	static bool is_open = false;
	err = nullptr;

	if (is_open)
		return true;

	is_open = true;

	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		DWORD winerr = GetLastError();

		if (winerr == ERROR_ACCESS_DENIED)
		{
			// The process is already attached to a console. We'll assume stdout
			// and friends are already being directed there.
			is_open = true;
			return is_open;
		}

		// Create our own console if we can't attach to an existing one.
		if (!AllocConsole())
		{
			is_open = false;
			err = "Could not create console.";
			return is_open;
		}

		SetConsoleTitle(TEXT("LOVE Console"));

		const int MAX_CONSOLE_LINES = 5000;
		CONSOLE_SCREEN_BUFFER_INFO console_info;

		// Set size.
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_info);
		console_info.dwSize.Y = MAX_CONSOLE_LINES;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), console_info.dwSize);
	}

	FILE *fp;

	// Redirect stdout.
	fp = freopen("CONOUT$", "w", stdout);
	if (fp == NULL)
	{
		err = "Console redirection of stdout failed.";
		return is_open;
	}

	// Redirect stdin.
	fp = freopen("CONIN$", "r", stdin);
	if (fp == NULL)
	{
		err = "Console redirection of stdin failed.";
		return is_open;
	}

	// Redirect stderr.
	fp = freopen("CONOUT$", "w", stderr);
	if (fp == NULL)
	{
		err = "Console redirection of stderr failed.";
		return is_open;
	}

	return is_open;
}

int w__openConsole(lua_State *L)
{
	const char *err = nullptr;
	bool isopen = love_openConsole(err);
	if (err != nullptr)
		return luaL_error(L, err);
	love::luax_pushboolean(L, isopen);
	return 1;
}

#endif // LOVE_LEGENDARY_CONSOLE_IO_HACK

#ifdef LOVE_LEGENDARY_ACCELEROMETER_AS_JOYSTICK_HACK
int w__setAccelerometerAsJoystick(lua_State *L)
{
	bool enable = (bool) lua_toboolean(L, 1);
	SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, enable ? "1" : "0");
	return 0;
}
#endif // LOVE_LEGENDARY_ACCELEROMETER_AS_JOYSTICK_HACK

int luaopen_love_nogame(lua_State *L)
{
	if (luaL_loadbuffer(L, (const char *)love::nogame_lua, sizeof(love::nogame_lua), "nogame.lua") == 0)
		lua_call(L, 0, 1);

	return 1;
}

int luaopen_love_boot(lua_State *L)
{
	if (luaL_loadbuffer(L, (const char *)love::boot_lua, sizeof(love::boot_lua), "boot.lua") == 0)
		lua_call(L, 0, 1);

	return 1;
}


