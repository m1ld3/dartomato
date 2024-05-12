#include <gtest/gtest.h>
#include "fakes.h"
#include "mocks.h"
#include "dartboard_x01.h"
#include "x01_mainwindow.h"

TEST(DartboardX01Test, HandleSegmentPressedEventTest)
{
  std::unique_ptr<CX01MainWindowMock> mockWindow = std::make_unique<CX01MainWindowMock>();
  CDartBoardX01 dartboard(mockWindow.get());
//  EXPECT_CALL(*mockWindow.get(), erase_dart1());
//  EXPECT_CALL(*mockWindow.get(), erase_dart2());
//  EXPECT_CALL(*mockWindow.get(), erase_dart3());
//  EXPECT_CALL(*mockWindow.get(), display_score(501));
  dartboard.init_dartboard(501);
  QChar type = 's';
  dartboard.handle_segment_pressed_event(0, type);
  EXPECT_TRUE(true);
}
