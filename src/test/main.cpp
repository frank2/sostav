#include "main.hpp"

using namespace Sostav;
using namespace Sostav::Windows;

WindowTest::WindowTest
(void)
   : Window(NULL, L"SvWindowTest")
{
   this->staticText.setParent(this);
   this->button.setParent(this);
   this->edit.setParent(this);

   this->addStyle(WS_POPUP);
   this->setSize(500, 500);
   this->center();

   this->staticText.setPosition(50, 50);
   this->staticText.setWindowText(L"Hello, world!");
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
