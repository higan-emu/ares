#if defined(Hiro_Button)

namespace hiro {

auto pButton::construct() -> void {
  hwnd = CreateWindow(
    L"BUTTON", L"", WS_CHILD | WS_TABSTOP,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  _setState();
}

auto pButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pButton::minimumSize() const -> Size {
  Size icon = {(s32)state().icon.width(), (s32)state().icon.height()};
  Size text = state().text ? pFont::size(self().font(true), state().text) : Size{};
  Size size;
  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(icon.width() + (icon && text ? 5 : 0) + text.width());
    size.setHeight(max(icon.height(), text.height()));
  }
  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(icon.width(), text.width()));
    size.setHeight(icon.height() + (icon && text ? 5 : 0) + text.height());
  }
  size.setHeight(max(size.height(), pFont::size(self().font(true), " ").height()));
  return {size.width() + (state().bordered && text ? 20 : 10), size.height() + 10};
}

auto pButton::setBordered(bool bordered) -> void {
  _setState();
}

auto pButton::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  _setState();
}

auto pButton::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  _setState();
}

auto pButton::setIcon(const image& icon) -> void {
  _setState();
}

auto pButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

auto pButton::setText(const string& text) -> void {
  _setState();
}

auto pButton::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  _setState();
}

//

auto pButton::onActivate() -> void {
  self().doActivate();
}

//BUTTON cannot draw borderless buttons on its own
//BS_OWNERDRAW will send WM_DRAWITEM; but will disable hot-tracking notifications
//to gain hot-tracking plus borderless buttons; BUTTON is superclassed and WM_PAINT is hijacked
//note: letting hiro paint bordered buttons will lose the fade animations on Vista+;
//however, it will allow placing icons immediately next to text (original forces icon left alignment)
auto pButton::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_KEYDOWN) {
    //very useful for MessageDialog
    self().doActivate();
  }

  if(msg == WM_PAINT) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    auto buttonState = Button_GetState(hwnd);
    Button_CustomDraw(hwnd, ps,
      state().bordered, false, self().enabled(true), buttonState,
      self().font(true), state().icon, state().orientation, state().text
    );
    EndPaint(hwnd, &ps);
    return false;
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

//

auto pButton::_setState() -> void {
  InvalidateRect(hwnd, 0, false);
}

//this function is designed to be used with Button, CheckButton, and RadioButton
auto Button_CustomDraw(HWND hwnd, PAINTSTRUCT& ps, bool bordered, bool checked, bool enabled, u32 state, const Font& font, const image& icon, Orientation orientation, const string& text) -> void {
  RECT rc;
  GetClientRect(hwnd, &rc);
  Geometry geometry{rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top}, iconGeometry, textGeometry;
  if(icon) iconGeometry.setSize({(s32)icon.width(), (s32)icon.height()});
  if(text) textGeometry.setSize(pFont::size(font, text));

  Position position;
  Size size;

  switch(orientation) {
  case Orientation::Horizontal:
    size = {iconGeometry.width() + (icon && text ? 5 : 0) + textGeometry.width(), max(iconGeometry.height(), textGeometry.height())};
    position = {(geometry.width() - size.width()) / 2, (geometry.height() - size.height()) / 2};
    iconGeometry.setPosition({position.x(), position.y() + (size.height() - iconGeometry.height()) / 2});
    textGeometry.setPosition({position.x() + size.width() - textGeometry.width(), position.y() + (size.height() - textGeometry.height()) / 2});
    break;
  case Orientation::Vertical:
    size = {max(iconGeometry.width(), textGeometry.width()), iconGeometry.height() + (icon && text ? 5 : 0) + textGeometry.height()};
    position = {(geometry.width() - size.width()) / 2, (geometry.height() - size.height()) / 2};
    iconGeometry.setPosition({position.x() + (size.width() - iconGeometry.width()) / 2, position.y()});
    textGeometry.setPosition({position.x() + (size.width() - textGeometry.width()) / 2, position.y() + size.height() - textGeometry.height()});
    break;
  }

  if(auto theme = OpenThemeData(hwnd, L"BUTTON")) {
    DrawThemeParentBackground(hwnd, ps.hdc, &rc);
    u32 flags = 0;
    if(state & BST_PUSHED || checked) flags = PBS_PRESSED;
    else if(state & BST_HOT) flags = PBS_HOT;
    else if(bordered) flags = enabled ? PBS_NORMAL : PBS_DISABLED;
    if(bordered || flags) DrawThemeBackground(theme, ps.hdc, BP_PUSHBUTTON, flags, &rc, &ps.rcPaint);
    CloseThemeData(theme);
  } else {
    //Windows Classic
    FillRect(ps.hdc, &rc, GetSysColorBrush(COLOR_3DFACE));
    u32 flags = (state & BST_PUSHED || checked) ? DFCS_PUSHED : 0;
    if(bordered || flags) DrawFrameControl(ps.hdc, &rc, DFC_BUTTON, DFCS_BUTTONPUSH | flags | (enabled ? 0 : DFCS_INACTIVE));
  }

  if(GetFocus() == hwnd) {
    s32 offset = state ? 4 : 1;
    RECT rcFocus{rc.left + offset, rc.top + offset, rc.right - offset, rc.bottom - offset};
    if(!(state & BST_PUSHED) && !(state & BST_HOT)) DrawFocusRect(ps.hdc, &rcFocus);
  }

  if(icon) {
    HDC hdcSource = CreateCompatibleDC(ps.hdc);
    auto bitmap = CreateBitmap(icon);
    SelectBitmap(hdcSource, bitmap);
    BLENDFUNCTION blend{AC_SRC_OVER, 0, (BYTE)(IsWindowEnabled(hwnd) ? 255 : 128), AC_SRC_ALPHA};
    AlphaBlend(
      ps.hdc, iconGeometry.x(), iconGeometry.y(), icon.width(), icon.height(),
      hdcSource, 0, 0, icon.width(), icon.height(), blend
    );
    DeleteObject(bitmap);
    DeleteDC(hdcSource);
  }

  if(text) {
    utf16_t wText(text);
    SetBkMode(ps.hdc, TRANSPARENT);
    SetTextColor(ps.hdc, GetSysColor(IsWindowEnabled(hwnd) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
    auto hFont = pFont::create(font);
    SelectObject(ps.hdc, hFont);
    RECT rcText{
      LONG(textGeometry.x()),
      LONG(textGeometry.y()),
      LONG(textGeometry.x() + textGeometry.width()),
      LONG(textGeometry.y() + textGeometry.height())
    };
    DrawText(ps.hdc, wText, -1, &rcText, DT_NOPREFIX | DT_END_ELLIPSIS);
    DeleteObject(hFont);
  }
}

}

#endif
