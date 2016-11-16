#include "main.hpp"

int CALLBACK
wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
   Sostav::Window window;
   Sostav::SubclassedWindow staticText(&window, L"STATIC");

   window.setPosition(50, 50);
   window.setSize(200, 200);

   staticText.setPosition(10, 30);
   staticText.setSize(100, 20);
   staticText.setWindowText(L"Test string");
   
   window.show();
   staticText.show();

   Sostav::Window::MessageLoop();

   return 0;
}
