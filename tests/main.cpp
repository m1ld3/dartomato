#include "fixtures.h"

TEST_F(CDartBoardX01Test, InitTest)
{
  EXPECT_CALL(*mMockWindow.get(), erase_dart1());
  EXPECT_CALL(*mMockWindow.get(), erase_dart2());
  EXPECT_CALL(*mMockWindow.get(), erase_dart3());
  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  mDartBoard->init_dartboard(501);
  QChar type = 's';
//  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(true);
}

TEST_F(CDartBoardX01Test, NoScoreTestAtBegin)
{
  mDartBoard->init_dartboard(501);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(true);
}
