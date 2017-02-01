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
   this->edit.setParent(this);

   /*
   this->button.setParent(this);
   this->edit.setParent(this);
   */

   this->setBGColor(0xFF, 0xCC, 0x99, 0xCC);
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
   this->staticText.setBorderSize(2);
   this->staticText.setBGColor(Color::Transparent());
   this->staticText.setFGColor(0xFF, 0xFF, 0xFF, 0);

   this->edit.setRelativePosition(0.1, 0.3);
   this->edit.setTypeface(defaultFont);
   this->edit.setStyle(WS_CHILD | WS_VISIBLE);
   this->edit.setSize(200, 200);
   this->edit.setWindowText(L"I'm an edit box! \\o/");
   this->edit.setBorderSize(2);
   this->edit.setBorderColor(0xFF, 0x99, 0xCC, 0xCC);
   this->edit.setBGColor(0xFF, 0x33, 0x99, 0x99);
   this->edit.setFGColor(0xFF, 0xFF, 0xFF, 0xFF);
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
