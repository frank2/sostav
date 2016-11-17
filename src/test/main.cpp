#include "main.hpp"

int CALLBACK
wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
   Sostav::Window window;
   Sostav::Win32::Static staticText(&window, L"Test string");

   window.setStyle(WS_POPUP);
   window.setPosition(50, 50);
   window.setSize(200, 200);
   window.setBGColor(Sostav::Color::Alpha_Opaque
                     ,0xEE, 0xE1, 0xF4);

   staticText.setPosition(10, 30);
   staticText.setSize(100, 20);
   staticText.setBGColor(Sostav::Color::Transparent());
   
   window.show();

   Sostav::Window::MessageLoop();

   return 0;
}
