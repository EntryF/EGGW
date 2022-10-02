#include <windows.h>

#include "WinMsgMap.h"
#include "WinDef.h"

#pragma once

class Window
{
private:
	// singleton управл€ет регистрацией/очисткой класса window
	class WindowClass
	{
	public:
		static const char* GetName() noexcept; // возвращает им€ класс
		static HINSTANCE GetInstance() noexcept; // дескриптор окна

	private:
		WindowClass() noexcept; // конструктор регистрирует класс окна
		WindowClass(const WindowClass&) = delete; // 
		WindowClass& operator=(const WindowClass&) = delete; // перегрузка оператора = очищает пам€ть

		~WindowClass(); // диструктор уничтожаем класс окна

		static constexpr const char* wndClassName = "Chili Direct3D Engine Window"; // называем класс окна
		static WindowClass wndClass; // создаЄм класс окна, в дальнейше с ним будет проходить работа
		HINSTANCE hInst; // дескриптор экземпл€ра присваеватес€ окну класса
	};

public:
	Window(int width, int height, const char* name) noexcept; // инициализаци€ окна
	Window(const Window&) = delete; // дл€ переделывани€ окна
	Window& operator=(const Window&) = delete; // перегрузка оператор = очищаем пам€ть

	~Window(); // деструктор

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // обрабатывать Ќастройку Msg
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // обрабатывать фрагмент кода дл€ выполнени€ дополнительного вычислени€
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // обрабатывать событи€ окна

private:

	HWND hWnd; // дескриптор окна
};

