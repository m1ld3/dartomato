#pragma once

#include "settings.h"

class ICricketMainWindow
{
public:

  virtual ~ICricketMainWindow() = default;

  [[nodiscard]] virtual bool is_slot_free(ECricketSlots iSlot, uint32_t iPlayer) const = 0;
  [[nodiscard]] virtual bool is_score_bigger(uint32_t iScore) const = 0;
  [[nodiscard]] virtual bool is_score_smaller(uint32_t iScore) const = 0;
  virtual void increase_extra_points_of_other_players(ECricketSlots iSlot, uint32_t iPoints) = 0;
  virtual QVector<uint32_t> compute_extra_points(ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer) = 0;
  virtual void set_scores() = 0;
  virtual void update_extra_points_labels() = 0;
  virtual void update_darts(uint32_t iPlayer) = 0;
};

//Q_DECLARE_INTERFACE(ICricketMainWindow, "com.ICricketMainWindow");
