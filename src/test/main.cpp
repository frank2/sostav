#include "main.hpp"

int CALLBACK
wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
   Sostav::Window window;
   Sostav::Win32::Static staticText(&window, L"Test string");
   Sostav::Win32::Edit editBox(&window);

   window.addStyle(WS_POPUP);
   window.setPosition(50, 50);
   window.setSize(200, 200);
   window.setBGColor(Sostav::Color::Alpha_Opaque
                     ,0xEE, 0xE1, 0xF4);

   staticText.setPosition(10, 30);
   staticText.setSize(100, 20);
   staticText.setBGColor(Sostav::Color::Transparent());

   editBox.setPosition(10, 60);
   editBox.setSize(100, 20);
   editBox.setFGColor(Sostav::Color::Alpha_Opaque
                      ,0xFF, 0xFF, 0xFF);
   editBox.setBGColor(Sostav::Color::Alpha_Opaque
                      ,0x08, 0x42, 0x50);
   
   window.show();

   Sostav::Window::MessageLoop();

   return 0;
}
