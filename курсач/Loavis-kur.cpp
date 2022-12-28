#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Loavis-kur-syava.h"
#include <iostream>
#include <string>


std::ofstream out("output.txt");
std::ifstream in("input.txt");

COORD* coord;                                                       // ��������� �� ������ ��������� ��������� ����� ������������������
COORD* coordTrans;                                                  // ��������� �� ������ ��������� ����� ������������ ���� 

int* mus;                                                           // ��������� �� ������ ������ ������� ��������� ��������� ����������� �����
int* transWay;                                                      // ��������� �� ������ ����� ������������ ���� (���������� + ��� ����� � ����)
int* matrix;                                                        // ��������� �� ������ ����������� ������� ���������, � ������� ������ ��� ����� �����  ������������ �����, � ������� - ������ ����� ����������� �����

char N, P;                                                          // N - ���-�� ������, P - ���-�� ������ + ����� � ����
char M1, M2;                                                        // M1 - ���-�� ������ � ����� �����,  �2 - ���-�� ������ � ������ �����

char* copy;
char* copy2;

//====================================================================
// �������� ������ ��������� (����������� � ������), ��������� �������� � ������ � �������
void makeGraf() {

	std::string s; // ���� ����� ������ ��������� ������
	std::string stack1 = "auto";
	std::string stack2 = "manual";

	getline(in, s);
	copy = strcpy(new char[s.length() + 1], s.c_str());
	std::cout << copy;
	M1 = copy[0] - 48;

	getline(in, s);
	copy2 = strcpy(new char[s.length() + 1], s.c_str());
	std::cout << copy2;
	M2 = copy2[0] - 48;

	getline(in, s);
	bool found = stack1.find(s) != std::string::npos;
	bool found2 = stack2.find(s) != std::string::npos;
	if (found) {
		 N = M1 + M2;                                                    // ����� ������ - ����� ����� � ������ �����
    P = N + 2;                                                      // � ������������ ���� ����������� ��� ����� � ���� (+2)

    char M3 = (M1 > M2) ? M1 : M2;                                  // �3 - ����������� ���-�� ������ � ����� �� ������

    mus = (int*)malloc(N * N * sizeof(int));                        // �������� ������ ������� ��������� ������ 
    matrix = (int*)malloc(M3 * M3 * sizeof(int));                   // �������� ����������� ������� ��������� ������ �������� M3 * M3 - ���������� ������

    {
        bool badMus;                                                // BadMus - ���� ��������� ��������� ������ ��� ������� �������

        for (char i = 0; i < M1; i++) {
            badMus = true;                                          // �������� badMus
            for (char j = 0; j < M2; j++) {

                *(matrix + i * M1 + j) = rand() % 2 * rand() % 2 * rand() % 2;      // ����������� ��������� 1 ����� 0.125 ��� (1/8)

                if (*(matrix + i * M1 + j) == 1)                    // ���� ������� ���� �� ���� 1, �� ������������� �� �����
                    badMus = false;               
            }
            if (badMus)                                             // ���� ��� �������� ��������� = 0, �������� ���������� ������
                i--;
        }

        for (char j = 0; j < M2; j++) {

            badMus = true;                                          // �������� badMus

            for (char i = 0; i < M1; i++) 
                if (*(matrix + i * M1 + j) == 1)                    // ���� ������� ���� �� ���� 1, �� ������������� �� �����
                    badMus = false;                       

            if (badMus) {                                           // ���� ��� �������� ��������� = 0, �������� ���������� �������
                for (char i = 0; i < M1; i++)
                    *(matrix + i * M1 + j) = rand() % 2 * rand() % 2 * rand() % 2;      // ����������� ��������� 1 ����� 0.125 ��� (1/8)
                j--;                                                // �������� ��������� �������
            }
        }
    }

    resetMatrix(&mus[0], N, N);                                     // ����� ������� � 0

    makeMatrixAdjac();                                              // ������� ����������� ������� ��������� � ������ ������� ���������
	}
	
	if (found2) {
		N = M1 + M2;                                                    // ����� ������ - ����� ����� � ������ �����
		P = N + 2;                                                      // � ������������ ���� ����������� ��� ����� � ���� (+2)

		char M3 = (M1 > M2) ? M1 : M2;                                  // �3 - ����������� ���-�� ������ � ����� �� ������

		mus = (int*)malloc(N * N * sizeof(int));                        // �������� ������ ������� ��������� ������ 
		matrix = (int*)malloc(M3 * M3 * sizeof(int));                   // �������� ����������� ������� ��������� ������ �������� M3 * M3 - ���������� ������

		{
			bool badMus;                                                // BadMus - ���� ��������� ��������� ������ ��� ������� �������

			for (char i = 0; i < M1; i++) {
				badMus = true;                                          // �������� badMus
				for (char j = 0; j < M2; j++) {

					getline(in, s);
					copy = strcpy(new char[s.length() + 1], s.c_str());
					std::cout << copy;

					*(matrix + i * M1 + j) = copy[0] - 48;      // ����������� ��������� 1 ����� 0.125 ��� (1/8)

					if (*(matrix + i * M1 + j) == 1)                    // ���� ������� ���� �� ���� 1, �� ������������� �� �����
						badMus = false;
				}
				if (badMus)                                             // ���� ��� �������� ��������� = 0, �������� ���������� ������
					i--;
			}

			for (char j = 0; j < M2; j++) {

				badMus = true;                                          // �������� badMus

				for (char i = 0; i < M1; i++)
					if (*(matrix + i * M1 + j) == 1)                    // ���� ������� ���� �� ���� 1, �� ������������� �� �����
						badMus = false;

				if (badMus) {                                           // ���� ��� �������� ��������� = 0, �������� ���������� �������
					for (char i = 0; i < M1; i++)
						* (matrix + i * M1 + j) = rand() % 2 * rand() % 2 * rand() % 2;      // ����������� ��������� 1 ����� 0.125 ��� (1/8)
					j--;                                                // �������� ��������� �������
				}
			}
		}

		resetMatrix(&mus[0], N, N);                                     // ����� ������� � 0

		makeMatrixAdjac();
	}
}

// �������� ������� ������������ ���� 
void makeTransPut() {
    transWay = (int*)malloc(P * P * sizeof(int));

    resetMatrix(&transWay[0], P, P);                                        // ����� ������������ ������� � 0

    for (char i = 0; i < P - 1; i++) {
        for (char j = i + 1; j < P; j++)
            if (i == 0 && j <= M1 || j == P - 1 && i > M1)                  // � 0-�� ������ ����������� ������ M1 ������, ������� ��������� � ����� ����� ������������ ����� ��������� 1 - ��� ��� ���� �������
                *(transWay + i * P + j) = 1;                                // � ��������� ������� � ������� ��� �������� ������ �� ������ ����� ������������ ����� ��������� 1 - ��� ��� ���� �������
            else if (i > 0 && i < P - 1 && j > 0 && j < P - 1)              // ����������� ������ ������� ��������� �� ������ ������� ������������ ���� � 1 ������ � ������� -  �� ������������� ������ � �������
                *(transWay + i * P + j) = *(mus + (i - 1) * N + j - 1);
    }
}

// �������� ������� ���������
void makeCoord() {

    if (coord != NULL) {                                                        // �������� �������� ���������, 
        delete[] coord;                                                         // ���� ��� ���� ������� �����
        delete[] coordTrans;
    }

    coord = new COORD[N];                                                       // �������� ����� �������� ��������� ��� ������ ������� ���������
    coordTrans = new COORD[P];                                                  // � ��� ������� ������������ ����

    char k1 = (M1 % 2 == 0) ? 40 : 0;                                           // ���� ����� ������ ������ � ����� � � ������ ����� �� �����������, �� ������� ������������ 
    char k2 = (M2 % 2 == 0) ? 40 : 0;                                           // ������ �� �������� ��������� ����� ��������� :  k1 - ��� �����, k2 - ��� ������ �����

    POINT offset = { 300, 150 + 40 * (int)((M1 > M2 ? M1 : M2) / 2) };          // �������� ��������� ������������ (0, 0)

    for (char i = 0; i < N; i++) {
        if (i < M1) {                                                           // ���� ������� ��������� � ����� ����� �����, �� �� ��������� ������ �� �
            coord[i].X = offset.x;
            coord[i].Y = offset.y + (i - (int)(M1 / 2)) * 80 + k1;
        }
        else {                                                                  // ���� ������� ��������� � ������ �����, �� ��������� �� + 100 �� �
            coord[i].X = offset.x + 100;
            coord[i].Y = offset.y + (i - M1 - (int)(M2 / 2)) * 80 + k2;         // (-�1) - ����� ������� ���� �� ����� ������, � ��� ��� ��������� ������ ������������, ���������� ������� ������� �� ����� �����
        }
        coordTrans[i + 1].X = coord[i].X + 400;                                 // �������� ���������� ���������� ��� ������������ ����� ������ ������� ���������
        coordTrans[i + 1].Y = coord[i].Y;                                       // �� �� � ��������� �� 400, ����� ����� �� ������������
    }

    coordTrans[0].X = offset.x + 300;                                           // ��������� �� 400 � ���������� 100, ��� ��� ��� �������� ������, ��� ��������� ����� ����� ����� �����
    coordTrans[0].Y = offset.y;                                                 // ���������� �� � ����������
    coordTrans[P - 1].X = offset.x + 600;                                       // ��������� �� 400 � ����������� ��� 200, ��� ��� ��� ������� ����, � ��������� ������ ������ ����� �����
    coordTrans[P - 1].Y = offset.y;

}

//====================================================================
// ������ ������� �� ����� - (ptr = ��������� �� ������, Size1 - ���-�� �����, Size2 - ���-�� ��������, 
// x � y - �������� ������������ (0,0), center - ��� = true - �������� ������� ��������� �����, a � b - ������� �������� � �������, ������� ����� �������� �������)
void printMus(int* ptr, char Size1, char Size2, HDC hdc, int x, int y, bool center, char a, char b) {



	for (char i = 0; i < Size1; i++)
	{
		out << "\n" << '\0';
		for (char j = 0; j < Size2; j++) {

			SetTextColor(hdc, (center && i == j) ? RGB(100, 100, 100) : ((i == a && j == b) ? RGB(255, 0, 0) : (*(ptr + i * Size1 + j) == 0) ? RGB(0, 255, 0) : RGB(0, 0, 0)));     // �����  ����� - ���� ������� ��������� - �� �����, ���� 0 - �������, ���� �� 0 - ������, ���� ��������� ������� (a,b) - �������

			std::strstream str;
			str << (int) * (ptr + i * Size1 + j) << '\0';
			out << (int) * (ptr + i * Size1 + j) << '\0';

			TextOutA(hdc, x + j * 15, y + 15 * i, str.str(), strlen(str.str()));
		}
	}
}

// ����������� ����������� ������� ��������� � ������ ������� ��������� �����
void makeMatrixAdjac() {
    for (char i = 0; i < M1; i++) {
        for (char j = 0; j < M2; j++)
            if (*(matrix + i * M1 + j) == 1) {                          // ���� ������� ����� 1, �� ��������� � ������ ������� �� ��������� ��������
                *(mus + i * N + M1 + j) = 1;                            
                *(mus + (M1 + j) * N + i) = *(mus + i * N + M1 + j);    // ���������� ������������ ������� ���������
            }
    }
}

// ����� �������, ��������� ���� ���������
void resetMatrix(int* ptr, char Size1, char Size2) {      // ptr - ���������, Size1 - ���-�� �����, Size2 - ���-�� ��������,
    for (char i = 0; i < Size1; i++)
        for (char j = 0; j < Size2; j++)
            *(ptr + i * Size1 + j) = 0;
}

//====================================================================
// ������ ��������� ��������, �������� ������� � �������� ������� ������ ���������
void searchDepth(HWND hWnd, HDC hdc) {

    resetMatrix(&mus[0], N, N);                                     // ����� ������� mus � 0

    for (char i = 0; i < N; i++)                                    // �������� ��������� ��������� ������������ 
        coord[i].X += 800;                                          // ����� �� 800, ��� ������ ����������

    drawPoint(0, hdc);                                              // ����� ������ ��������� �����, �� ��� �����

    resetMatrix(&matrix[0], M1, M2);                                // ����� ����������� ������� matrix � 0

    char count = 0;                                                 // count - ������� ������������� ��� ������������� ������
    for (char i = 0; i < M1; i++)                                   // ��������� ���� �� 0 �� M1 - �������� ��� ������ ����� ����� ������������ �����
        if (searchDepthRec(hdc, 0, 0))                              // ������� ���������� true|false.  false - ���� ������� �� �������� �����, ������ ������������� �� ������ ��� ���� �������                      
            count++;                                                // ��� ���������� �����, ������� ������ true, ������ ���� ������ � ��� ���� ��������������               

    outPutResult(hdc, count);                                       // ����� ����������. ���-�� ������������� � ����������� �������

    makeMatrixAdjac();                                              // ������� ����������� ������� ��������� � ������ ������� ���������                                            
}

// ����������� ������� ������ � �������. ��������� ����� - ��� ������� ����� ����� ������������ �����
bool searchDepthRec(HDC hdc, char st, char sd) {

    for (char i = 0; i < P; i++) {                              // ����� ������� ������� ��� ������� 
        if (*(transWay + st * P + i) != 0) {                    // ���� ������� ���� �������, �� 

            drawLine(1, hdc, st, i, true, RGB(255, 255, 0));    // �������������� �����, ��� ��� ���������� - ��������
            Sleep(500);                                         // �������� �� 0.5 ���

            if (i == P - 1)                                     // ���� ��������� ������� ��� �����, �� ����� �� ������� � ��������� true
                return true;                                    // ��� ��� ���� �� ������ � ����� ��� ������

            for (int j = 0; j <= M1; j++)                       // ���� ������ ������� �� ����, �� ��������� ��� �����, ������� ������ � ������� �������
                *(transWay + j * P + i) = 0;                    // ����� ��������� �����������, ���������� ������ � ��� �������

            if (i > M1)                                         
                *(matrix + (st - 1) * M1 + i - M1 - 1) = 1;     // ����������� ����� � ����������� �������

            if (searchDepthRec(hdc, i, sd))                     // ����������� ����� ������� ������ � �������
                return true;                                    // ���� ������� ������� - true, �� ����� � ������� true

        }
    }

    return false;                                               // ���� ���� �� ��� ���������, � ������ ������ ���, ���� ������������� �� ���������� ��� ���� �������, ������� false
}

// ����� ���������� ������
void outPutResult(HDC hdc, char count) {
	out << "\r\n ������ �������� ������� " << '\0';
	out << "\r\n ������������ ����� = " << (int)count << " \n " << '\0';
	out << "\n ����� ������������ ������� " << " \n " << '\0';

    SetTextColor(hdc, RGB(0, 0, 0));                                    // ���� ������ � ������
    std::strstream str;                                                 
    str << "������������ ����� = " << (int)count << "  " << '\0';
	

    TextOutA(hdc, 30, 270, str.str(), strlen(str.str()));               // ����� �� ����� ������������ ����� ��� ���-�� Max �������������

    printMus(&matrix[0], M1, M2, hdc, 50, 300, false, -1, -1);          // ������ ����������� ������� ���������, � ������� �������� ������ ��������� �������������

	out << "\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << '\0';

}

//====================================================================
//��������� ����� ������ �����
void drawLine(char index, HDC hdc, char cursor, char point, bool pow, DWORD colorPen) {

    HPEN RPEN = CreatePen(PS_SOLID, 3, colorPen);                       // ����� ���������
    SelectObject(hdc, RPEN);

    int* ptr = mus;                                                     // ��������� �� ������ ������ ������� ���������
    char p = N;                                                         // ������ �������
    COORD* crd = coord;                                                 // ��������� �� ������ ���������

    if (index == 1) {                                                   // ���� ��� ������� ������������ ����
        ptr = transWay;                                                 // ��������� �� ������� ������������ ����
        p = P;                                                          // Ÿ ������
        crd = coordTrans;                                               // � ����������
    }

    double pi = 3.14155926535, anglePow = 20 * pi / 180, widthPow = 20, rad = 21, angleLine, sinAngle, cosAngle;

    if (point == -1) {                              // ���� �� ������� ������ �������, ������ ������ ���� ����������� �����
        for (char i = cursor; i < p; i++) {
            if (*(ptr + cursor * p + i) != 0) {

                angleLine = atan2(crd[i].X - crd[cursor].X, crd[i].Y - crd[cursor].Y);      // ���������� ���� ������������ �����, ����������� ��� Ox 
                sinAngle = sin(angleLine) * rad;                                            // ���������� �����, ����� �� �������� �� �����             
                cosAngle = cos(angleLine) * rad;

                MoveToEx(hdc, crd[cursor].X + sinAngle, crd[cursor].Y + cosAngle, NULL);    // ��������� ������� �� ���� �������
                LineTo(hdc, crd[i].X - sinAngle, crd[i].Y - cosAngle);                      // ��������� ����� �� ������ ������
                if (pow) {                                                                  // ���� ������� �������, (Pow == true)
                    LineTo(hdc, crd[i].X - sinAngle - sin(angleLine + anglePow) * widthPow, crd[i].Y - cosAngle - cos(angleLine + anglePow) * widthPow);    
                    MoveToEx(hdc, crd[i].X - sinAngle, crd[i].Y - cosAngle, NULL);
                    LineTo(hdc, crd[i].X - sinAngle - sin(angleLine - anglePow) * widthPow, crd[i].Y - cosAngle - cos(angleLine - anglePow) * widthPow);
                }
            }
        }
    }
    else {
        angleLine = atan2(crd[point].X - crd[cursor].X, crd[point].Y - crd[cursor].Y);
        sinAngle = sin(angleLine) * rad;
        cosAngle = cos(angleLine) * rad;

        MoveToEx(hdc, crd[cursor].X + sinAngle, crd[cursor].Y + cosAngle, NULL);
        LineTo(hdc, crd[point].X - sinAngle, crd[point].Y - cosAngle);

        if (pow) {
            LineTo(hdc, crd[point].X - sinAngle - sin(angleLine + anglePow) * widthPow, crd[point].Y - cosAngle - cos(angleLine + anglePow) * widthPow);
            MoveToEx(hdc, crd[point].X - sinAngle, crd[point].Y - cosAngle, NULL);
            LineTo(hdc, crd[point].X - sinAngle - sin(angleLine - anglePow) * widthPow, crd[point].Y - cosAngle - cos(angleLine - anglePow) * widthPow);
        }
    }
}

// ��������� ������ �����

void drawPoint(char index, HDC hdc) {

    HPEN RPEN;

    char p = N;                                                         // ������ �������
    COORD* crd = coord;                                                 // ��������� �� ������ ���������

    if (index == 1) {                                                   // ���� ��� ������� ������������ ����
        p = P;                                                          // Ÿ ������
        crd = coordTrans;                                               // � ����������
    }

    for (char i = 0, rad; i < p; i++) {

        RPEN = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, RPEN);
        rad = 20;

        Ellipse(hdc, crd[i].X - rad, crd[i].Y + rad, crd[i].X + rad, crd[i].Y - rad);       // ������ ������ ����, � ������ ��������

        RPEN = CreatePen(PS_SOLID, rad, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, RPEN);        
        rad = 9;

        Ellipse(hdc, crd[i].X - rad, crd[i].Y - rad, crd[i].X + rad, crd[i].Y + rad);       // ������ ������� ���� �� ������� ����, ����� ��� ������ �������
    }
}

//====================================================================
// ������� ��������� ���������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {

        case WM_PAINT:
        {   
            makeGraf();                                                 // �������� ������ ���������
            makeTransPut();                                             // �������� ������������ ����
            makeCoord();                                                // �������� ��������� ��� ������

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            printMus(&matrix[0], M1, M2, hdc, 50, 50, false, -1, -1);   // ����� �� ����� �������� �������������� ����������� ������� 

            for (char index = 0; index < 3; index++) {                  // ���������� 0 1 0

                drawPoint(index, hdc);                                  // ��������� ������

                for (char i = 0; i < ((index == 1) ? P : N); i++)       // ��������� ����� ��� ������ �������
                    drawLine(index, hdc, i, -1, (index == 1), RGB(255, 0, 0));

                if (index == 1)                                         // ������ ������ � ������� ����� ������ ������������ �������
                    searchDepth(hWnd, hdc);

            }

            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



//===============================================================================================
//===============================================================================================
// ����� ������ ������ �� ����!!
WCHAR szTitle[100];
WCHAR szWindowClass[100];


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow) {

	

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, 100);
    LoadStringW(hInstance, IDC_LOAVISKURSYAVA, szWindowClass, 100);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOAVISKURSYAVA));

    MSG msg;

    //srand((unsigned)time(NULL));

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOAVISKURSYAVA));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LOAVISKURSYAVA);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    HINSTANCE hInst = hInstance; // ��������� ������ ���������� � ���������� ����������

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 1300, 720, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
