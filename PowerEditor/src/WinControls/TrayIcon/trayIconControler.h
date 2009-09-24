#ifndef TRAY_ICON_CONTROLER_H
#define TRAY_ICON_CONTROLER_H


#define ADD     NIM_ADD
#define REMOVE  NIM_DELETE

// Error codes
#define INCORRECT_OPERATION     1
#define OPERATION_INCOHERENT    2

class trayIconControler
{
public:
  trayIconControler(HWND hwnd, UINT uID, UINT uCBMsg, HICON hicon, TCHAR *tip);
  int doTrayIcon(DWORD op);
  bool isInTray() const {return _isIconShowed;};

private:
  NOTIFYICONDATA    _nid;
  bool              _isIconShowed;
};

#endif //TRAY_ICON_CONTROLER_H
