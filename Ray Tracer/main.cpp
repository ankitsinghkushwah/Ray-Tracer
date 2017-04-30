

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
#include"vec3.h"
#include"ray.h"
#include"hitable_list.h"
#include"hitable.h"
#include"sphere.h"
#include"random.h"
#include"camera.h"
#include"material.h"
#include"diffuse_material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

//Exposing namespaces

//Globals
HDC hDisplayDC = nullptr;
LPCWSTR title = L"Ray Tracer";


//forward declarations
bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC);



LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);






vec3 color(const ray& r,hitable_list& list,int depth) {

  hit_record temp;
  bool got_hit = list.hit(r,0.001f,global_settings::MAXFLOAT,temp);
  if (got_hit) {
    vec3 attenuation;
    ray scattered;
    if (depth < 50 && temp.mat->scatter(r, temp, attenuation, scattered)){
      return attenuation*color(scattered,list,depth+1);
    }
    else{
      return vec3(0.0f);
    }
  }
  else
  {
    vec3 unitDirection = r.get_direction().normalize();
    float t = 0.5f*(unitDirection.y() + 1.0f);
    return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + (t*vec3(0.5f, 0.7f, 1.0f));
  }
}


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
	
	//Setting up data to render

	//Initializing timer and logger
	std::shared_ptr<timer> _timer = std::make_shared<timer>();
	std::unique_ptr<logger>_logger = std::make_unique<logger>(
		(std::string("Log : ")+_timer->get_current_time_and_date()).c_str()
	);

  auto randomizer = random::getInstance();

  //generating image.bmp
  
  unsigned char* image = nullptr;
  int w = global_settings::window_width;
  int h = global_settings::window_height;
  camera cam;

  hitable *list[4];
  list[0] = new sphere(vec3(0.0f,0.0f,-1.0f),0.5f, new diffuse_material(vec3(.8,.3,.3)));
  list[1] = new sphere(vec3(0.0f, -100.5f,-1.0f), 100.0f, new diffuse_material(vec3(.8, .8, .0)));
  list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), .5f, new diffuse_material(vec3(.8, .6, .2)));
  list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), .5f, new diffuse_material(vec3(.8, .8, .8)));
  hitable_list world(list,4);
  int maxSamples = 30;
  vec3 finalColor(0.0f);
  image = new unsigned char[w*h*3];


#define ANTI_ALIASING
#define GAMMA

  for (int row = 0 ; row <h  ; row++) {
  
    for (int col = 0; col < w; col++) {
   

#ifdef ANTI_ALIASING

      for (int sample = 0; sample < maxSamples; sample++)
      {
        float u = float(col + randomizer->real(0,1)) / float(w);
        float v = float(row + randomizer->real(0,1)) / float(h);
        ray r = cam.get_ray(u,v);
        finalColor = finalColor + color(r, world,0);
      }

      finalColor /= maxSamples;

#else

      float u = float(col) / float(w);
      float v = float(row) / float(h);
      ray r = cam.get_ray(u, v);
      finalColor = color(r, world);

#endif

#ifdef GAMMA
      finalColor = vec3(sqrt(finalColor.x()), sqrt(finalColor.y()), sqrt(finalColor.z()));
#endif

      int ir = (int)(255.99f*finalColor.r());
      int ig = (int)(255.99f*finalColor.g());
      int ib = (int)(255.99f*finalColor.b());

      int revRow = (h - 1)-row;
      int index = ((revRow*w)+col)*3;

      image[index] = ir;
      image[index+1] = ig;
      image[index+2] = ib;

      finalColor = vec3(0.0f);
    }
  }

  stbi_write_bmp("image.bmp",w,h,3,image);
  delete[] image;

  int FPS = 0;
  MSG msg = { 0 };

  while (true)
  {
    // If there are Window messages then process them.
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
      {
        _logger->log("Fps = %d",FPS);
        break;
      }
    } 
    else
    {
      _timer->update();
      FPS = _timer->get_FPS();
      LoadAndBlitBitmap(L"image.bmp",hDisplayDC);
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
    return 0;
    break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}




bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC)
{
	// Load the bitmap image file
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);
	// Verify that the image was loaded
	if (hBitmap == NULL) {
		::MessageBox(NULL,L"LoadImage Failed",L"Error", MB_OK);
		return false;
	}

	// Create a device context that is compatible with the window
	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hWinDC);
	// Verify that the device context was created
	if (hLocalDC == NULL) {
		::MessageBox(NULL, L"CreateCompatibleDC Failed", L"Error", MB_OK);
		return false;
	}

	// Get the bitmap's parameters and verify the get
	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
		reinterpret_cast<LPVOID>(&qBitmap));
	if (!iReturn) {
		::MessageBox(NULL, L"GetObject Failed", L"Error", MB_OK);
		return false;
	}

	// Select the loaded bitmap into the device context
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL) {
		::MessageBox(NULL, L"SelectObject Failed", L"Error", MB_OK);
		return false;
	}

	// Blit the dc which holds the bitmap onto the window's dc
	BOOL qRetBlit = ::BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight,
		hLocalDC, 0, 0, SRCCOPY);
	if (!qRetBlit) {
		::MessageBox(NULL, L"Blit Failed", L"Error", MB_OK);
		return false;
	}

	// Unitialize and deallocate resources
	::SelectObject(hLocalDC, hOldBmp);
	::DeleteDC(hLocalDC);
	::DeleteObject(hBitmap);
	return true;
}