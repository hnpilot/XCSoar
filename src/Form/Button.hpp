/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009

	M Roberts (original release)
	Robin Birch <robinb@ruffnready.co.uk>
	Samuel Gisiger <samuel.gisiger@triadis.ch>
	Jeff Goodenough <jeff@enborne.f2s.com>
	Alastair Harrison <aharrison@magic.force9.co.uk>
	Scott Penrose <scottp@dd.com.au>
	John Wharington <jwharington@gmail.com>
	Lars H <lars_hn@hotmail.com>
	Rob Dunning <rob@raspberryridgesheepfarm.com>
	Russell King <rmk@arm.linux.org.uk>
	Paolo Ventafridda <coolwind@email.it>
	Tobias Lohner <tobias@lohner-net.de>
	Mirek Jezek <mjezek@ipplc.cz>
	Max Kellermann <max@duempel.org>
	Tobias Bieniek <tobias.bieniek@gmx.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef XCSOAR_FORM_BUTTON_HPP
#define XCSOAR_FORM_BUTTON_HPP

#include "Screen/PaintWindow.hpp"

class ContainerWindow;

/**
 * This class is used for creating buttons.
 * It is based on the WindowControl class.
 */
class WndButton : public PaintWindow {
public:
  typedef void (*ClickNotifyCallback_t)(WndButton &button);

protected:
  Color text_color;
  Brush background_brush;
  TCHAR mCaption[254];

public:
  /**
   * Constructor of the WndButton class
   * @param Parent Parent window/ContainerControl
   * @param Caption Text on the button
   * @param X x-Coordinate relative to the parent
   * @param Y y-Coordinate relative to the parent
   * @param Width Width of the button
   * @param Height Height of the button
   * @param Function The function that should be called
   * when the button is clicked
   */
  WndButton(ContainerWindow &parent, const TCHAR *Caption,
      int X, int Y, int Width, int Height,
            const WindowStyle style,
            Color background_color,
      ClickNotifyCallback_t Function = NULL);

  /**
   * Sets the function that should be called when the button is pressed
   * @param Function Pointer to the function to be called
   */
  void
  SetOnClickNotify(ClickNotifyCallback_t Function)
  {
    mOnClickNotify = Function;
  }

  /**
   * Sets the Caption/Text of the Control and resets the cached text height
   * (derived from WindowControl)
   * @param Value The new Caption/Text of the Control
   */
  void SetCaption(const TCHAR *Value);

  void SetForeColor(Color color) {
    text_color = color;
    invalidate();
  }

protected:
  /**
   * Called when the button is clicked (either by mouse or by
   * keyboard).  The default implementation invokes the OnClick
   * callback.
   */
  virtual void on_click();

  virtual bool on_setfocus();
  virtual bool on_killfocus();

  /**
   * The on_mouse_up event is called when the mouse is released over the button
   * (derived from Window)
   */
  virtual bool on_mouse_up(int x, int y);
  /**
   * The on_mouse_down event is called when the mouse is pressed over the button
   * (derived from Window)
   */
  virtual bool on_mouse_down(int x, int y);
  /**
   * The on_mouse_move event is called when the mouse is moved over the button
   * (derived from Window)
   */
  virtual bool on_mouse_move(int x, int y, unsigned keys);

  virtual bool on_key_check(unsigned key_code);

  /**
   * The on_key_down event is called when a key is pressed while the
   * button is focused
   * (derived from Window)
   */
  virtual bool on_key_down(unsigned key_code);
  /**
   * The on_key_down event is called when a key is released while the
   * button is focused
   * (derived from Window)
   */
  virtual bool on_key_up(unsigned key_code);

  /**
   * The on_paint event is called when the button needs to be drawn
   * (derived from PaintWindow)
   */
  virtual void on_paint(Canvas &canvas);

  /** True if the button is currently pressed */
  bool mDown;

private:
  /**
   * Height of the drawn caption
   * (this is used to speed up the drawing process)
   */
  int mLastDrawTextHeight;

  /**
   * The callback-function that should be called when the button is pressed
   * @see SetOnClickNotify()
   */
  ClickNotifyCallback_t mOnClickNotify;
};

#endif