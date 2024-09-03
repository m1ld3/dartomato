#include "fixtures.h"
#include <initializer_list>

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

TEST_F(CDartBoardX01Test, DoubleOutCorrectScoreButNoDoubleTest)
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

TEST_F(CDartBoardX01Test, DoubleOutGameShotTest)
{
  mDartBoard->init_dartboard(32);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::DOUBLE_OUT);
  QChar type = 'd';
  EXPECT_CALL(*mMockWindow.get(), display_score(0));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(32));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(32, type);
  EXPECT_TRUE(verify_dartboard_x01({true, false, 0, 32, 2, {true, false, false}, {"d32"}, {32, 0, 0}}));
}

TEST_F(CDartBoardX01Test, MasterOutCorrectScoreButNoTripleTest)
{
  mDartBoard->init_dartboard(60);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::MASTER_OUT);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(60));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(60, type);
  EXPECT_TRUE(verify_dartboard_x01({true, true, 60, 60, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, MasterOutGameShotTest)
{
  mDartBoard->init_dartboard(33);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::MASTER_OUT);
  QChar type = 't';
  EXPECT_CALL(*mMockWindow.get(), display_score(0));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(33));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(33, type);
  EXPECT_TRUE(verify_dartboard_x01({true, false, 0, 33, 2, {true, false, false}, {"t33"}, {33, 0, 0}}));
}

TEST_F(CDartBoardX01Test, NormalBustedTest)
{
  mDartBoard->init_dartboard(7);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(7));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(19, type);
  EXPECT_TRUE(verify_dartboard_x01({true, true, 7, 7, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, DoubleOutBustedTest)
{
  mDartBoard->init_dartboard(2);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::DOUBLE_OUT);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(2));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(1, type);
  EXPECT_TRUE(verify_dartboard_x01({true, true, 2, 2, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, MasterOutBustedTest)
{
  mDartBoard->init_dartboard(3);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::MASTER_OUT);
  QChar type = 's';
  EXPECT_CALL(*mMockWindow.get(), display_score(3));
  EXPECT_CALL(*mMockWindow.get(), display_dart1(0));
  EXPECT_CALL(*mMockWindow.get(), set_focus_to_submit_button);
  mDartBoard->handle_segment_pressed_event(1, type);
  EXPECT_TRUE(verify_dartboard_x01({true, true, 3, 3, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, AlreadyBustedTest)
{
  mDartBoard->init_dartboard(16);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(17, type);
  mDartBoard->handle_segment_pressed_event(15, type);
  EXPECT_TRUE(verify_dartboard_x01_warning("Warning: You are already busted!"));
  EXPECT_TRUE(verify_dartboard_x01({true, true, 16, 16, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, TooManyDartsTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::DOUBLE_OUT);
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(20, type);
  mDartBoard->handle_segment_pressed_event(5, type);
  mDartBoard->handle_segment_pressed_event(1, type);
  mDartBoard->handle_segment_pressed_event(25, type);
  EXPECT_TRUE(verify_dartboard_x01_warning("Warning: You only have three darts!"));
  EXPECT_TRUE(verify_dartboard_x01({true, false, 475, 501, 0, {false, false, false}, {"s20", "s5", "s1"}, {20, 5, 1}}));
}

TEST_F(CDartBoardX01Test, SingleOutCheckoutTests)
{
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::SINGLE_OUT);
  QChar type = 's';

  mDartBoard->init_dartboard(1);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 1, 1, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(61);
  mDartBoard->handle_segment_pressed_event(10, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 51, 61, 2, {false, false, false}, {"s10"}, {10, 0, 0}}));

  mDartBoard->init_dartboard(52);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 52, 52, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(45);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 45, 45, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, DoubleOutCheckoutTests)
{
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::DOUBLE_OUT);
  QChar type = 's';

  mDartBoard->init_dartboard(2);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 2, 2, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(50);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 50, 50, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(42);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 42, 42, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(32);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 32, 32, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, MasterOutCheckoutTests)
{
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::MASTER_OUT);
  QChar type = 's';

  mDartBoard->init_dartboard(3);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 3, 3, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(61);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 61, 61, 2, {false, false, false}, {"s0"}, {0, 0, 0}}));

  mDartBoard->init_dartboard(42);
  mDartBoard->handle_segment_pressed_event(0, type);
  EXPECT_TRUE(verify_dartboard_x01({false, false, 42, 42, 2, {true, false, false}, {"s0"}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, UndoTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(1, type);
  mDartBoard->handle_segment_pressed_event(2, type);
  mDartBoard->handle_segment_pressed_event(3, type);

  EXPECT_CALL(*mMockWindow.get(), display_score(498));
  EXPECT_CALL(*mMockWindow.get(), erase_dart3());
  EXPECT_CALL(*mMockWindow.get(), update_finishes(498, 1));
  mDartBoard->perform_undo();
  EXPECT_TRUE(verify_dartboard_x01({false, false, 498, 501, 1, {false, false, false}, {"s1", "s2"}, {1, 2, 0}}));

  EXPECT_CALL(*mMockWindow.get(), display_score(500));
  EXPECT_CALL(*mMockWindow.get(), erase_dart2());
  EXPECT_CALL(*mMockWindow.get(), update_finishes(500, 2));
  mDartBoard->perform_undo();
  EXPECT_TRUE(verify_dartboard_x01({false, false, 500, 501, 2, {false, false, false}, {"s1"}, {1, 0, 0}}));

  EXPECT_CALL(*mMockWindow.get(), display_score(501));
  EXPECT_CALL(*mMockWindow.get(), erase_dart1());
  EXPECT_CALL(*mMockWindow.get(), update_finishes(501, 3));
  mDartBoard->perform_undo();
  EXPECT_TRUE(verify_dartboard_x01({false, false, 501, 501, 3, {false, false, false}, {}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, UndoBustedTest)
{
  mDartBoard->init_dartboard(2);
  set_x01_mode(EGame::GAME_501);
  set_out_mode(EX01OutMode::DOUBLE_OUT);
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(1, type);
  mDartBoard->perform_undo();
  EXPECT_TRUE(verify_dartboard_x01({false, false, 2, 2, 3, {false, false, false}, {}, {0, 0, 0}}));
}

TEST_F(CDartBoardX01Test, SubmitScoreIncompleteTest)
{
  mDartBoard->init_dartboard(123);
  set_x01_mode(EGame::GAME_501);
  mDartBoard->submit_score();
  EXPECT_TRUE(verify_dartboard_x01_warning("Score incomplete: Please enter all darts."));
}

TEST_F(CDartBoardX01Test, SubmitScoreAlreadyFinishedTest)
{
  mDartBoard->init_dartboard(0);
  set_x01_mode(EGame::GAME_501);
  mDartBoard->set_finished();
  mDartBoard->submit_score();
  EXPECT_TRUE(verify_dartboard_x01_warning(": Game already finished!"));
}

TEST_F(CDartBoardX01Test, SubmitScoreTest)
{
  mDartBoard->init_dartboard(501);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(20, type);
  mDartBoard->handle_segment_pressed_event(19, type);
  mDartBoard->handle_segment_pressed_event(18, type);
  QVector<QString> darts = {"s20", "s19", "s18"};
  EXPECT_CALL(*mMockWindow.get(), submit_score_to_player(57, 3, 0, darts));
  mDartBoard->submit_score();
}

TEST_F(CDartBoardX01Test, SubmitScoreBustedTest)
{
  mDartBoard->init_dartboard(10);
  set_x01_mode(EGame::GAME_501);
  QChar type = 's';
  mDartBoard->handle_segment_pressed_event(20, type);
  QVector<QString> darts = {"s0", "s0", "s0"};
  EXPECT_CALL(*mMockWindow.get(), submit_score_to_player(0, 3, 1, darts));
  mDartBoard->submit_score();
}

TEST_F(CX01GroupBoxTest, SubmitScoreDefaultTest)
{
  EXPECT_CALL(*mMockWindow.get(), update_players(EUpdateType::DEFAULT));
  CX01Class::CPlayerData expected = {0, 0, 0, 275, 0, 0, 3, 26.0 / 3.0, 26, 0, 26.0 / 3.0, {26}, {}, {26}, QVector<QVector<QString>>({{"s20", "s1", "s5"}}), QVector<QVector<QString>>({{"s20", "s1", "s5"}}), QVector<QVector<QVector<QString>>>({}), {301, 275}, {}, false};
  mBox->submit_score(26, 3, 0, {"s20", "s1", "s5"});
  EXPECT_TRUE(verify_score_sound_path(26));
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CX01GroupBoxTest, SubmitScoreTwiceTest)
{
  EXPECT_CALL(*mMockWindow.get(), update_players(EUpdateType::DEFAULT)).Times(2);
  CX01Class::CPlayerData expected = {0, 0, 0, 144, 0, 0, 6, 157.0 / 6.0, 157.0 / 2.0, 0, 157.0 / 3.0, {100, 57}, {}, {100, 57}, QVector<QVector<QString>>({{"s20", "t20", "s20"}, {"s20", "s20", "s19"}}), QVector<QVector<QString>>({{"s20", "t20", "s20"}, {"s20", "s20", "s19"}}), QVector<QVector<QVector<QString>>>({}), {301, 201, 144}, {}, false};
  mBox->submit_score(100, 3, 0, {"s20", "t20", "s20"});
  EXPECT_TRUE(verify_score_sound_path(100));
  mBox->submit_score(57, 3, 0, {"s20", "s20", "s19"});
  EXPECT_TRUE(verify_score_sound_path(57));
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CX01GroupBoxTest, SubmitScoreCheckoutAttemptTest)
{
  QVector<CX01Class::CPlayerData> gameData;
  CX01Class::CPlayerData snap1 = {0, 0, 0, 121, 0, 0, 3, 180.0 / 3.0, 180, 0, 180.0 / 3.0, {180}, {}, {180}, QVector<QVector<QString>>({{"t20", "t20", "t20"}}), QVector<QVector<QString>>({{"t20", "t20", "t20"}}), QVector<QVector<QVector<QString>>>({}), {301, 121}, {}, false};
  CX01Class::CPlayerData snap2 = {0, 0, 0, 16, 0, 0, 6, 285.0 / 6.0, 285.0 / 2.0, 0, 285.0 / 3.0, {180, 105}, {}, {180, 105}, QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}}), QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}}), QVector<QVector<QVector<QString>>>({}), {301, 121, 16}, {}, true};
  CX01Class::CPlayerData expected = {0, 0, 0, 8, 3, 0, 9, 293.0 / 9.0, 293.0 / 3.0, 0, 293.0 / 3.0, {180, 105, 8}, {}, {180, 105, 8}, QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}, {"s0", "s8", "s0"}}), QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}, {"s0", "s8", "s0"}}), QVector<QVector<QVector<QString>>>({}), {301, 121, 16, 8}, {}, true};
  gameData = {snap1, snap2};
  EXPECT_CALL(*mDb.get(), init_dartboard(16));
  mBox->set_game_data(gameData);
  mBox->submit_score(8, 3, 3, {"s0", "s8", "s0"});
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CX01GroupBoxTest, SubmitScoreCheckoutHitTest)
{
  QVector<CX01Class::CPlayerData> gameData;
  CX01Class::CPlayerData snap1 = {0, 0, 0, 121, 0, 0, 3, 180.0 / 3.0, 180, 0, 180.0 / 3.0, {180}, {}, {180}, QVector<QVector<QString>>({{"t20", "t20", "t20"}}), QVector<QVector<QString>>({{"t20", "t20", "t20"}}), QVector<QVector<QVector<QString>>>({}), {301, 121}, {}, false};
  CX01Class::CPlayerData snap2 = {0, 0, 0, 16, 0, 0, 6, 285.0 / 6.0, 285.0 / 2.0, 0, 285.0 / 3.0, {180, 105}, {}, {180, 105}, QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}}), QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}}), QVector<QVector<QVector<QString>>>({}), {301, 121, 16}, {}, true};
  CX01Class::CPlayerData expected = {1, 0, 1, 0, 3, 1, 9, 301.0 / 9.0, 301.0 / 3.0, (1.0 / 3.0) * 100, 301.0 / 3.0, {180, 105, 16}, {}, {180, 105, 16}, QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}, {"s0", "s0", "d8"}}), QVector<QVector<QString>>({{"t20", "t20", "t20"}, {"t20", "d20", "s5"}, {"s0", "s0", "d8"}}), QVector<QVector<QVector<QString>>>({}), {301, 121, 16, 0}, {}, true};
  gameData = {snap1, snap2};
  EXPECT_CALL(*mDb.get(), init_dartboard(16));
  EXPECT_CALL(*mMockWindow.get(), reset_scores_of_all_players());
  EXPECT_CALL(*mMockWindow.get(), update_players(EUpdateType::SET));
  EXPECT_CALL(*mMockWindow.get(), handle_game_won(0));
  mBox->set_game_data(gameData);
  mBox->submit_score(16, 3, 3, {"s0", "s0", "d8"});
  mBox->create_snapshot();
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CX01GroupBoxTest, PerformUndoAtStartTest)
{
  CX01Class::CPlayerData expected = {0, 0, 0, 301, 0, 0, 0, 0, 0, 0, 0, {}, {}, {}, QVector<QVector<QString>>({}), QVector<QVector<QString>>({}), QVector<QVector<QVector<QString>>>({}), {301}, {}, false};
  perform_undo();
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CX01GroupBoxTest, PerformUndoNormalTest)
{
  CX01Class::CPlayerData expected = {0, 0, 0, 301, 0, 0, 0, 0, 0, 0, 0, {}, {}, {}, QVector<QVector<QString>>({}), QVector<QVector<QString>>({}), QVector<QVector<QVector<QString>>>({}), {301}, {}, false};
  mBox->submit_score(57, 3, 0, {"s20", "s20", "s19"});
  perform_undo();
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CCricketGroupBoxTest, HandleSubmitButtonClicked3DartsTest)
{
  CCricketClass::CPlayerData expected = {0, 0, 0, 3, 0, 2, 2, {{"s20", "s20", "s0"}}, {}, {0, 2}, {}, {0, 0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}, {false}, {}, true};
  QVector<QString> darts = {"s20", "s20", "s0"};
  EXPECT_CALL(*mMockWindow.get(), is_score_bigger(0)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*mMockWindow.get(), is_score_smaller(0)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*mMockWindow.get(), update_extra_points_labels());
  EXPECT_CALL(*mMockWindow.get(), update_players(EUpdateType::DEFAULT));
  mBox->set_active();
  mBox->handle_submit_button_clicked(3, darts);
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CCricketGroupBoxTest, HandleSubmitButtonClickedOverflowTest)
{
  CCricketClass::CPlayerData expected = {0, 0, 0, 3, 0, 3, 3, {{"s20", "s20", "s20"}}, {}, {0, 3}, {}, {0, 0, 0, 0, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}, {false}, {}, true};
  QVector<QString> darts = {"s20", "s20", "d20"};
  EXPECT_CALL(*mMockWindow.get(), is_score_bigger(0)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*mMockWindow.get(), is_score_smaller(0)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*mMockWindow.get(), update_extra_points_labels());
  EXPECT_CALL(*mMockWindow.get(), update_players(EUpdateType::DEFAULT));
  EXPECT_CALL(*mMockWindow.get(), is_slot_free(ECricketSlots::SLOT_20, 0));
  mBox->set_active();
  mBox->handle_submit_button_clicked(3, darts);
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CCricketGroupBoxTest, HandleSubmitButtonClickedOverflowFillsExtraPointsTest)
{
  CCricketClass::CPlayerData expected = {0, 0, 0, 3, 20, 4, 4, {{"s20", "s20", "d20"}}, {}, {0, 4}, {}, {0, 0, 0, 0, 0, 3, 0}, {0, 0, 0, 0, 0, 20, 0}, {false}, {}, true};
  QVector<QString> darts = {"s20", "s20", "d20"};
  EXPECT_CALL(*mMockWindow.get(), is_score_bigger(20)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*mMockWindow.get(), is_score_smaller(20)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*mMockWindow.get(), update_extra_points_labels());
  EXPECT_CALL(*mMockWindow.get(), update_players(EUpdateType::DEFAULT));
  EXPECT_CALL(*mMockWindow.get(), is_slot_free(ECricketSlots::SLOT_20, 0)).WillOnce(::testing::Return(true));
  mBox->set_active();
  mBox->handle_submit_button_clicked(3, darts);
  EXPECT_TRUE(verify_snapshot(expected));
}

TEST_F(CStatsWindowX01Test, UpdateLegHistoryTest)
{
  CStatsWindowX01::SLegStatsData expected = {(501.0 / 35.0) * 3, 501.0 / 35.0, 0, 161.0 / 3.0, 0, 0, 35};
  update_leg_history(0);
  EXPECT_TRUE(verify_leg_stats_data(expected));
  update_leg_history(1);
  expected = {(501.0 / 24.0) * 3, 501.0 / 24.0, 0, 161.0 / 3.0, 0, 0, 24};
  EXPECT_TRUE(verify_leg_stats_data(expected));
  update_leg_history(2);
  expected = {(501.0 / 31.0) * 3, 501.0 / 31.0, 0, 59.0 / 3.0, 0, 0, 31};
  EXPECT_TRUE(verify_leg_stats_data(expected));
  update_leg_history(3);
  expected = {(109.0 / 6.0) * 3, 109.0 / 6.0, 0, 109.0 / 3.0, 0, 0, 6};
  EXPECT_TRUE(verify_leg_stats_data(expected));
}

TEST_F(CStatsWindowX01Test, CountScoresTest)
{
  count_scores();
  std::array<uint32_t, static_cast<int>(CStatsWindowX01::EScoreCountsIdx::SCORE_COUNT_MAX)> expected = {0, 0, 0, 0, 2, 2, 7, 12, 5, 5, 0, 0, 2, 1, 0, 5, 2, 3, 1, 1};
  EXPECT_TRUE(verify_score_counts(expected));
}

TEST_F(CStatsWindowX01Test, CalculateSegmentCountsTest)
{
  calculate_segment_counts();
  std::array<uint32_t, static_cast<int>(CStatsWindowX01::EDartCountsIdx::SEG_MAX)> expected = {2, 12, 0, 2, 0, 10, 2, 0, 0, 0, 1, 0, 3, 0, 0, 1, 3, 2, 4, 5, 48, 1, 5};
  EXPECT_TRUE(verify_segment_counts(expected));
}

TEST_F(CStatsWindowX01Test, ComputeDartCountAndCheckoutsTest)
{
  CStatsWindowX01::SGlobalGameStatsData expected = {50.375, 50.375 / 3.0, 40.833333333333336, 3, 4, 56, 11, 3};
  compute_dart_count_and_checkouts();
  EXPECT_TRUE(verify_global_stats(expected));
}

TEST_F(CStatsWindowCricketTest, UpdateLegHistoryTest)
{
  CStatsWindowCricket::SLegStatsData expected = {21.0 / 11.0, 105.0 / 4.0, 0, 0, 33};
  update_leg_history(0);
  EXPECT_TRUE(verify_leg_stats_data(expected));
  update_leg_history(1);
  expected = {21.0 / 11.0, 105.0 / 4.0, 0, 0, 33};
  EXPECT_TRUE(verify_leg_stats_data(expected));
  update_leg_history(2);
  expected = {21.0 / 12.0, 105.0 / 4.0, 0, 0, 36};
  EXPECT_TRUE(verify_leg_stats_data(expected));
  update_leg_history(3);
  expected = {6.0 / 3.0, 105.0 / 4.0, 0, 0, 9};
  EXPECT_TRUE(verify_leg_stats_data(expected));
}

TEST_F(CStatsWindowCricketTest, CalculateSegmentCountsTest)
{
  calculate_segment_counts();
  std::array<uint32_t, static_cast<int>(CStatsWindowCricket::EDartCountsIdx::SEG_MAX)> expected = {7, 9, 1, 11, 10, 11, 3, 1, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 46};
  EXPECT_TRUE(verify_segment_counts(expected));
}

TEST_F(CStatsWindowCricketTest, ComputeDartCountOfWonLegsTest)
{
  CStatsWindowCricket::SLegStatsData expected = {0, 0, 33, 36, 0};
  compute_dart_count_of_won_legs();
  EXPECT_TRUE(verify_leg_stats_data(expected));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventDefault1stDartTest)
{
  mScoreInput->handle_segment_pressed_event(20, 's');
  EXPECT_TRUE(verify_state(0, {"s20", "", ""}, {0, 0, 0, 0, 0, 1, 0}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventCutThroat1stDartTest)
{
  CSettings settings(EGame::GAME_CRICKET, {"Player"}, 1, 1, EX01InMode::SINGLE_IN, EX01OutMode::DOUBLE_OUT, true /*cutthroat*/);
  set_settings(settings);
  mScoreInput->handle_segment_pressed_event(20, 's');
  EXPECT_TRUE(verify_state(0, {"s20", "", ""}, {0, 0, 0, 0, 0, 1, 0}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventDefault2ndDartFillsSlotTest)
{
  EXPECT_CALL(*mMockWindow.get(), is_slot_free(ECricketSlots::SLOT_19, 0));
  mScoreInput->handle_segment_pressed_event(19, 's');
  mScoreInput->handle_segment_pressed_event(57, 't');
  EXPECT_TRUE(verify_state(0, {"s19", "t19", ""}, {0, 0, 0, 0, 3, 0, 0}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventDefault2ndDartNoScoreTest)
{
  EXPECT_CALL(*mMockWindow.get(), is_slot_free(ECricketSlots::SLOT_15, 0));
  mScoreInput->handle_segment_pressed_event(19, 's');
  mScoreInput->handle_segment_pressed_event(0, 's');
  EXPECT_TRUE(verify_state(0, {"s19", "s0", ""}, {0, 0, 0, 0, 1, 0, 0}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventDefault3DartsTest)
{
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(20, 's');
  EXPECT_TRUE(verify_state(0, {"s20", "s20", "s20"}, {0, 0, 0, 0, 0, 3, 0}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventDefaultTooManyDartsTest)
{
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(0, 's');
  EXPECT_TRUE(verify_state(0, {"s20", "s20", "s20"}, {0, 0, 0, 0, 0, 3, 0}, {}, {}));
  EXPECT_TRUE(verify_warning("Warning: You only have three darts!"));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventCutThroatTooManyDartsTest)
{
  CSettings settings(EGame::GAME_CRICKET, {"Player"}, 1, 1, EX01InMode::SINGLE_IN, EX01OutMode::DOUBLE_OUT, true /*cutthroat*/);
  set_settings(settings);
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(20, 's');
  mScoreInput->handle_segment_pressed_event(0, 's');
  EXPECT_TRUE(verify_state(0, {"s20", "s20", "s20"}, {0, 0, 0, 0, 0, 3, 0}, {}, {}));
  EXPECT_TRUE(verify_warning("Warning: You only have three darts!"));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventDefaultGameShotTest)
{
  EXPECT_CALL(*mDb, play_game_shot_sound());
  EXPECT_CALL(*mMockWindow.get(), is_score_bigger(0)).WillOnce(::testing::Return(true));
  set_state(0, {"", "", ""}, {3, 3, 3, 3, 3, 3, 2}, {}, {});
  mScoreInput->handle_segment_pressed_event(25, 's');
  EXPECT_TRUE(verify_state(0, {"s25", "", ""}, {3, 3, 3, 3, 3, 3, 3}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventCutThroatGameShotTest)
{
  CSettings settings(EGame::GAME_CRICKET, {"Player"}, 1, 1, EX01InMode::SINGLE_IN, EX01OutMode::DOUBLE_OUT, true /*cutthroat*/);
  set_settings(settings);
  EXPECT_CALL(*mDb, play_game_shot_sound());
  for (int i = 0; i < static_cast<int>(ECricketSlots::SLOT_MAX); i++)
  {
    EXPECT_CALL(*mMockWindow.get(), compute_extra_points(static_cast<ECricketSlots>(i), 0, 0));
  }
  set_state(0, {"", "", ""}, {3, 3, 3, 3, 3, 3, 2}, {}, {});
  mScoreInput->handle_segment_pressed_event(25, 's');
  EXPECT_TRUE(verify_state(0, {"s25", "", ""}, {3, 3, 3, 3, 3, 3, 3}, {}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventComputeScoreTest)
{
  set_state(0, {"", "", ""}, {3, 3, 3, 3, 3, 3, 0}, {15, 32, 0, 0, 57, 20, 0}, {});
  mScoreInput->handle_segment_pressed_event(25, 's');
  EXPECT_TRUE(verify_state(124, {"s25", "", ""}, {3, 3, 3, 3, 3, 3, 1}, {15, 32, 0, 0, 57, 20, 0}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventIncreaseDefaultExtraPointsTest)
{
  set_state(0, {"", "", ""}, {0, 0, 0, 0, 0, 1, 0}, {}, {});
  EXPECT_CALL(*mMockWindow.get(), is_slot_free(ECricketSlots::SLOT_20, 0)).WillOnce(::testing::Return(true));
  mScoreInput->handle_segment_pressed_event(60, 't');
  EXPECT_TRUE(verify_state(20, {"t20", "", ""}, {0, 0, 0, 0, 0, 3, 0}, {0, 0, 0, 0, 0, 20, 0}, {}));
}

TEST_F(CCricketInputTest, HandleSegmentPressedEventUndoTest)
{
  mScoreInput->handle_segment_pressed_event(25, 's');
  EXPECT_TRUE(verify_undo(0, {"", "", ""}, {0, 0, 0, 0, 0, 0, 0}, {}, {}));
}

TEST_F(CGameDataHandlerTest, AddNewPlayerTest)
{
  QString player = "TestPlayer";
  CGameDataHandler handler;
  handler.add_new_player(player);
  EXPECT_TRUE(player_exists(player, handler));
}

TEST_F(CGameDataHandlerTest, GetPlayerNamesTest)
{
  QStringList players {"Player1, Player2, Player3"};
  CGameDataHandler handler;
  for (const auto & player : players)
  {
    handler.add_new_player(player);
  }
  EXPECT_TRUE(players == handler.get_player_names());
}
