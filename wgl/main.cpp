#include <windows.h>
LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
// function declarations
int mainMessageLoop(HACCEL hAccelTable=0);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdArgs, int cmdShow){
	wchar_t className[256];
	wcsncpy_s(className, L"OpenGL", 256-1);
	WNDCLASSEX winClass; 
	winClass.cbSize        = sizeof(WNDCLASSEX);
    winClass.style         = 0;                                     // class styles: CS_OWNDC, CS_PARENTDC, CS_CLASSDC, CS_GLOBALCLASS, ...
    winClass.lpfnWndProc   = windowProcedure;                  // pointer to window procedure
    winClass.cbClsExtra    = 0;
    winClass.cbWndExtra    = 0;
    winClass.hInstance     = hInst;                              // owner of this class
    winClass.hIcon         = LoadIcon(hInst, IDI_APPLICATION);   // default icon
    winClass.hIconSm       = 0;
    winClass.hCursor       = LoadCursor(0, IDC_ARROW);              // default arrow cursor
    winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // default white brush
    winClass.lpszMenuName  = 0;
	winClass.lpszClassName = (LPCSTR)className;
    winClass.hIconSm       = LoadIcon(hInst, IDI_APPLICATION);   // default small icon
	if(!::RegisterClassEx(&winClass)) return 0;

    HWND handle = ::CreateWindowEx(WS_EX_WINDOWEDGE,           // window border with a sunken edge
		(LPCSTR)className,            // name of a registered window class
                              (LPCSTR)className,                // caption of window
                              WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,             // window style
                              CW_USEDEFAULT,                    // x position
                              CW_USEDEFAULT,                    // y position
                              CW_USEDEFAULT,                // witdh
                              CW_USEDEFAULT,               // height
                              0,         // handle to parent window
                              0,           // handle to menu
							  hInst,             // application instance
                              (LPVOID)0);  // window creation data
	if (!handle){
		DWORD error = GetLastError();
	}
	::ShowWindow(handle, SW_SHOWDEFAULT);
    ::UpdateWindow(handle);
	// main message loop //////////////////////////////////////////////////////
    int exitCode;
    HACCEL hAccelTable = 0;
    //hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(ID_ACCEL));
    exitCode = mainMessageLoop(hAccelTable);

    return exitCode;
}

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	LRESULT returnValue = 0;        // return value
	if(WM_CLOSE == msg){
		DestroyWindow(hwnd);
		PostQuitMessage(0);
	}else{
		returnValue = ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return returnValue;
}

int mainMessageLoop(HACCEL hAccelTable)
{
    HWND activeHandle;
    MSG msg;

    while(::GetMessage(&msg, 0, 0, 0) > 0)  // loop until WM_QUIT(0) received
    {
        // determine the activated window is dialog box
        // skip if messages are for the dialog windows
        activeHandle = GetActiveWindow();
        if(::GetWindowLongPtr(activeHandle, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) // WS_EX_CONTROLPARENT is automatically added by CreateDialogBox()
        {
            if(::IsDialogMessage(activeHandle, &msg))
                continue;   // message handled, back to while-loop
        }

        // now, handle window messages
        if(!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;                 // return nExitCode of PostQuitMessage()
}

