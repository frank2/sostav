#include "main.hpp"

using namespace Sostav;
using namespace Sostav::Chiptunes;
using namespace Sostav::Drawing;
using namespace Sostav::Win32;
using namespace Sostav::Windows;

ChiptuneButton::ChiptuneButton
(Window *parent, MikModModule module)
   : Button(parent)
{
   this->setModule(module);
}

ChiptuneButton::ChiptuneButton
(void)
   : Button()
{
}

void
ChiptuneButton::setModule
(MikModModule module)
{
   this->module.setBuffer(module.getBuffer(), module.getBufferSize());
}

LRESULT
ChiptuneButton::onLButtonUp
(WORD virtualKeys, WORD x, WORD y)
{
   ChiptuneSelector *selector = (ChiptuneSelector *)this->parent;

   selector->stop();

   if (this->module.getBuffer() == NULL)
      selector->setModule(NULL);
   else
      selector->setModule(&this->module);
   
   selector->play();

   return (LRESULT)0;
}

ChiptuneSelector::ChiptuneSelector
(Window *parent)
   : Button(parent)
{
   this->module = NULL;
}

ChiptuneSelector::ChiptuneSelector
(void)
   : Button()
{
   this->module = NULL;
}

void
ChiptuneSelector::setModule
(MikModModule *module)
{
   this->module = module;
}

void
ChiptuneSelector::play
(void)
{
   if (this->module != NULL)
   {
      this->module->play();
      this->songTitle.setWindowText(this->module->getSongName());
   }
   else
      this->songTitle.setWindowText(L"music stopped");
}

void
ChiptuneSelector::stop
(void)
{
   if (this->module != NULL && this->module->isPlaying())
      this->module->stop();
}

void
ChiptuneSelector::preCreate
(void)
{
   double relativeWidth, relativeHeight;
   DWORD absoluteWidth, absoluteHeight;
   DWORD buttonStyle = WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;

   Button::preCreate();
   
   this->addStyle(BS_GROUPBOX);

   relativeWidth = 0.90 / 5.0;
   relativeHeight = 0.10;

   absoluteWidth = this->size.cx * relativeWidth;
   absoluteHeight = this->size.cy * relativeHeight;
   
   this->impulseTracker.setParent(this);
   this->moduleButton.setParent(this);
   this->screamTracker3.setParent(this);
   this->fastTracker2.setParent(this);
   this->noMusic.setParent(this);
   this->songTitle.setParent(this);

   this->impulseTracker.setWindowText(L"IT");
   this->impulseTracker.setTypeface(Font::MessageFont());
   this->impulseTracker.setStyle(buttonStyle);
   this->impulseTracker.setRelativePosition(0.05, 0.80);
   this->impulseTracker.setSize(absoluteWidth, absoluteHeight);
   this->impulseTracker.setModule(MikModModule(MAKEINTRESOURCE(IDI_BEYOND), L"ITMOD"));

   this->moduleButton.setWindowText(L"MOD");
   this->moduleButton.setTypeface(Font::MessageFont());
   this->moduleButton.setStyle(buttonStyle);
   this->moduleButton.setRelativePosition(0.05 + relativeWidth, 0.80);
   this->moduleButton.setSize(absoluteWidth, absoluteHeight);
   this->moduleButton.setModule(MikModModule(MAKEINTRESOURCE(IDI_SPACE), L"MOD"));

   this->screamTracker3.setWindowText(L"S3M");
   this->screamTracker3.setTypeface(Font::MessageFont());
   this->screamTracker3.setStyle(buttonStyle);
   this->screamTracker3.setRelativePosition(0.05 + relativeWidth * 2, 0.80);
   this->screamTracker3.setSize(absoluteWidth, absoluteHeight);
   this->screamTracker3.setModule(MikModModule(MAKEINTRESOURCE(IDI_UNREAL), L"S3MOD"));

   this->fastTracker2.setWindowText(L"XM");
   this->fastTracker2.setTypeface(Font::MessageFont());
   this->fastTracker2.setStyle(buttonStyle);
   this->fastTracker2.setRelativePosition(0.05 + relativeWidth * 3, 0.80);
   this->fastTracker2.setSize(absoluteWidth, absoluteHeight);
   this->fastTracker2.setModule(MikModModule(MAKEINTRESOURCE(IDI_DEADLOCK), L"XMMOD"));
   
   this->noMusic.setWindowText(L"stop");
   this->noMusic.setTypeface(Font::MessageFont());
   this->noMusic.setStyle(buttonStyle);
   this->noMusic.setRelativePosition(0.05 + relativeWidth * 4, 0.80);
   this->noMusic.setSize(absoluteWidth, absoluteHeight);

   this->songTitle.setStyle(WS_CHILD | WS_VISIBLE | SS_CENTER);
   this->songTitle.setTypeface(Font::MessageFont());
   this->songTitle.setBGColor(Color::Transparent());
   this->songTitle.setFGColor(0xFF, 0x6A, 0xCE, 0xCB);
   this->songTitle.setRelativePosition(0.05, 0.2);
   this->songTitle.setSize(this->size.cx * 0.90, absoluteHeight);
}
   
MainWindow::MainWindow
(void)
   : Window()
{
   this->setClassName(L"SostavTestMainWindow");
}

void
MainWindow::preCreate
(void)
{
   Window::preCreate();
   
   this->setIcon(Icon::MainIcon());
   this->setBGColor(0xFF, 0x00, 0x71, 0x6D);
   this->addStyle(WS_CAPTION | WS_SYSMENU);
   this->setSize(500, 500);
   this->center();

   this->sostavBanner.setParent(this);
   this->chiptunes.setParent(this);

   this->sostavBanner.setStyle(WS_CHILD | WS_VISIBLE);
   this->sostavBanner.setImage(Image(MAKEINTRESOURCE(IDI_SOSTAV), L"PNG"));
   this->sostavBanner.setPosition(10, 10);
   this->sostavBanner.setBGColor(Color::Transparent());
   this->sostavBanner.centerX();

   this->chiptunes.setStyle(WS_CHILD | WS_VISIBLE);
   this->chiptunes.setTypeface(Font::MessageFont());
   this->chiptunes.setPosition(10, 100);
   this->chiptunes.setFGColor(0xFF, 0x6A, 0xCE, 0xCB);
   this->chiptunes.setBGColor(Color::Transparent());
   this->chiptunes.setSize(300, 200);
   this->chiptunes.center();
}

LRESULT
MainWindow::onShowWindow
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
MainWindow::onDestroy
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
   MainWindow mainWindow;
      
   mainWindow.show();

   Window::MessageLoop();

   return 0;
}
