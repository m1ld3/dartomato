#include "fixtures.h"

TEST_F(CDartBoardX01Test, InitTest)
{
  EXPECT_CALL(*mMockWindow.get(), erase_dart1());
  EXPECT_CALL(*mMockWindow.get(), erase_dart2());
  EXPECT_CALL(*mMockWindow.get(), erase_dart3());
  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  mDartBoard->init_dartboard(501);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 501, 501, 3, {false, false, false}, {}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, NoScoreAtBeginTest)
{
  mDartBoard->init_dartboard(501);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 501, 501, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));
}
