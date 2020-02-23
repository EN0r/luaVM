// luaTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include "string"
#include "Windows.h"
#include <iostream>


extern "C"
{
#include "Vendor/lua5.3/include/lua.h"
#include "Vendor/lua5.3/include/lauxlib.h"
#include "Vendor/lua5.3/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "Vendor/lua5.3/liblua53.a")
#endif

class luaCpp
{
protected:
	lua_State* L = luaL_newstate();

public:

	bool checkLua(lua_State* L, int r);
	void initLua(); // define lua funcs here
	void startLua(std::string fileDirectory);
	void luaRun();

	~luaCpp();
};

luaCpp::~luaCpp()
{
	lua_close(L);
}

// custom functions
static int pause(lua_State* L)
{
	system("pause");
	return 0;
}

static int clearConsole(lua_State* L)
{
	system("cls");
	return 0;
}


static int consoleColor(lua_State* L)
{
	int color = static_cast<int> (luaL_checknumber(L, 1));
	system(" COLOR " + color);
	return 0;
}

static int wait(lua_State* L)
{

	int time = static_cast<int> (luaL_checknumber(L, 1));
	Sleep(time);
	return 0;
}


// Lua stuffs

bool luaCpp::checkLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string err = lua_tostring(L, -1);
		std::cout << err << "\n";
		return false;
	}
	return true;
}

void luaCpp::initLua()
{
	luaL_openlibs(L);
}

void luaCpp::startLua(std::string fileDirectory)
{

	// register custom functions here
	lua_register(L, "wait", wait);
	lua_register(L, "clearConsole", clearConsole);
	lua_register(L, "fontColor", consoleColor);
	lua_register(L,"pause",pause);



	checkLua(L, luaL_dofile(L, fileDirectory.c_str())); // open file and look for errors

}

void luaCpp::luaRun()
{

	lua_getglobal(L, "Update");
	if (lua_pcall(L, 0, 0, 0))
	{
		std::cout << "WARNING # Update function not found!" << "\n";
		return;
	}

	lua_getglobal(L, "Start");
	if (lua_pcall(L, 0, 0, 0))
	{
		std::cout << "WARNING  # Start function not found!" << "\n";
		return;
	}

	
	
}

int main()
{
	luaCpp Lua;
	Lua.initLua();

	std::cout << "INFO # Please state the name of the script : " << "\n";
	std::string path;
	std::getline(std::cin, path);
	std::string basepath = "Scripts/" + path;
	system("cls");
	system("COLOR 3");

	Lua.startLua(basepath);
	Lua.luaRun();

	system("pause");
}