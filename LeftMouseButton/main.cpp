#include<Windows.h>
#include<windowsx.h>
#include<string>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	const wchar_t* szAppName[] = { L"Left Mouse Button" };
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName[0];

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, L"Something went wrong!", szAppName[0], MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName[0], L"Get left mouse button coordinates", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return msg.wParam;
}

enum class ColorState
{
	INCREMENTRED,
	DECREMENTRED,
	INCREMENTGREEN,
	DECREMENTGREEN,
	INCREMENTBLUE,
	DECREMENTBLUE
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int colorMax = 255, colorMin = 0;
	static int mButtonX, mButtonY, mButtonXClickLocation, mButtonYClickLocation, red = colorMax, green = colorMin, blue = colorMin;
	static bool redIncrementer, redDecrementer, greenIncrementer, greenDecrementer, blueIncrementer, blueDecrementer;
	static ColorState currentColorState;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static std::string stringToPrint;
	static std::wstring wideVersionOfIt;
	static std::string moveString;
	static std::wstring moveStringWide;
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		if (red == colorMax && green == colorMin && blue == colorMin)
		{
			currentColorState = ColorState::INCREMENTGREEN;
		}
		if (red == colorMax && green == colorMax && blue == colorMin)
		{
			currentColorState = ColorState::DECREMENTRED;
		}
		if (red == colorMin && green == colorMax && blue == colorMin)
		{
			currentColorState = ColorState::INCREMENTBLUE;
		}
		if (red == colorMin && green == colorMax && blue == colorMax)
		{
			currentColorState = ColorState::DECREMENTGREEN;
		}
		if (red == colorMin && green == colorMin && blue == colorMax)
		{
			currentColorState = ColorState::INCREMENTRED;
		}
		if (red == colorMax && green == colorMin && blue == colorMax)
		{
			currentColorState = ColorState::DECREMENTBLUE;
		}
		if (currentColorState == ColorState::INCREMENTRED) red++;
		if (currentColorState == ColorState::DECREMENTRED) red--;
		if (currentColorState == ColorState::INCREMENTGREEN) green++;
		if (currentColorState == ColorState::DECREMENTGREEN) green--;
		if (currentColorState == ColorState::INCREMENTBLUE) blue++;
		if (currentColorState == ColorState::DECREMENTBLUE) blue--;
		SetTextColor(hdc, RGB(red, green, blue));
		TextOut(hdc, mButtonXClickLocation, mButtonYClickLocation, (LPCWSTR)wideVersionOfIt.c_str(), wideVersionOfIt.size());
		TextOut(hdc, mButtonX, mButtonY + 20, (LPCWSTR)moveStringWide.c_str(), moveStringWide.size());
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		mButtonX = GET_X_LPARAM(lParam);
		mButtonY = GET_Y_LPARAM(lParam);
		mButtonXClickLocation = mButtonX;
		mButtonYClickLocation = mButtonY;
		stringToPrint = "Mouse coordinate click: (" + std::to_string(mButtonX) + ", " + std::to_string(mButtonY) + ")";
		wideVersionOfIt = std::wstring(stringToPrint.begin(), stringToPrint.end());
		rect = { mButtonXClickLocation, mButtonYClickLocation, mButtonXClickLocation + 250, mButtonYClickLocation + 60 };
		InvalidateRect(hwnd, &rect, TRUE);
		return 0;
	case WM_MOUSEMOVE:
		mButtonX = GET_X_LPARAM(lParam);
		mButtonY = GET_Y_LPARAM(lParam);
		moveString = "Mouse location: (" + std::to_string(mButtonX) + ", " + std::to_string(mButtonY) + ")";
		moveStringWide = std::wstring(moveString.begin(), moveString.end());
		rect = { mButtonX, mButtonY, mButtonX + 250, mButtonY + 60 };
		InvalidateRect(hwnd, &rect, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, message, wParam, lParam);
}