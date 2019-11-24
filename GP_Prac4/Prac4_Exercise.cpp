#include <Windows.h>
#include <gl/GL.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

boolean x, y, z;
int page = 1;
float rotateHand = 0.0, rotateArm = 0.0; 

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if (wParam == VK_F1)
		{
			page = 1;
		}
		else if (wParam == VK_F2)
		{
			page = 2;
		}
		else if (wParam == 0x58)
		{
			x = true;
		}
		else if (wParam == 0x59)
		{
			y = true;
		}
		else if (wParam == 0x5A)
		{
			z = true;
		}
		else if (wParam == VK_UP)
		{
			rotateHand -= 1;
		}
		else if (wParam == VK_DOWN)
		{
			rotateHand += 1;
		}
		else if (wParam == VK_LEFT)
		{
			rotateArm -= 1;
		}
		else if (wParam == VK_RIGHT)
		{
			rotateArm += 1;
		}
		else if (wParam == VK_SPACE)
		{
			rotateHand = 0;
			rotateArm = 0;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void drawPyramid(float sz)
{
	glColor3f(1, 1, 1);
	//bottom
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(sz, 0, 0);
	glVertex3f(sz, sz, 0);
	glVertex3f(0, sz, 0);
	//front
	glVertex3f(0, 0, 0);
	glVertex3f(sz, 0, 0);
	glVertex3f(sz/2, sz/2, sz);
	//left
	glVertex3f(0, 0, 0);
	glVertex3f(0, sz, 0);
	glVertex3f(sz / 2, sz / 2, sz);
	//back
	glVertex3f(0, sz, 0);
	glVertex3f(sz, sz, 0);
	glVertex3f(sz / 2, sz / 2, sz);
	//right
	glVertex3f(sz, 0, 0);
	glVertex3f(sz, sz, 0);
	glVertex3f(sz / 2, sz / 2, sz);
	glEnd();
}

void drawRectangular(float size, float width)
{
	glColor3f(1, 1, 1);
	//bottom
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(size * width, 0, 0);
	glVertex3f(size * width, size, 0);
	glVertex3f(0, size, 0);
	//front
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, size);
	glVertex3f(size * width, 0, size);
	glVertex3f(size * width, 0, 0);
	//right
	glVertex3f(size * width, 0, 0);
	glVertex3f(size * width, 0, size);
	glVertex3f(size * width, size, size);
	glVertex3f(size * width, size, 0);
	//back
	glVertex3f(size * width, size, 0);
	glVertex3f(size * width, size, size);
	glVertex3f(0, size, size);
	glVertex3f(0, size, 0);
	//left
	glVertex3f(0, size, 0);
	glVertex3f(0, size, size);
	glVertex3f(0, 0, size);
	glVertex3f(0, 0, 0);
	//top
	glVertex3f(0, 0, size);
	glVertex3f(size * width, 0, size);
	glVertex3f(size * width, size, size);
	glVertex3f(0, size, size);
	glEnd();
}

void display()
{
	switch (page)
	{
	case 1:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		if (x == true) {
			glRotatef(-0.05, 1, 0, 0);
		}
		if (y == true) {
			glRotatef(-0.05, 0, 1, 0);
		}
		if (z == true) {
			glRotatef(-0.05, 0, 0, 1);
		}

		drawPyramid(0.5);
		break;
	case 2:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glPushMatrix();
		{
			glRotatef(0 + rotateArm, 1, 1, 1);
			drawRectangular(0.3, 2);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotatef(0 + rotateHand, 0, 0, 1);
			glRotatef(0 + rotateArm, 1, 1, 1);
			glTranslatef(-0.6, 0, 0);
			drawRectangular(0.3, 2);
		}
		glPopMatrix();
		
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------