// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GTK_GO_BUTTON_GTK_H_
#define CHROME_BROWSER_GTK_GO_BUTTON_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/task.h"
#include "chrome/browser/gtk/custom_button.h"
#include "chrome/common/owned_widget_gtk.h"

class Browser;
class Task;

class GoButtonGtk {
 public:
  enum Mode { MODE_GO = 0, MODE_STOP };
  enum ButtonState { BS_NORMAL = 0, BS_HOT };

  explicit GoButtonGtk(Browser* browser);
  ~GoButtonGtk();

  GtkWidget* widget() const { return widget_.get(); }
  ButtonState state() const { return state_; }

  // Force the button state.  Useful for when the foreground tab changes.
  void ChangeMode(Mode mode);

  // Ask for a specified button state.  This is called when the loading state of
  // the tab changes.  This method may postpone the actual ChangeMode() call
  // until another event (such as waiting for a potential double click to end,
  // or for the mouse to stop hovering over the button).
  void ScheduleChangeMode(Mode mode);

 private:
  friend class GoButtonGtkPeer;

  // gtk signals
  static gboolean OnExpose(GtkWidget* widget,
                           GdkEventExpose* e,
                           GoButtonGtk* button);
  static gboolean OnEnter(GtkButton* widget, GoButtonGtk* button);
  static gboolean OnLeave(GtkButton* widget, GoButtonGtk* button);
  static gboolean OnClicked(GtkButton* widget, GoButtonGtk* button);

  Task* CreateButtonTimerTask();
  void OnButtonTimer();

  // Keep a pointer to the Browser object to execute commands on it.
  Browser* const browser_;

  // Delay time to wait before allowing a mode change.  This is to prevent a
  // mode switch while the user is double clicking.
  int button_delay_;
  ScopedRunnableMethodFactory<GoButtonGtk> stop_timer_;

  // The mode we should be in
  Mode intended_mode_;

  // The currently-visible mode - this may different from the intended mode
  Mode visible_mode_;

  ButtonState state_;

  CustomDrawButtonBase go_;
  CustomDrawButtonBase stop_;

  OwnedWidgetGtk widget_;

  DISALLOW_COPY_AND_ASSIGN(GoButtonGtk);
};

#endif  // CHROME_BROWSER_GTK_GO_BUTTON_GTK_H_
