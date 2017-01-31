#include "main.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

WindowTest::WindowTest
(void)
   : Window(NULL, L"SvWindowTest")
{
   NONCLIENTMETRICS metrics;
   Font defaultFont;
   
   this->staticText.setParent(this);

   /*
   this->button.setParent(this);
   this->edit.setParent(this);
   */

   this->addStyle(WS_CAPTION | WS_SYSMENU);
   this->setSize(500, 500);
   this->center();

   memset(&metrics, 0, sizeof(NONCLIENTMETRICS));
   
   metrics.cbSize = sizeof(NONCLIENTMETRICS);

   if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS
                             ,sizeof(NONCLIENTMETRICS)
                             ,(LPVOID)&metrics
                             ,NULL))
      throw WindowException("SystemParametersInfo failed");

   defaultFont.setLogFont(metrics.lfMessageFont);

   this->staticText.setTypeface(defaultFont);
   this->staticText.setStyle(WS_CHILD | WS_VISIBLE);
   this->staticText.setSize(100, 100);
   this->staticText.setRelativePosition(0.1, 0.1);
   this->staticText.setWindowText(L"Hello, world!");
}

WindowTest::~WindowTest
(void)
{
   Window::~Window();
}

int CALLBACK
wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
   WindowTest test;

   test.show();

   Window::MessageLoop();

   return 0;
}
