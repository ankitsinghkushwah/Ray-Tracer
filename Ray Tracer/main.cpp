

//C++ headers
#include<memory>
#include<fstream>
//Windows specific headers
#include<windows.h>
#include<windowsx.h>
//custom headers
#include"timer.h"
#include"global_settings.h"
#include"logger.h"
#include"ray_tracer.h"
#include"enum_classes.h"

//Exposing namespaces

//Globals

//display device context
HDC hDisplayDC = nullptr;
float dt = 0.0f;

ray_tracer rt(global_settings::window_width, global_settings::window_height);

LPCWSTR title = L"Ray Tracer v1.0";


//forward declarations of functions

bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC,HDC hLocalDC);



LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);




/*
====================
WIN32 MAIN
====================
*/
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	
	HWND hWnd;
	
	WNDCLASSEX wc;

	
	ZeroMemory(&wc, sizeof(WNDCLASSEX));


	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	
	RegisterClassEx(&wc);

  RECT wr = { 0, 0, global_settings::window_width, global_settings::window_height };    // set the size, but not the position
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
	
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    
		title,   
		WS_OVERLAPPEDWINDOW,  
		0,   
		0,   
    wr.right - wr.left,    // width of the window
    wr.bottom - wr.top, 
		NULL,   
		NULL,    
		hInstance,   
		NULL);  
			
	ShowWindow(hWnd, nCmdShow);
	hDisplayDC = GetDC(hWnd);	
	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hDisplayDC);

	MSG msg = { 0 };


	logger rtLogger("==============RAY TRACER V1.0===============");
	timer rtTimer;

	rtTimer.start();

	int sigmaFPS = 0;

  while (true)
  {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
      {
		//  rtLogger.log("average FPS : %f", float(sigmaFPS/rtTimer.get_frame_count()));
        break;
      }
    } 
    else
    {
		rtTimer.update();
		dt =  rtTimer.get_elapsed_time();
		rtLogger.log("FPS : %d", rtTimer.get_FPS());
		rt.render();
        LoadAndBlitBitmap(L"result.bmp",hDisplayDC,hLocalDC);
    }
  }

  return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_UP:
			rt.updateCameraPos(Direction::FORWARD, dt);
			break;

		case VK_DOWN:
			rt.updateCameraPos(Direction::BACKWARD, dt);
			break;

		case VK_LEFT:
			rt.updateCameraPos(Direction::LEFT, dt);
			break;

		case VK_RIGHT:
			rt.updateCameraPos(Direction::RIGHT, dt);
			break;
		case 0x4E:
			rt.updateCameraPos(Direction::UP, dt);
			break;

		case 0x4D:
			rt.updateCameraPos(Direction::DOWN, dt);
			break;

		case 0x41://key A
			rt.updateLightPos(Direction::LEFT, dt);
			break;

		case 0x44://key D
			rt.updateLightPos(Direction::RIGHT, dt);
			break;

		case 0x53: //key S
			rt.updateLightPos(Direction::BACKWARD, dt);
			break;

		case 0x57: //key W
			rt.updateLightPos(Direction::FORWARD, dt);
			break;

		case 0x51: //key S
			rt.updateLightPos(Direction::UP, dt);
			break;

		case 0x45: //key W
			rt.updateLightPos(Direction::DOWN, dt);
			break;
		}

	}
	break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}




bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC,HDC hLocalDC) {
	// Load the bitmap image file
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);
	// Create a device context that is compatible with the window
// Get the bitmap's parameters and verify the get
	BITMAP qBitmap;
	GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
		reinterpret_cast<LPVOID>(&qBitmap));
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	::BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight,
		hLocalDC, 0, 0, SRCCOPY);
	// Unitialize and deallocate resources
	::DeleteObject(hBitmap);
	return true;
}