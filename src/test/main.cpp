#include "main.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

GradientTest::GradientTest
(HWND parent)
   : LayeredWindow(parent, L"GradientTest")
{
   this->initialize(parent, L"GradientTest");
}

GradientTest::GradientTest
(Window *parent)
   : LayeredWindow(parent, L"GradientTest")
{
   this->initialize((parent == NULL) ? NULL : parent->getHWND(), L"GradientTest");
}

GradientTest::GradientTest
(void)
   : LayeredWindow()
{
   this->setClassName(L"GradientTest");
   this->initialize(NULL, L"GradientTest");
}

void
GradientTest::postCreate
(void)
{
   HDC screen, compatible;
   HBITMAP gradient, oldBitmap;
   POINT ptZero = { 0 };
   POINT fuckit = { 300, 150 };
   BLENDFUNCTION blender = { 0 };

   screen = GetDC(NULL);
   compatible = CreateCompatibleDC(screen);
   gradient = this->gradient.render(this->size.cx, this->size.cy);

   oldBitmap = (HBITMAP)SelectObject(compatible, gradient);

   blender.BlendOp = AC_SRC_OVER;
   blender.SourceConstantAlpha = 255;
   blender.AlphaFormat = AC_SRC_ALPHA;

   UpdateLayeredWindow(this->hwnd
                       ,screen
                       ,&fuckit
                       ,&this->size
                       ,compatible
                       ,&ptZero
                       ,this->transparency.colorRef()
                       ,&blender
                       ,ULW_ALPHA);
   
   SelectObject(compatible, oldBitmap);
   DeleteDC(compatible);
   DeleteObject(gradient);
   ReleaseDC(NULL, screen);
}

void
GradientTest::initialize
(HWND parent, std::wstring className)
{
   RelativePoint pt1, pt2;

   pt1.setX(0.5);

   if (!(pt1 < pt2) && !(pt2 < pt1))
      OutputDebugString(L"WTF FUCK OFF");

   this->gradient.addColor(RelativePoint(0.0, 0.0)
                           ,Color(Color::Alpha_Opaque
                                  ,255, 0, 0));
   this->gradient.addColor(RelativePoint(0.5, 0.0)
                           ,Color(Color::Alpha_Opaque
                                  ,0, 255, 0));
   this->gradient.addColor(RelativePoint(1.0, 0.0)
                           ,Color(Color::Alpha_Opaque
                                  ,0, 0, 255));

   this->gradient.addColor(RelativePoint(0.0, 1.0)
                           ,Color(Color::Alpha_Opaque
                                  ,255, 255, 0));
   this->gradient.addColor(RelativePoint(0.33, 1.0)
                           ,Color(Color::Alpha_Opaque
                                  ,255, 0, 255));
   this->gradient.addColor(RelativePoint(0.66, 1.0)
                           ,Color(Color::Alpha_Opaque
                                  ,0, 255, 255));
   this->gradient.addColor(RelativePoint(1.0, 1.0)
                           ,Color(Color::Alpha_Opaque
                                  ,255, 255, 255));
}

int CALLBACK
wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
   GradientTest test;

   test.addStyle(WS_POPUP);
   test.setSize(500, 500);
   test.setPosition((long)300, (long)150);
   // test.setAlpha(0xFF);

   test.update();
   test.show();

   Window::MessageLoop();

   return 0;
}
