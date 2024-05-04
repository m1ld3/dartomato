#pragma once

#include <QObject>
#include "settings.h"

class IX01MainWindow
{
public:

  virtual ~IX01MainWindow() = default;

  virtual void display_dart1(uint32_t iVal) = 0;
  virtual void display_dart2(uint32_t iVal) = 0;
  virtual void display_dart3(uint32_t iVal) = 0;
  virtual void erase_dart1() = 0;
  virtual void erase_dart2() = 0;
  virtual void erase_dart3() = 0;
  virtual void submit_score_to_player(uint32_t iScore, uint32_t iNumberOfDarts, uint32_t iCheckoutAttempts, const QVector<QString> & iDarts) = 0;
  virtual void update_finishes(uint32_t iScore, uint32_t iNumberOfDarts) = 0;
  virtual void set_focus_to_submit_button() = 0;
  virtual void display_score(uint32_t iScore) = 0;

protected:

  virtual void connect_main_window_slots() = 0;

protected slots:

  virtual void submit_button_clicked_slot() = 0;
  virtual void undo_button_clicked_slot() = 0;
};

Q_DECLARE_INTERFACE(IX01MainWindow, "com.IX01MainWindow")
