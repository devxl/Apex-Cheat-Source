#include "main.h"
#include <thread>
#include <random>
#include <string>
#include <Windows.h>




void check()
{
	while (TRUE)
	{
		sofmain1337::process_id = sofmain1337::find_process("r5apex_dx12.exe");
		if (!sofmain1337::process_id)
		{
			exit(1);
		}
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
}

void random_title_thread()
{
	const char charset[] =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		"!\"§$%&/()=?`´*+'#-_.:,;<>|^°";

	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

	while (true)
	{
		std::string random_title;
		int length = rand() % 120 + 80; // Titel-Länge zwischen 10 und 40

		for (int i = 0; i < length; ++i)
		{
			random_title += charset[dist(rng)];
		}

		SetConsoleTitleA(random_title.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // alle 0,5 Sek aktualisieren
	}
}

auto init() -> bool
{
	HWND hwnd = GetConsoleWindow();
	LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 235, LWA_ALPHA); // Hier kannst du die Transparenz ändern (0–255)

	if (!sofmain1337::find_driver())
	{
		cout << "<!> Failed to find driver" << endl;
		return false;
	}

	cout << "Press F1 in lobby" << endl;	

	while (true)
	{
		if (GetAsyncKeyState(VK_F1))
		{
			break;
		}

	}

	sofmain1337::process_id = sofmain1337::find_process("r5apex_dx12.exe");
	if (!sofmain1337::process_id)
	{
		cout << "<!> Failed to find process" << endl;
		return false;
	}


	auto cr3 = sofmain1337::fetch_cr3();
	if (!cr3)
	{
		cout << "<!> Failed to resolve cr3" << endl;
		return false;
	}

	sofmainisud = sofmain1337::find_image();
	if (!sofmainisud)
	{
		cout << "<!> Failed to get base address" << endl;
		return false;
	}

	if (!input::init())
	{
		cout << "<!> Failed to Initialize input" << endl;
	}


	bool WindowFocus = false;
	while (WindowFocus == false) {
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (sofmain1337::find_process("r5apex_dx12.exe") == ForegroundWindowProcessID) {
			Process.ID = GetCurrentProcessId();
			Process.Handle = GetCurrentProcess();
			Process.Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(Process.Hwnd, &TempRect);
			Width = TempRect.right - TempRect.left;
			Height = TempRect.bottom - TempRect.top;
			Process.WindowLeft = TempRect.left;
			Process.WindowRight = TempRect.right;
			Process.WindowTop = TempRect.top;
			Process.WindowBottom = TempRect.bottom;

			char TempTitle[MAX_PATH];
			GetWindowText(Process.Hwnd, TempTitle, sizeof(TempTitle));
			Process.Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process.Hwnd, TempClassName, sizeof(TempClassName));
			Process.ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process.Handle, NULL, TempPath, sizeof(TempPath));
			Process.Path = TempPath;

			WindowFocus = true;
		}
	}

	setupoverlay();

	std::thread(setup).detach();
	std::thread(check).detach();

	FreeConsole();
	ShowWindow(hwnd, SW_HIDE);

	while (TRUE)
	{
		render_loop();
	}

}

auto main() -> void
{
	std::thread(random_title_thread).detach();

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Matrix Apex V1" << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); 

	if (!init())
	{
		cout << "<!> failed to intialize" << endl;
		Sleep(3000);
		exit(0);
	}
}