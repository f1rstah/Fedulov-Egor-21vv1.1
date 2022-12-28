#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Loavis-kur-syava.h"
#include <iostream>
#include <string>


std::ofstream out("output.txt");
std::ifstream in("input.txt");

COORD* coord;                                                       // Указатель на массив координат исходного графа неориентированного
COORD* coordTrans;                                                  // Указатель на массив координат графа транспортной сети 

int* mus;                                                           // Указатель на массив полной матрицы смежности исходного двудольного графа
int* transWay;                                                      // Указатель на массив графа транспортной сети (двудольный + еще исток и сток)
int* matrix;                                                        // Указатель на массив уменьшенной матрицы смежности, у которой строки это левая часть  двухдольного графа, а столбцы - правая часть двудольного графа

char N, P;                                                          // N - кол-во вершин, P - кол-во вершин + исток и сток
char M1, M2;                                                        // M1 - кол-во вершин в левой части,  М2 - кол-во вершин в правой части

char* copy;
char* copy2;

//====================================================================
// Создание матриц смежности (уменьшенной и полной), генерация значений и запись в матрицы
void makeGraf() {

	std::string s; // сюда будем класть считанные строки
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
		 N = M1 + M2;                                                    // Всего вершин - сумма левой и правой части
    P = N + 2;                                                      // В транспортной сети добавляется еще Исток и сток (+2)

    char M3 = (M1 > M2) ? M1 : M2;                                  // М3 - наиобольшее кол-во вершин в одной из частей

    mus = (int*)malloc(N * N * sizeof(int));                        // Создание полной матрицы смежности вершин 
    matrix = (int*)malloc(M3 * M3 * sizeof(int));                   // Создание уменьшенной матрицы смежности вершин размером M3 * M3 - двухмерный массив

    {
        bool badMus;                                                // BadMus - флаг повторной генерация строки или столбца массива

        for (char i = 0; i < M1; i++) {
            badMus = true;                                          // Обнуляем badMus
            for (char j = 0; j < M2; j++) {

                *(matrix + i * M1 + j) = rand() % 2 * rand() % 2 * rand() % 2;      // вероятность появления 1 равна 0.125 или (1/8)

                if (*(matrix + i * M1 + j) == 1)                    // Если имеется хотя бы одна 1, то перегенерация не нужна
                    badMus = false;               
            }
            if (badMus)                                             // Если все элементы равнялись = 0, повторно генерируем строку
                i--;
        }

        for (char j = 0; j < M2; j++) {

            badMus = true;                                          // Обнуляем badMus

            for (char i = 0; i < M1; i++) 
                if (*(matrix + i * M1 + j) == 1)                    // Если имеется хотя бы одна 1, то перегенерация не нужна
                    badMus = false;                       

            if (badMus) {                                           // Если все элементы равнялись = 0, повторно генерируем столбец
                for (char i = 0; i < M1; i++)
                    *(matrix + i * M1 + j) = rand() % 2 * rand() % 2 * rand() % 2;      // вероятность появления 1 равна 0.125 или (1/8)
                j--;                                                // Повторно проверяем стоблец
            }
        }
    }

    resetMatrix(&mus[0], N, N);                                     // Сброс матрицы в 0

    makeMatrixAdjac();                                              // Перенос уменьшенной матрицы смежности в полную матрицу смежности
	}
	
	if (found2) {
		N = M1 + M2;                                                    // Всего вершин - сумма левой и правой части
		P = N + 2;                                                      // В транспортной сети добавляется еще Исток и сток (+2)

		char M3 = (M1 > M2) ? M1 : M2;                                  // М3 - наиобольшее кол-во вершин в одной из частей

		mus = (int*)malloc(N * N * sizeof(int));                        // Создание полной матрицы смежности вершин 
		matrix = (int*)malloc(M3 * M3 * sizeof(int));                   // Создание уменьшенной матрицы смежности вершин размером M3 * M3 - двухмерный массив

		{
			bool badMus;                                                // BadMus - флаг повторной генерация строки или столбца массива

			for (char i = 0; i < M1; i++) {
				badMus = true;                                          // Обнуляем badMus
				for (char j = 0; j < M2; j++) {

					getline(in, s);
					copy = strcpy(new char[s.length() + 1], s.c_str());
					std::cout << copy;

					*(matrix + i * M1 + j) = copy[0] - 48;      // вероятность появления 1 равна 0.125 или (1/8)

					if (*(matrix + i * M1 + j) == 1)                    // Если имеется хотя бы одна 1, то перегенерация не нужна
						badMus = false;
				}
				if (badMus)                                             // Если все элементы равнялись = 0, повторно генерируем строку
					i--;
			}

			for (char j = 0; j < M2; j++) {

				badMus = true;                                          // Обнуляем badMus

				for (char i = 0; i < M1; i++)
					if (*(matrix + i * M1 + j) == 1)                    // Если имеется хотя бы одна 1, то перегенерация не нужна
						badMus = false;

				if (badMus) {                                           // Если все элементы равнялись = 0, повторно генерируем столбец
					for (char i = 0; i < M1; i++)
						* (matrix + i * M1 + j) = rand() % 2 * rand() % 2 * rand() % 2;      // вероятность появления 1 равна 0.125 или (1/8)
					j--;                                                // Повторно проверяем стоблец
				}
			}
		}

		resetMatrix(&mus[0], N, N);                                     // Сброс матрицы в 0

		makeMatrixAdjac();
	}
}

// Создание матрицы транспортной сети 
void makeTransPut() {
    transWay = (int*)malloc(P * P * sizeof(int));

    resetMatrix(&transWay[0], P, P);                                        // Сброс транспортной матрицы в 0

    for (char i = 0; i < P - 1; i++) {
        for (char j = i + 1; j < P; j++)
            if (i == 0 && j <= M1 || j == P - 1 && i > M1)                  // В 0-ой строке заполняется первые M1 вершин, которые относются к левой части двухдольного графа заносится 1 - так как еони смежные
                *(transWay + i * P + j) = 1;                                // В последнем столбце в строках под номерами вершин из правой части двухдольного графа заносится 1 - так как еони смежные
            else if (i > 0 && i < P - 1 && j > 0 && j < P - 1)              // Копирование полной матрицы смежности во внутрь матрицы транспортной сети с 1 строки и столбца -  по предпоследнюю строку и столбец
                *(transWay + i * P + j) = *(mus + (i - 1) * N + j - 1);
    }
}

// Создание массива координат
void makeCoord() {

    if (coord != NULL) {                                                        // Удаление массивов координат, 
        delete[] coord;                                                         // если они были созданы ранее
        delete[] coordTrans;
    }

    coord = new COORD[N];                                                       // Создание новых массивов координат для полной матрицы смежности
    coordTrans = new COORD[P];                                                  // и для матрицы транспортной сети

    char k1 = (M1 % 2 == 0) ? 40 : 0;                                           // Если число вершин четное в левой и в правой части по отдельности, то смещаем относительно 
    char k2 = (M2 % 2 == 0) ? 40 : 0;                                           // центра на половину растояния между вершинами :  k1 - для левой, k2 - для правой части

    POINT offset = { 300, 150 + 40 * (int)((M1 > M2 ? M1 : M2) / 2) };          // Смещение координат относительно (0, 0)

    for (char i = 0; i < N; i++) {
        if (i < M1) {                                                           // Если вершина относится к левой части графа, то не смещается больше по Х
            coord[i].X = offset.x;
            coord[i].Y = offset.y + (i - (int)(M1 / 2)) * 80 + k1;
        }
        else {                                                                  // Если вершина относится к правой части, то смещается на + 100 по Х
            coord[i].X = offset.x + 100;
            coord[i].Y = offset.y + (i - M1 - (int)(M2 / 2)) * 80 + k2;         // (-М1) - чтобы вершины были на одном уровне, а так как номерация вершин продолжается, необходимо вычесть вершины из левой части
        }
        coordTrans[i + 1].X = coord[i].X + 400;                                 // Копируем Полученные координаты для двухдольного графа полной матрицы смежности
        coordTrans[i + 1].Y = coord[i].Y;                                       // Но по Х смещается на 400, чтобы графы не пересекались
    }

    coordTrans[0].X = offset.x + 300;                                           // Смещается на 400 и вычитается 100, так как это веришина истока, она находится левее левой части графа
    coordTrans[0].Y = offset.y;                                                 // Координаты по У копируются
    coordTrans[P - 1].X = offset.x + 600;                                       // Смещается на 400 и добавляется еще 200, так как это вершина сток, и находится правее правой части графа
    coordTrans[P - 1].Y = offset.y;

}

//====================================================================
// Печать массива на экран - (ptr = указатель на массив, Size1 - кол-во строк, Size2 - кол-во столбцов, 
// x и y - смещение относительно (0,0), center - при = true - выделяет главную диагональ серым, a и b - индексы жлемента в массиве, который наадо выделить красным)
void printMus(int* ptr, char Size1, char Size2, HDC hdc, int x, int y, bool center, char a, char b) {



	for (char i = 0; i < Size1; i++)
	{
		out << "\n" << '\0';
		for (char j = 0; j < Size2; j++) {

			SetTextColor(hdc, (center && i == j) ? RGB(100, 100, 100) : ((i == a && j == b) ? RGB(255, 0, 0) : (*(ptr + i * Size1 + j) == 0) ? RGB(0, 255, 0) : RGB(0, 0, 0)));     // Выбор  цвета - если главная диагональ - то серым, если 0 - зеленый, если не 0 - черный, если выбранный элемент (a,b) - красным

			std::strstream str;
			str << (int) * (ptr + i * Size1 + j) << '\0';
			out << (int) * (ptr + i * Size1 + j) << '\0';

			TextOutA(hdc, x + j * 15, y + 15 * i, str.str(), strlen(str.str()));
		}
	}
}

// Копирование уменьшенной матрицы смежности в полную матрицу смежности графа
void makeMatrixAdjac() {
    for (char i = 0; i < M1; i++) {
        for (char j = 0; j < M2; j++)
            if (*(matrix + i * M1 + j) == 1) {                          // Если элемент равен 1, то заносится в полную матрицу по смещенным индексам
                *(mus + i * N + M1 + j) = 1;                            
                *(mus + (M1 + j) * N + i) = *(mus + i * N + M1 + j);    // Зеркалится относительно главной диагонали
            }
    }
}

// Сброс матрицы, обнуление всех элементов
void resetMatrix(int* ptr, char Size1, char Size2) {      // ptr - указатель, Size1 - кол-во строк, Size2 - кол-во столбцов,
    for (char i = 0; i < Size1; i++)
        for (char j = 0; j < Size2; j++)
            *(ptr + i * Size1 + j) = 0;
}

//====================================================================
// Задает начальные значения, обнуляет матрицы и вызывает функцию вывода резульата
void searchDepth(HWND hWnd, HDC hdc) {

    resetMatrix(&mus[0], N, N);                                     // Сброс матрицы mus в 0

    for (char i = 0; i < N; i++)                                    // Смещение координат исходного двухдольного 
        coord[i].X += 800;                                          // графа на 800, для вывода результата

    drawPoint(0, hdc);                                              // Вывод вершин исходного графа, но без ребер

    resetMatrix(&matrix[0], M1, M2);                                // Сброс уменьшенной матрицы matrix в 0

    char count = 0;                                                 // count - счетчик паросочетания или максимального потока
    for (char i = 0; i < M1; i++)                                   // Запускаем цикл от 0 до M1 - проходим все вершин левой части двухдольного графа
        if (searchDepthRec(hdc, 0, 0))                              // Функция возравщает true|false.  false - если функция не достигла стока, значит паросочетание не надено для этой вершины                      
            count++;                                                // При достижении стока, функция вернет true, значит путь найден и еще одно папросочетание               

    outPutResult(hdc, count);                                       // Вывод результата. Кол-во паросочетаний и уменьшенную матрицу

    makeMatrixAdjac();                                              // Перенос уменьшенной матрицы смежности в полную матрицу смежности                                            
}

// Рекурсивная функция поиска в глубину. Стартовые верши - это вершины левой части двухдольного графа
bool searchDepthRec(HDC hdc, char st, char sd) {

    for (char i = 0; i < P; i++) {                              // Поиск смежной вершины для текущей 
        if (*(transWay + st * P + i) != 0) {                    // Если вершина была найдена, то 

            drawLine(1, hdc, st, i, true, RGB(255, 255, 0));    // Отрисовывается ребро, как уже пройденное - анимация
            Sleep(500);                                         // Задержка на 0.5 сек

            if (i == P - 1)                                     // Если найденная вершина это стокк, то выход из функции с возвратом true
                return true;                                    // Так как путь от истока к стоку был найден

            for (int j = 0; j <= M1; j++)                       // Если текщая вершина не сток, то удаляются все ребра, которые входят в текущую вершину
                *(transWay + j * P + i) = 0;                    // чтобы исключить возможность, повторного захода в эту вершину

            if (i > M1)                                         
                *(matrix + (st - 1) * M1 + i - M1 - 1) = 1;     // Добавляется ребро в уменьшенную матрицу

            if (searchDepthRec(hdc, i, sd))                     // Рекурсивный вызов функции поиска в глубину
                return true;                                    // Если функция вернула - true, то выход и возврат true

        }
    }

    return false;                                               // Если сток не был достигнут, а вершин больше нет, знат паросочетания не существует для этой вершины, возврат false
}

// Вывод результата поиска
void outPutResult(HDC hdc, char count) {
	out << "\r\n Сверху исходная матрица " << '\0';
	out << "\r\n Максимальный поток = " << (int)count << " \n " << '\0';
	out << "\n Снизу обработанная матрица " << " \n " << '\0';

    SetTextColor(hdc, RGB(0, 0, 0));                                    // цвет текста в черный
    std::strstream str;                                                 
    str << "Максимальный поток = " << (int)count << "  " << '\0';
	

    TextOutA(hdc, 30, 270, str.str(), strlen(str.str()));               // вывод на экран максимальный поток или кол-во Max паросочетаний

    printMus(&matrix[0], M1, M2, hdc, 50, 300, false, -1, -1);          // Печать уменьшенной матрицы смежности, в которой остались только найденные паросочетания

	out << "\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << '\0';

}

//====================================================================
//Отрисовка ребер вершин графа
void drawLine(char index, HDC hdc, char cursor, char point, bool pow, DWORD colorPen) {

    HPEN RPEN = CreatePen(PS_SOLID, 3, colorPen);                       // Кисть рисования
    SelectObject(hdc, RPEN);

    int* ptr = mus;                                                     // Указатель на полный массив матрицы смежности
    char p = N;                                                         // Размер массива
    COORD* crd = coord;                                                 // Указатель на массив координат

    if (index == 1) {                                                   // Если это матрица транспортной сети
        ptr = transWay;                                                 // Указатель на матрицу транспротной сети
        p = P;                                                          // Её размер
        crd = coordTrans;                                               // и координаты
    }

    double pi = 3.14155926535, anglePow = 20 * pi / 180, widthPow = 20, rad = 21, angleLine, sinAngle, cosAngle;

    if (point == -1) {                              // ЕСли не указана вторая вершина, значит печать всех инцидентных ребер
        for (char i = cursor; i < p; i++) {
            if (*(ptr + cursor * p + i) != 0) {

                angleLine = atan2(crd[i].X - crd[cursor].X, crd[i].Y - crd[cursor].Y);      // Вычисления угла нарисованной линии, оносительно оси Ox 
                sinAngle = sin(angleLine) * rad;                                            // Уменьшение линии, чтобы не заходила на круги             
                cosAngle = cos(angleLine) * rad;

                MoveToEx(hdc, crd[cursor].X + sinAngle, crd[cursor].Y + cosAngle, NULL);    // Установка курсора на одну вершину
                LineTo(hdc, crd[i].X - sinAngle, crd[i].Y - cosAngle);                      // Отрисовка линии до другой верины
                if (pow) {                                                                  // Если указаны стрелки, (Pow == true)
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

// Отрисовка вершин графа

void drawPoint(char index, HDC hdc) {

    HPEN RPEN;

    char p = N;                                                         // Размер массива
    COORD* crd = coord;                                                 // Указатель на массив координат

    if (index == 1) {                                                   // Если это матрица транспортной сети
        p = P;                                                          // Её размер
        crd = coordTrans;                                               // и координаты
    }

    for (char i = 0, rad; i < p; i++) {

        RPEN = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, RPEN);
        rad = 20;

        Ellipse(hdc, crd[i].X - rad, crd[i].Y + rad, crd[i].X + rad, crd[i].Y - rad);       // Рисуем черный круг, с точкой границей

        RPEN = CreatePen(PS_SOLID, rad, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, RPEN);        
        rad = 9;

        Ellipse(hdc, crd[i].X - rad, crd[i].Y - rad, crd[i].X + rad, crd[i].Y + rad);       // Рисуем цветной круг до черного круг, чтобы был внутри черного
    }
}

//====================================================================
// Функция обработки сообщений

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {

        case WM_PAINT:
        {   
            makeGraf();                                                 // Создание матриц смежности
            makeTransPut();                                             // Создание транспортной сети
            makeCoord();                                                // Создание координат для матриц

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            printMus(&matrix[0], M1, M2, hdc, 50, 50, false, -1, -1);   // Вывод на экран исходной сгенерированой уменьшенной матрицы 

            for (char index = 0; index < 3; index++) {                  // ПРорисовка 0 1 0

                drawPoint(index, hdc);                                  // Отрисовка вершин

                for (char i = 0; i < ((index == 1) ? P : N); i++)       // Отрисовка ребер для каждой вершины
                    drawLine(index, hdc, i, -1, (index == 1), RGB(255, 0, 0));

                if (index == 1)                                         // Начало поиска в глубину после вывода транспортной матрицы
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
// Здесь ничего менять не надо!!
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
    HINSTANCE hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 1300, 720, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
