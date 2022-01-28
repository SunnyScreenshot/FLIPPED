#pragma once
#include <windows.h>
#include <atlstr.h>
#include <vector>

class WinInfo
{
public:
	WinInfo() {
		left = 0;
		top = 0;
		width = 0;
		height = 0;
		hwnd = nullptr;
		procPath = _T("");
		procName = _T("");
	}

	WinInfo(const RECT rt, const HWND hwnd, const CString procPath) {
		left = rt.left;
		top = rt.top;
		width = rt.right - rt.left;
		height = rt.bottom - rt.top;
	}

//private:
	int32_t left;
	int32_t top;
	int32_t width;
	int32_t height;
	HWND hwnd;
	CString procPath;
	CString procName;
};

class ISysWinInfo
{
public:
	virtual void getAllWinInfo(std::vector<WinInfo>& vec) = 0;
};

