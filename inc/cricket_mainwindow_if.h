#pragma once

#include "settings.h"

class ICricketMainWindow
{
public:

  virtual ~ICricketMainWindow() = default;

  virtual bool is_slot_free(const ECricketSlots iSlot, uint32_t iPlayer) const = 0;
  virtual bool is_score_bigger(uint32_t iScore) const = 0;
  virtual bool is_score_smaller(uint32_t iScore) const = 0;
  virtual void increase_extra_points_of_other_players(const ECricketSlots iSlot, uint32_t iPoints) = 0;
  virtual QVector<uint32_t> compute_extra_points(const ECricketSlots iSlot, uint32_t iPoints, uint32_t iPlayer) = 0;
  virtual void set_scores() = 0;
  virtual void update_extra_points_labels() = 0;
  virtual void update_darts(uint32_t iPlayer) = 0;
};

//Q_DECLARE_INTERFACE(ICricketMainWindow, "com.ICricketMainWindow");
