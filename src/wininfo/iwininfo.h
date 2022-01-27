#pragma once
#include <windows.h>
#include <stdint.h>
#include <atlstr.h>
#include <vector>

class WinInfo
{
public:
	WinInfo() {
		left = 0;
		right = 0;
		width = 0;
		height = 0;
		hwnd = nullptr;
		prodectPath = _T("");
		prodectName = _T("");
	}

	WinInfo(const RECT rt, const HWND hwnd, const CString prodectPath) {
		left = rt.left;
		right = rt.top;
		width = rt.right - rt.left;
		height = rt.bottom - rt.top;
	}

private:
	int32_t left;
	int32_t right;
	int32_t width;
	int32_t height;
	HWND hwnd;
	CString prodectPath;
	CString prodectName;
};

class ISysWinInfo
{
public:
	virtual void getAllWinInfo(std::vector<WinInfo>& vec) = 0;
};

