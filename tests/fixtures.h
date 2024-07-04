#pragma once

#include <gtest/gtest.h>
#include <QPointer>
#include "x01_mainwindow.h"
#include "dartboard_x01.h"
#include "mocks.h"
#include "stats_window_x01.h"
#include "stats_window_cricket.h"

class CDartBoardX01Test : public testing::Test
{
protected:

  CDartBoardX01Test()
    : mMockWindow(std::make_unique<CX01MainWindowMock>())
    , mDartBoard(std::make_unique<CDartBoardX01>(mMockWindow.get()))
  {}

  struct SState
  {
    bool Stop = false;
    bool Busted = false;
    uint32_t Score = 0;
    uint32_t OldScore = Score;
    uint32_t Counter = 3;
    QVector<bool> CheckoutAttempts = {false, false, false};
    QVector<QString> Darts = {};
    QVector<uint32_t> Undo = {0, 0, 0};
  };

  testing::AssertionResult verify_dartboard_x01(const SState iExpectedState)
  {
    if (iExpectedState.Stop == mDartBoard->mStop &&
        iExpectedState.Busted == mDartBoard->mBusted &&
        iExpectedState.Score == mDartBoard->mScore &&
        iExpectedState.OldScore == mDartBoard->mOldScore &&
        iExpectedState.Counter == mDartBoard->mCounter &&
        iExpectedState.CheckoutAttempts == mDartBoard->mCheckoutAttempts &&
        iExpectedState.Darts == mDartBoard->mDarts &&
        iExpectedState.Undo == mDartBoard->mUndo
        )
    {
      return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
  }

  testing::AssertionResult verify_dartboard_x01_warning(const std::string iExpectedStr)
  {
    if (mDartBoard->mOutput.str() == iExpectedStr) return testing::AssertionSuccess();
    return testing::AssertionFailure();
  }

  void set_in_mode(EX01InMode iMode)
  {
    mDartBoard->mSettings.InMode = iMode;
  }

  void set_out_mode(EX01OutMode iMode)
  {
    mDartBoard->mSettings.OutMode = iMode;
  }

  void set_x01_mode(EGame iGame)
  {
    mDartBoard->mSettings.Game = iGame;
  }

  std::unique_ptr<CX01MainWindowMock> mMockWindow;
  std::unique_ptr<CDartBoardX01> mDartBoard;
  SState mExpectedState;
};

class CX01GroupBoxTest : public ::testing::Test
{
protected:

  CX01GroupBoxTest()
    : mDb(std::make_unique<CDartBoardMock>())
    , mMockWindow(std::make_unique<CX01MainWindowMock>())
    , mBox(std::make_unique<CX01GroupBox>(mMockWindow.get(), mSettings, 0, mDb.get()))
  {}

  testing::AssertionResult verify_score_sound_path(const uint32_t iExpectedScore)
  {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << iExpectedScore;
    std::string digits = ss.str();
    std::string expectedPath = "qrc:/resources/sounds/" + digits + ".wav";

    if (mBox->mSoundPath.toStdString() == expectedPath)
    {
      return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
  }

  testing::AssertionResult verify_snapshot(const CX01Class::CPlayerData & iExpected)
  {
    auto & snap = mBox->mHistory.back();
    if (snap == iExpected)
    {
      return testing::AssertionSuccess();
    }
    return testing::AssertionFailure();
  }

  void perform_undo() { mBox->perform_undo(); }

  CSettings mSettings;
  std::unique_ptr<CDartBoardMock> mDb;
  std::unique_ptr<CX01MainWindowMock> mMockWindow;
  std::unique_ptr<CX01GroupBox> mBox;
};

class CStatsWindowX01Test : public ::testing::Test
{
protected:

  CStatsWindowX01Test()
  {
    double avg1Dart = 16.791666666666668;
    double avg3Dart = 50.375;
    double checkoutRate = 27.27272727272727;
    double first9Avg = 40.833333333333336;
    bool active = false;
    uint32_t checkoutAttempts = 11;
    uint32_t checkoutHits = 3;
    uint32_t legsWonPerset = 1;
    uint32_t remainingPoints = 392;
    uint32_t setsWon = 1;
    uint32_t totalDarts = 96;
    uint32_t totalLegsWon = 3;
    QVector<uint32_t> allScoresFlat = { 98, 26, 37, 45, 41, 65, 60, 46, 39, 20, 18, 6, 60, 60, 41, 100, 85, 41, 58, 56, 45, 3, 11, 30, 60, 79, 59, 58, 100, 44, 12, 60, 49};
    QVector<uint32_t> remainingPointsOfCurrentLeg = {501, 441, 392};
    QVector<uint32_t> scoresOfCurrentLeg = {60, 49};
    QVector<QVector<uint32_t>> allScoresOfAllLegs = {{ 98, 26, 37, 45, 41, 65, 60, 46, 39, 20, 18, 6}, {60, 60, 41, 100, 85, 41, 58, 56}, {45, 3, 11, 30, 60, 79, 59, 58, 100, 44, 12}};
    QVector<QVector<uint32_t>> remainingPointsOfAllLegs = {{501, 403, 377, 340, 295, 254, 189, 129, 83, 44, 24, 6, 0}, {501, 441, 381, 340, 240, 155, 114, 56, 0}, {501, 456, 453, 442, 412, 352, 273, 214, 156, 56, 12, 0}};

    QVector<QVector<QVector<QString>>> thrownDartsOfAllLegs = {{{"t60", "s20", "s18"}, {"s1", "s20", "s5"}, {"s12", "s5", "s20"}, {"s20", "s20", "s5"}, {"s20", "s20", "s1"}, {"s25", "s20", "s20"}, {"s20", "s20", "s20"}, {"d40", "s5", "s1"}, {"s18", "s1", "s20"}, {"s18", "s1", "s1"}, {"s12", "s0", "s6"}, {"s0", "d6"}},
                                                               {{"s20", "s20", "s20"}, {"s20", "s20", "s20"}, {"s20", "s20", "s1"}, {"s20", "t60", "s20"}, {"s20", "s5", "t60"}, {"s20", "s20", "s1"}, {"s20", "s18", "s20"}, {"s16", "s20", "d20"}},
                                                               {{"s5", "s20", "s20"}, {"s1", "s1", "s1"}, {"s5", "s5", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s20"}, {"t57", "s19", "s3"}, {"s20", "s20", "s19"}, {"s20", "s19", "s19"}, {"s20", "s20", "t60"}, {"s16", "s16", "s12"}, {"d12"}}};
    QVector<QVector<QString>> thrownDartsOfAllLegsFlat = {{"t60", "s20", "s18"}, {"s1", "s20", "s5"}, {"s12", "s5", "s20"}, {"s20", "s20", "s5"}, {"s20", "s20", "s1"}, {"s25", "s20", "s20"}, {"s20", "s20", "s20"}, {"d40", "s5", "s1"}, {"s18", "s1", "s20"}, {"s18", "s1", "s1"}, {"s12", "s0", "s6"}, {"s0", "d6"},
                                                          {"s20", "s20", "s20"}, {"s20", "s20", "s20"}, {"s20", "s20", "s1"}, {"s20", "t60", "s20"}, {"s20", "s5", "t60"}, {"s20", "s20", "s1"}, {"s20", "s18", "s20"}, {"s16", "s20", "d20"},
                                                          {"s5", "s20", "s20"}, {"s1", "s1", "s1"}, {"s5", "s5", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s20"}, {"t57", "s19", "s3"}, {"s20", "s20", "s19"}, {"s20", "s19", "s19"}, {"s20", "s20", "t60"}, {"s16", "s16", "s12"}, {"d12"}, {"s20", "s20", "s20"}, {"s15", "s17", "s17"}};
    QVector<QVector<QString>> thrownDartsOfCurrentLeg = {{"s20", "s20", "s20"}, {"s15", "s17", "s17"}};

    mData = {setsWon, legsWonPerset, totalLegsWon, remainingPoints, checkoutAttempts, checkoutHits, totalDarts, avg1Dart, avg3Dart, checkoutRate, first9Avg, scoresOfCurrentLeg, allScoresOfAllLegs, allScoresFlat, thrownDartsOfCurrentLeg, thrownDartsOfAllLegsFlat, thrownDartsOfAllLegs, remainingPointsOfCurrentLeg, remainingPointsOfAllLegs, active};
    mStats = std::make_unique<CStatsWindowX01>(mData);
  }

  void update_leg_history(int iIndex) { mStats->update_leg_history(iIndex); }
  void count_scores() { mStats->count_scores(); }
  void calculate_segment_counts() { mStats->calculate_segment_counts(); }
  void compute_dart_count_and_checkouts() { mStats->compute_dart_count_and_checkouts(); }

  testing::AssertionResult verify_leg_stats_data(CStatsWindowX01::SLegStatsData iExpected)
  {
    if (mStats->mLegStatsData == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_score_counts(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EScoreCountsIdx::SCORE_COUNT_MAX)> & iExpected)
  {
    if (mStats->mScoreCounts == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_segment_counts(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EDartCountsIdx::SEG_MAX)> & iExpected)
  {
    if (mStats->mSegmentCounts == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_global_stats(CStatsWindowX01::SGlobalGameStatsData & iExpected)
  {
    if (mStats->mGlobalGameStatsData == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  CX01Class::CPlayerData mData;
  std::unique_ptr<CStatsWindowX01> mStats;
};

class CStatsWindowCricketTest : public ::testing::Test
{
protected:

  CStatsWindowCricketTest()
  {
    uint32_t setsWon = 1;
    uint32_t legsWonPerSet = 1;
    uint32_t totalLegsWon = 3;
    uint32_t totalDarts = 105;
    uint32_t score = 0;
    uint32_t totalHits = 69;
    double hitsPerRound = 1.9714285714285715;
    QVector<QVector<QString>> scoresOfCurrentLeg = {{"s17", "s19", "s0"}, {"s0", "s18", "s18"}, {"s20", "s20", "s0"}};
    QVector<QVector<QVector<QString>>> scoringHistory = {{{"s20", "s20", "s20"}, {"s0", "s0", "s18"}, {"s18", "s18", "s0"}, {"s15", "s0", "d15"}, {"t17", "s0", "s19"}, {"s16", "s16", "s0"}, {"s16", "s0", "s0"}, {"s19", "s0", "s19"}, {"s25", "s0", "s0"}, {"s0", "s0", "s0"}, {"d25", "", ""}},
                                                         {{"s20", "s20", "s0"}, {"s18", "s0", "s20"}, {"s18", "s18", "s0"}, {"s19", "s19", "s19"}, {"t17", "s0", "s0"}, {"s15", "s15", "s0"}, {"s15", "s16", "s16"}, {"s16", "s0", "s0"}, {"s0", "s0", "d25"}, {"s0", "s0", "s0"}, {"s25", "", ""}},
                                                         {{"s20", "s20", "s20"}, {"s18", "s18", "s0"}, {"s15", "s15", "s15"}, {"t17", "s0", "s0"}, {"s19", "s19", "s19"}, {"s18", "s0", "s0"}, {"s16", "s16", "s16"}, {"s25", "s0", "s0"}, {"s0", "s0", "s0"}, {"s0", "s0", "s0"}, {"s0", "s0", "s0"}, {"d25", "", ""}}};
    QVector<uint32_t> hitsOfCurrentLeg = {2, 2, 2};
    QVector<QVector<uint32_t>> hitsHistory = {{0, 3, 1, 2, 3, 4, 2, 1, 2, 1, 0, 2}, {2, 2, 2, 3, 3, 2, 3, 1, 2, 0, 1}, {3, 2, 3, 3, 3, 1, 3, 1, 0, 0, 0, 2}};
    QVector<uint32_t> slotArray = {0, 0, 1, 2, 1, 2, 0};
    QVector<uint32_t> extraPointsArray = {0, 0, 0, 0, 0, 0, 0};
    QVector<bool> legWonVec = {false, false, false};
    QVector<QVector<bool>> legWonHistory = {{false, false, false, false, false, false, false, false, false, false, true},
                                            {false, false, false, false, false, false, false, false, false, false, true},
                                            {false, false, false, false, false, false, false, false, false, false, false, true}};

    mData = {setsWon, legsWonPerSet, totalLegsWon, totalDarts, score, totalHits, hitsPerRound, scoresOfCurrentLeg, scoringHistory, hitsOfCurrentLeg, hitsHistory, slotArray, extraPointsArray, legWonVec, legWonHistory, true};
    mStats = std::make_unique<CStatsWindowCricket>(mData);
  }

  CCricketClass::CPlayerData mData;
  std::unique_ptr<CStatsWindowCricket> mStats;
};
