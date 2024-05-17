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
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 501, 501, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, AlreadyFinishedTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  mDartBoard->set_finished();
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(20, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 501, 501, 3, {false, false, false}, {}, {0, 0, 0}}));
  EXPECT_TRUE(verify_dartboard_x01_warning(": Game already finished!"));
}

TEST_F(CDartBoardX01Test, StartValSingleInTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(489));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(489, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(12));
  mDartBoard->handle_segment_pressed_event(12, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 489, 501, 2, {false, false, false}, {"s12"}, {12, 0, 0}}));
}

TEST_F(CDartBoardX01Test, StartValDoubleInMissTest)
{
  mDartBoard->init_dartboard(301);
  set_x01_mode(EGame::GAME_301);
  set_in_mode(EX01InMode::DOUBLE_IN);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(301));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(301, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  mDartBoard->handle_segment_pressed_event(5, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 301, 301, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, StartValDoubleInHitTest)
{
  mDartBoard->init_dartboard(701);
  set_x01_mode(EGame::GAME_701);
  set_in_mode(EX01InMode::DOUBLE_IN);
  QChar type = 'd';
  EXPECT_CALL(*mMockWindow.get(), display_score(661));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(661, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(40));
  mDartBoard->handle_segment_pressed_event(40, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 661, 701, 2, {false, false, false}, {"d40"}, {40, 0, 0}}));
}

TEST_F(CDartBoardX01Test, StartValMasterInMissTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  set_in_mode(EX01InMode::MASTER_IN);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(501, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  mDartBoard->handle_segment_pressed_event(20, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 501, 501, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, StartValMasterInHitTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  set_in_mode(EX01InMode::MASTER_IN);
  QChar type = 't';
  EXPECT_CALL(*mMockWindow.get(), display_score(447));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(447, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(54));
  mDartBoard->handle_segment_pressed_event(54, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 447, 501, 2, {false, false, false}, {"t54"}, {54, 0, 0}}));
}

TEST_F(CDartBoardX01Test, ScoreInRangeTest)
{
  mDartBoard->init_dartboard(355);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(335));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(335, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(20));
  mDartBoard->handle_segment_pressed_event(20, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 335, 355, 2, {false, false, false}, {"s20"}, {20, 0, 0}}));
}

TEST_F(CDartBoardX01Test, ScoreInRange3DartsTest)
{
  mDartBoard->init_dartboard(212);
  set_x01_mode(EGame::GAME_501);
  QChar type = 't';
  EXPECT_CALL(*mMockWindow.get(), display_score(152));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(152, 2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(60));
  mDartBoard->handle_segment_pressed_event(60, type);

  EXPECT_CALL(*mMockWindow.get(), display_score(92));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(92, 1));
  EXPECT_CALL(*mMockWindow.get(), display_dart2(60));
  mDartBoard->handle_segment_pressed_event(60, type);

  EXPECT_CALL(*mMockWindow.get(), display_score(32));
  EXPECT_CALL(*mMockWindow.get(), update_finishes(32, 0));
  EXPECT_CALL(*mMockWindow.get(), display_dart3(60));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(60, type);
  EXPECT_TRUE(verify_dartboard_x01({true, false, 32, 212, 0, {false, false, false}, {"t60", "t60", "t60"}, {60, 60, 60}}));
}

TEST_F(CDartBoardX01Test, SingleOutGameShotTest)
{
  mDartBoard->init_dartboard(11);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::SINGLE_OUT);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(0));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(11));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(11, type);
  EXPECT_TRUE(verify_dartboard_x01({true, false, 0, 11, 2, {true, false, false}, {"s11"}, {11, 0, 0}}));
}

TEST_F(CDartBoardX01Test, DoubleOutCorrectScoreButSingleTest)
{
  mDartBoard->init_dartboard(20);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::DOUBLE_OUT);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(20));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(20, type);
  EXPECT_TRUE(verify_dartboard_x01({true, true, 20, 20, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}
