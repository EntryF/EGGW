#include <windows.h>

#include "WinMsgMap.h"
#include "WinDef.h"

#pragma once

class Window
{
private:
	// singleton ��������� ������������/�������� ������ window
	class WindowClass
	{
	public:
		static const char* GetName() noexcept; // ���������� ��� �����
		static HINSTANCE GetInstance() noexcept; // ���������� ����

	private:
		WindowClass() noexcept; // ����������� ������������ ����� ����
		WindowClass(const WindowClass&) = delete; // 
		WindowClass& operator=(const WindowClass&) = delete; // ���������� ��������� = ������� ������

		~WindowClass(); // ���������� ���������� ����� ����

		static constexpr const char* wndClassName = "Chili Direct3D Engine Window"; // �������� ����� ����
		static WindowClass wndClass; // ������ ����� ����, � ��������� � ��� ����� ��������� ������
		HINSTANCE hInst; // ���������� ���������� ������������� ���� ������
	};

public:
	Window(int width, int height, const char* name) noexcept; // ������������� ����
	Window(const Window&) = delete; // ��� ������������� ����
	Window& operator=(const Window&) = delete; // ���������� �������� = ������� ������

	~Window(); // ����������

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // ������������ ��������� Msg
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // ������������ �������� ���� ��� ���������� ��������������� ����������
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; // ������������ ������� ����

private:

	HWND hWnd; // ���������� ����
};

