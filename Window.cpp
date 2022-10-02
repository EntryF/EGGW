#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass; // инициализируем класс окна (статический)

Window::WindowClass::WindowClass() noexcept // функция не вызывает исключений
{
	WNDCLASSEX wc =			{ 0 };				// инициализируем класс окна
	wc.cbSize =				sizeof( wc );		// размер окна
	wc.style =				CS_OWNDC;			// стиль окна
	wc.lpfnWndProc =		HandleMsgSetup;		// проецедуры над окном
	wc.cbClsExtra =			0;					//
	wc.cbWndExtra =			0;					//
	wc.hInstance =			GetInstance();		// экземпляр дексриптора окна
	wc.hIcon =				nullptr;			// иконка окна
	wc.hCursor =			nullptr;			// фон окна
	wc.hbrBackground =		nullptr;			//
	wc.lpszMenuName =		nullptr;			//
	wc.lpszClassName =		GetName();			// название класса окна
	wc.hIconSm =			nullptr;			//
	RegisterClassEx(&wc);						// регистрируем класс окна
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance()); // уничтожаем класс окна
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName; // воврощаем название класс окна
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst; // возвращаем экземпляр дескриптора класса окна
}

Window::Window(int width, int height, const char* name) noexcept
{
	// координаты окна
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	//вычисляет требуемый размер прямоугольника окна
	AdjustWindowRect( &wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE );

	// создания окна
	hWnd = CreateWindow(
		WindowClass::GetName(),name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,wr.right - wr.left,wr.bottom - wr.top,
		nullptr,nullptr,WindowClass::GetInstance(),this);

	ShowWindow(hWnd, SW_SHOWDEFAULT); // показать окно
}

Window::~Window()
{
	DestroyWindow(hWnd); // уничтожить окно
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// используйте параметр create, переданный из CreateWindow(), для сохранения указателя класса окна на стороне WinAPI
	if (msg == WM_NCCREATE)
	{
		// извлечь ptr в класс window из данных создания
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// установите пользовательские данные, управляемые WinAPI, для хранения ptr в классе window
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// установите message proc в обычный (не настроенный) обработчик теперь, когда настройка завершена
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		// перенаправить сообщение обработчику класса window
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// если мы получим сообщение до сообщения WM_NCCREATE, обработайте с помощью обработчика по умолчанию
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// извлечение ptr в класс window
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// перенаправить сообщение обработчику класса window
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// вывод информации о взаимодействие пользователя с окном
	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam).c_str());

	switch( msg )
	{
	// мы не хотим, чтобы DefProc обрабатывал это сообщение
	// мы хотим, чтобы наш деструктор уничтожил окно, поэтому вернем 0 вместо break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}