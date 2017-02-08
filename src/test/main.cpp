#include "main.hpp"

using namespace Sostav;
using namespace Sostav::Drawing;
using namespace Sostav::Windows;

WindowTest::WindowTest
(void)
   : Window(NULL, L"SvWindowTest")
{
   Font defaultFont = Font::MessageFont();

   this->firstLaunch = true;
   
   this->sostavBanner.setParent(this);
   this->enabledStatic.setParent(this);
   this->disabledStatic.setParent(this);
   this->enabledEdit.setParent(this);
   this->disabledEdit.setParent(this);
   this->enabledPushButton.setParent(this);
   this->disabledPushButton.setParent(this);
   this->enabledRadioButton.setParent(this);
   this->disabledRadioButton.setParent(this);
   this->enabledCheckButton.setParent(this);
   this->disabledCheckButton.setParent(this);
   this->trayIcon.setParent(this);

   this->setIcon(Icon::MainIcon());
   this->setBGColor(0xFF, 0x3A, 0x1A, 0x3A);
   this->addStyle(WS_CAPTION | WS_SYSMENU);
   this->setSize(500, 500);
   this->center();

   this->sostavBanner.setStyle(WS_CHILD | WS_VISIBLE);
   this->sostavBanner.setImage(Image(MAKEINTRESOURCE(IDI_SOSTAV), L"PNG"));
   this->sostavBanner.setPosition(10, 10);
   this->sostavBanner.setBGColor(Color::Transparent());
   this->sostavBanner.centerX();

   this->enabledStatic.setStyle(WS_CHILD | WS_VISIBLE);
   this->enabledStatic.setPosition(100, 100);
   this->enabledStatic.setSize(100, 25);
   this->enabledStatic.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->enabledStatic.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->enabledStatic.setWindowText(L"Enabled static");
   this->enabledStatic.setTypeface(defaultFont);

   this->disabledStatic.setStyle(WS_CHILD | WS_VISIBLE);
   this->disabledStatic.setPosition(300, 100);
   this->disabledStatic.setSize(100, 25);
   this->disabledStatic.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->disabledStatic.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->disabledStatic.setWindowText(L"Disabled static");
   this->disabledStatic.setTypeface(defaultFont);
   this->disabledStatic.disable();

   this->enabledEdit.setStyle(WS_CHILD | WS_VISIBLE);
   this->enabledEdit.setPosition(100, 150);
   this->enabledEdit.setSize(100, 25);
   this->enabledEdit.setBorderSize(2);
   this->enabledEdit.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->enabledEdit.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->enabledEdit.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->enabledEdit.setCueText(L"Enabled edit");
   this->enabledEdit.setTypeface(defaultFont);
   this->enabledEdit.setShowCueOnFocus(true);

   this->disabledEdit.setStyle(WS_CHILD | WS_VISIBLE);
   this->disabledEdit.setPosition(300, 150);
   this->disabledEdit.setSize(100, 25);
   this->disabledEdit.setBorderSize(2);
   this->disabledEdit.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->disabledEdit.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->disabledEdit.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->disabledEdit.setWindowText(L"Disabled edit");
   this->disabledEdit.setTypeface(defaultFont);
   this->disabledEdit.disable();

   this->enabledPushButton.setStyle(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);
   this->enabledPushButton.setPosition(50, 200);
   this->enabledPushButton.setSize(200, 25);
   this->enabledPushButton.setBorderSize(2);
   this->enabledPushButton.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->enabledPushButton.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->enabledPushButton.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->enabledPushButton.setWindowText(L"Enabled push button");
   this->enabledPushButton.setTypeface(defaultFont);

   this->disabledPushButton.setStyle(WS_CHILD | WS_VISIBLE);
   this->disabledPushButton.setPosition(250, 200);
   this->disabledPushButton.setSize(200, 25);
   this->disabledPushButton.setBorderSize(2);
   this->disabledPushButton.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->disabledPushButton.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->disabledPushButton.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->disabledPushButton.setWindowText(L"Disabled push button");
   this->disabledPushButton.setTypeface(defaultFont);
   this->disabledPushButton.disable();

   this->enabledRadioButton.setStyle(WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON);
   this->enabledRadioButton.setPosition(50, 250);
   this->enabledRadioButton.setSize(200, 25);
   this->enabledRadioButton.setBorderSize(2);
   this->enabledRadioButton.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->enabledRadioButton.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->enabledRadioButton.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->enabledRadioButton.setWindowText(L"Enabled radio button");
   this->enabledRadioButton.setTypeface(defaultFont);

   this->disabledRadioButton.setStyle(WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON);
   this->disabledRadioButton.setPosition(250, 250);
   this->disabledRadioButton.setSize(200, 25);
   this->disabledRadioButton.setBorderSize(2);
   this->disabledRadioButton.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->disabledRadioButton.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->disabledRadioButton.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->disabledRadioButton.setWindowText(L"Disabled radio button");
   this->disabledRadioButton.setTypeface(defaultFont);
   this->disabledRadioButton.disable();

   this->enabledCheckButton.setStyle(WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);
   this->enabledCheckButton.setPosition(50, 300);
   this->enabledCheckButton.setSize(200, 25);
   this->enabledCheckButton.setBorderSize(2);
   this->enabledCheckButton.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->enabledCheckButton.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->enabledCheckButton.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->enabledCheckButton.setWindowText(L"Enabled check box");
   this->enabledCheckButton.setTypeface(defaultFont);

   this->disabledCheckButton.setStyle(WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX);
   this->disabledCheckButton.setPosition(250, 300);
   this->disabledCheckButton.setSize(200, 25);
   this->disabledCheckButton.setBorderSize(2);
   this->disabledCheckButton.setBGColor(0xFF, 0x7A, 0x0B, 0x7A);
   this->disabledCheckButton.setFGColor(0xFF, 0xBA, 0x5C, 0xBA);
   this->disabledCheckButton.setBorderColor(0xFF, 0x61, 0x00, 0x61);
   this->disabledCheckButton.setWindowText(L"Disabled check box");
   this->disabledCheckButton.setTypeface(defaultFont);
   this->disabledCheckButton.disable();

   this->trayIcon.setIcon(Icon::MainIcon());
   this->trayIcon.setInfoTitle(L"Sostav window test launched!");
   this->trayIcon.setInfo(L"This demonstrates the use of a tray icon and an info balloon.");
   this->trayIcon.setInfoFlags(NIIF_INFO);
}

LRESULT
WindowTest::onShowWindow
(BOOL show, WORD status)
{
   if (this->firstLaunch && show == TRUE)
   {
      LayeredImageWindow splashWindow;

      splashWindow.setStyle(WS_VISIBLE | WS_POPUP);
      splashWindow.setImage(Image(MAKEINTRESOURCE(IDI_SPLASH), L"PNG"));
      splashWindow.setBGColor(Color::Transparent());
      splashWindow.setAlpha(0xFF);
      splashWindow.setIcon(Icon::MainIcon());
      splashWindow.center();

      splashWindow.show();

      Sleep(3000);

      this->defWndProc(this->hwnd, WM_SHOWWINDOW, (WPARAM)show, (LPARAM)status);

      for (int i=255; i>0; i-=10)
      {
         splashWindow.setAlpha((BYTE)i);
         Sleep(10);
      }

      splashWindow.destroy();
      
      this->firstLaunch = false;

      return (LRESULT)0;
   }
   else
      return this->defWndProc(this->hwnd, WM_SHOWWINDOW, (WPARAM)show, (LPARAM)status);
}

LRESULT
WindowTest::onDestroy
(void)
{
   Window::onDestroy();

   ExitProcess(0);

   return (LRESULT)0;
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
