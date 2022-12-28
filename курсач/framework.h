#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <strstream>
#include <queue>
#include <time.h>

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void makeGraf(void);
void makeTransPut(void);
void makeCoord(void);

void printMus(int*, char, char, HDC, int, int, bool, char, char);

void drawPoint(char, HDC);
void drawLine(char, HDC, char, char, bool, DWORD);

bool searchDepthRec(HDC, char, char);
void outPutResult(HDC, char);

void makeMatrixAdjac(void);
void resetMatrix(int*, char, char);
