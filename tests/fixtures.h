#pragma once

#include <gtest/gtest.h>
#include <QPointer>
#include "x01_mainwindow.h"
#include "cricket_mainwindow.h"
#include "dartboard_x01.h"
#include "mocks.h"
#include "stats_window_x01.h"
#include "stats_window_cricket.h"
#include "cricket_input.h"
#include "game_data_handler.h"
#include <QtSql/QSqlDatabase>
#include <qfile.h>

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

  testing::AssertionResult verify_dartboard_x01(const SState iExpectedState) const
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

  testing::AssertionResult verify_dartboard_x01_warning(const std::string iExpectedStr) const
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

  testing::AssertionResult verify_score_sound_path(const uint32_t iExpectedScore) const
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

  testing::AssertionResult verify_snapshot(const CX01Class::CPlayerData & iExpected) const
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

class CCricketGroupBoxTest : public ::testing::Test
{
protected:

  CCricketGroupBoxTest()
    : mMockWindow(std::make_unique<CCricketMainWindowMock>())
    , mBox(std::make_unique<CCricketGroupBox>(mMockWindow.get(), mSettings, 0))
  {}

  testing::AssertionResult verify_snapshot(const CCricketClass::CPlayerData & iExpected) const
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
  std::unique_ptr<CCricketMainWindowMock> mMockWindow;
  std::unique_ptr<CCricketGroupBox> mBox;
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

  testing::AssertionResult verify_leg_stats_data(CStatsWindowX01::SLegStatsData iExpected) const
  {
    if (mStats->mLegStatsData == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_score_counts(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EScoreCountsIdx::SCORE_COUNT_MAX)> & iExpected) const
  {
    if (mStats->mScoreCounts == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_segment_counts(std::array<uint32_t, static_cast<int>(CStatsWindowX01::EDartCountsIdx::SEG_MAX)> & iExpected) const
  {
    if (mStats->mSegmentCounts == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_global_stats(CStatsWindowX01::SGlobalGameStatsData & iExpected) const
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

  testing::AssertionResult verify_leg_stats_data(CStatsWindowCricket::SLegStatsData iExpected) const
  {
    if (mStats->mLegStatsData == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  testing::AssertionResult verify_segment_counts(std::array<uint32_t, static_cast<int>(CStatsWindowCricket::EDartCountsIdx::SEG_MAX)> & iExpected) const
  {
    if (mStats->mSegmentCounts == iExpected) return testing::AssertionSuccess();
    else return testing::AssertionFailure();
  }

  void update_leg_history(int iIndex) { mStats->update_leg_history(iIndex); }
  void calculate_segment_counts() { mStats->calculate_segment_counts(); }
  void compute_dart_count_of_won_legs() { mStats->compute_dart_count_of_won_legs(); }

  CCricketClass::CPlayerData mData;
  std::unique_ptr<CStatsWindowCricket> mStats;
};

class CCricketInputTest : public ::testing::Test
{
protected:

  CCricketInputTest()
    : mMockWindow(std::make_unique<CCricketMainWindowMock>())
    , mDb(std::make_unique<CDartBoardMock>())
  {
    mScoreInput = std::make_unique<CCricketInput>(mDb.get(), mMockWindow.get());
  }

  void set_settings(CSettings & iSettings)
  {
    mScoreInput->mSettings = iSettings;
  }

  void set_state(uint32_t iScore,
                 const QVector<QString> & iDarts,
                 const std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> & iSlots,
                 const std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExtraPoints,
                 const std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> & iCutThroatExtraPoints
                 )
  {
    mScoreInput->mScore = iScore;
    mScoreInput->mDarts = iDarts;
    mScoreInput->mSlotArray = iSlots;
    mScoreInput->mExtraPointsArray = iExtraPoints;
    mScoreInput->mCutThroatExtraPointsArray = iCutThroatExtraPoints;
  }

  testing::AssertionResult verify_state(uint32_t iExpectedScore,
                                        const QVector<QString> & iExpectedDarts,
                                        const std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExpectedSlots,
                                        const std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExpectedExtraPoints,
                                        const std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExpectedCutThroatExtraPoints
                                        ) const
  {
    if (mScoreInput->mScore != iExpectedScore)                                    return ::testing::AssertionFailure() << "Unexpected Score: " << mScoreInput->mScore;
    if (mScoreInput->mDarts != iExpectedDarts)                                    return ::testing::AssertionFailure() << "Unexpected Darts";
    if (mScoreInput->mSlotArray != iExpectedSlots)                                return ::testing::AssertionFailure() << "Unexpected Slot Array";
    if (mScoreInput->mExtraPointsArray != iExpectedExtraPoints)                   return ::testing::AssertionFailure() << "Unexpected Extra Points";
    if (mScoreInput->mCutThroatExtraPointsArray != iExpectedCutThroatExtraPoints) return ::testing::AssertionFailure() << "Unexpected CutThroat Extra Points";
    return ::testing::AssertionSuccess();
  }

  testing::AssertionResult verify_warning(const std::string iExpectedStr) const
  {
    if (mScoreInput->mOutput.str() == iExpectedStr) return testing::AssertionSuccess();
    return testing::AssertionFailure();
  }

  testing::AssertionResult verify_undo(uint32_t iExpectedScore,
                                       const QVector<QString> & iExpectedDarts,
                                       const std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExpectedSlots,
                                       const std::array<uint32_t, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExpectedExtraPoints,
                                       const std::array<QVector<uint32_t>, static_cast<int>(ECricketSlots::SLOT_MAX)> & iExpectedCutThroatExtraPoints
                                       ) const
  {
    mScoreInput->undo_button_clicked_slot();
    return verify_state(iExpectedScore, iExpectedDarts, iExpectedSlots, iExpectedExtraPoints, iExpectedCutThroatExtraPoints);
  }

  std::unique_ptr<CCricketMainWindowMock> mMockWindow;
  std::unique_ptr<CCricketInput> mScoreInput;
  std::unique_ptr<CDartBoardMock> mDb;
};

class CGameDataHandlerTest : public ::testing::Test
{
protected:

  void SetUp() override
  {
    if (QFile::exists(CGameDataHandler::mFileName)) QFile::remove(CGameDataHandler::mFileName);
//    int argc = 0;
//    char * argv[] = {nullptr};
//    app = new QCoreApplication(argc, argv);
    create_x01_test_data();
    create_cricket_test_data();
    create_stats_test_data();
  }

  void create_x01_test_data()
  {
    CSettings settings(EGame::GAME_501, mPlayers, 1 /*sets*/, 1 /*legs*/, EX01InMode::SINGLE_IN, EX01OutMode::DOUBLE_OUT, false /*cutThroat*/);
    QVector<CX01Class::CPlayerData> player1Data = {{0, 0, 0, 501, 0, 0, 0,  0.0,   0.0,   0.0,  0.0,   {},                        {},                              {},                            {},                                                                                                                                       {},                                                                                                                                                             {},                                                                                                                                                               {501},                               {},                                      false},
                                                   {0, 0, 0, 441, 0, 0, 3,  20.0,  60.0,  0.0,  20.0,  {60},                      {},                              {60},                          {{"s20", "s20", "s20"}},                                                                                                                  {{"s20", "s20", "s20"}},                                                                                                                                        {},                                                                                                                                                               {501, 441},                          {},                                      false},
                                                   {0, 0, 0, 356, 0, 0, 6,  24.17, 72.5,  0.0,  48.33, {60, 85},                  {},                              {60, 85},                      {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}},                                                                                            {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}},                                                                                                                  {},                                                                                                                                                               {501, 441, 356},                     {},                                      false},
                                                   {0, 0, 0, 256, 0, 0, 9,  27.22, 81.67, 0.0,  81.67, {60, 85, 100},             {},                              {60, 85, 100},                 {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}},                                                                     {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}},                                                                                           {},                                                                                                                                                               {501, 441, 356, 256},                {},                                      false},
                                                   {0, 0, 0, 211, 0, 0, 12, 24.17, 72.5,  0.0,  81.67, {60, 85, 100, 45},         {},                              {60, 85, 100, 45},             {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}},                                               {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}},                                                                     {},                                                                                                                                                               {501, 441, 356, 256, 211},           {},                                      false},
                                                   {0, 0, 0, 121, 0, 0, 15, 25.33, 76.0,  0.0,  81.67, {60, 85, 100, 45, 90},     {},                              {60, 85, 100, 45, 90},         {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}, {"t54", "s18", "s18"}},                        {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}, {"t54", "s18", "s18"}},                                              {},                                                                                                                                                               {501, 441, 356, 256, 211, 121},      {},                                      false},
                                                   {0, 0, 0, 56,  0, 0, 18, 24.72, 74.17, 0.0,  81.67, {60, 85, 100, 45, 90, 65}, {},                              {60, 85, 100, 45, 90, 65},     {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}, {"t54", "s18", "s18"}, {"s25", "s20", "s20"}}, {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}, {"t54", "s18", "s18"}, {"s25", "s20", "s20"}},                       {},                                                                                                                                                               {501, 441, 356, 256, 211, 121, 56},  {},                                      false},
                                                   {1, 0, 1, 501, 2, 1, 21, 23.86, 71.57, 50.0, 81.67, {},                        {{60, 85, 100, 45, 90, 65, 56}}, {60, 85, 100, 45, 90, 65, 56}, {},                                                                                                                                       {{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}, {"t54", "s18", "s18"}, {"s25", "s20", "s20"}, {"s20", "s0", "d36"}}, {{{"s20", "s20", "s20"}, {"s20", "t60", "s5"}, {"s20", "s20", "t60"}, {"s20", "s20", "s5"}, {"t54", "s18", "s18"}, {"s25", "s20", "s20"}, {"s20", "s0", "d36"}}}, {501},                               {{501, 441, 356, 256, 211, 121, 56, 0}}, false}};

    QVector<CX01Class::CPlayerData> player2Data = {{0, 0, 0, 501, 0, 0, 0,  0.0,   0.0,   0.0,  0.0,   {},                        {},                              {},                            {},                                                                                                                                       {},                                                                                                                                                             {},                                                                                                                                                               {501},                               {},                                      false},
                                                   {0, 0, 0, 420, 0, 0, 3,  27.0,  81.0,  0.0,  27.0,  {81},                      {},                              {81},                          {{"s20", "t60", "s1"}},                                                                                                                   {{"s20", "t60", "s1"}},                                                                                                                                         {},                                                                                                                                                               {501, 420},                          {},                                      false},
                                                   {0, 0, 0, 379, 0, 0, 6,  20.3,  61.0,  0.0,  40.67, {81, 41},                  {},                              {81, 41},                      {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}},                                                                                             {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}},                                                                                                                   {},                                                                                                                                                               {501, 420, 379},                     {},                                      false},
                                                   {0, 0, 0, 349, 0, 0, 9,  16.89, 50.67, 0.0,  50.67, {81, 41, 30},              {},                              {81, 41, 30},                  {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}},                                                                        {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}},                                                                                              {},                                                                                                                                                               {501, 420, 379, 349},                {},                                      false},
                                                   {0, 0, 0, 291, 0, 0, 12, 17.5,  52.5,  0.0,  50.67, {81, 41, 30, 58},          {},                              {81, 41, 30, 58},              {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}},                                                 {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}},                                                                       {},                                                                                                                                                               {501, 420, 379, 349, 291},           {},                                      false},
                                                   {0, 0, 0, 208, 0, 0, 15, 19.53, 58.6,  0.0,  50.67, {81, 41, 30, 58, 83},      {},                              {81, 41, 30, 58, 83},          {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}, {"t3", "t60", "s20"}},                           {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}, {"t3", "t60", "s20"}},                                                 {},                                                                                                                                                               {501, 420, 379, 349, 291, 208},      {},                                      false},
                                                   {0, 0, 0, 111, 0, 0, 18, 21.67, 65.0,  0.0,  50.67, {81, 41, 30, 58, 83, 97},  {},                              {81, 41, 30, 58, 83, 97},      {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}, {"t3", "t60", "s20"}, {"t57", "s20", "s20"}},    {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}, {"t3", "t60", "s20"}, {"t57", "s20", "s20"}},                          {},                                                                                                                                                               {501, 420, 379, 349, 291, 208, 111}, {},                                      false},
                                                   {0, 0, 0, 501, 0, 0, 18, 21.67, 65.0,  0.0,  50.67, {},                        {{81, 41, 30, 58, 83, 97}},      {81, 41, 30, 58, 83, 97},      {},                                                                                                                                       {{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}, {"t3", "t60", "s20"}, {"t57", "s20", "s20"}},                          {{{"s20", "t60", "s1"}, {"s20", "s20", "s1"}, {"s5", "s5", "s20"}, {"s20", "s20", "s18"}, {"t3", "t60", "s20"}, {"t57", "s20", "s20"}}},                          {501},                               {{501, 420, 379, 349, 291, 208, 111}},   true}};

    QVector<QVector<CX01Class::CPlayerData>> x01Data = {player1Data, player2Data};
    mX01Data= CGameDataHandler::SGameData(mTimeStamp, true /*finished*/, settings, 0 /*winnerIdx*/, x01Data, {});
  }

  void create_cricket_test_data()
  {
    CSettings settings(EGame::GAME_CRICKET, mPlayers, 1 /*sets*/, 1 /*legs*/, EX01InMode::SINGLE_IN, EX01OutMode::DOUBLE_OUT, false /*cutThroat*/);
    QVector<CCricketClass::CPlayerData> player1Data = {{0, 0, 0, 0,  0,  0,  0.0,  {},                                                                                                                                                                                                                              {},                                                                                                                                                                                                                                                 {0},                               {},                                     {0, 0, 0, 0, 0, 0, 0}, {0,  0,  0, 0, 0,  0, 0}, {},                                                                     {},                                                                             false},
                                                       {0, 0, 0, 3,  0,  3,  3.0,  {{"s20", "s20", "s20"}},                                                                                                                                                                                                         {},                                                                                                                                                                                                                                                 {0, 3},                            {},                                     {0, 0, 0, 0, 0, 3, 0}, {0,  0,  0, 0, 0,  0, 0}, {false},                                                                {},                                                                             false},
                                                       {0, 0, 0, 6,  0,  6,  3.0,  {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}},                                                                                                                                                                                  {},                                                                                                                                                                                                                                                 {0, 3, 3},                         {},                                     {0, 0, 0, 3, 0, 3, 0}, {0,  0,  0, 0, 0,  0, 0}, {false, false},                                                         {},                                                                             false},
                                                       {0, 0, 0, 9,  0,  8,  2.67, {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}},                                                                                                                                                            {},                                                                                                                                                                                                                                                 {0, 3, 3, 2},                      {},                                     {0, 0, 0, 3, 2, 3, 0}, {0,  0,  0, 0, 0,  0, 0}, {false, false, false},                                                  {},                                                                             false},
                                                       {0, 0, 0, 12, 38, 11, 2.75, {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}},                                                                                                                                     {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3},                   {},                                     {0, 0, 0, 3, 3, 3, 0}, {0,  0,  0, 0, 38, 0, 0}, {false, false, false, false},                                           {},                                                                             false},
                                                       {0, 0, 0, 15, 54, 15, 3.0,  {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}},                                                                                                               {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3, 4},                {},                                     {0, 3, 0, 3, 3, 3, 0}, {0,  16, 0, 0, 38, 0, 0}, {false, false, false, false, false},                                    {},                                                                             false},
                                                       {0, 0, 0, 18, 54, 17, 2.83, {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}, {"s17", "s17", "s0"}},                                                                                         {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3, 4, 2},             {},                                     {0, 3, 2, 3, 3, 3, 0}, {0,  16, 0, 0, 38, 0, 0}, {false, false, false, false, false, false},                             {},                                                                             false},
                                                       {0, 0, 0, 21, 54, 19, 2.71, {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}, {"s17", "s17", "s0"}, {"s17", "s0", "s25"}},                                                                   {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3, 4, 2, 2},          {},                                     {0, 3, 3, 3, 3, 3, 1}, {0,  16, 0, 0, 38, 0, 0}, {false, false, false, false, false, false, false},                      {},                                                                             false},
                                                       {0, 0, 0, 24, 54, 21, 2.63, {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}, {"s17", "s17", "s0"}, {"s17", "s0", "s25"}, {"s15", "s0", "s15"}},                                             {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3, 4, 2, 2, 2},       {},                                     {2, 3, 3, 3, 3, 3, 1}, {0,  16, 0, 0, 38, 0, 0}, {false, false, false, false, false, false, false, false},               {},                                                                             false},
                                                       {0, 0, 0, 27, 99, 25, 2.78, {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}, {"s17", "s17", "s0"}, {"s17", "s0", "s25"}, {"s15", "s0", "s15"}, {"d15", "s15", "s15"}},                      {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3, 4, 2, 2, 2, 4},    {},                                     {3, 3, 3, 3, 3, 3, 1}, {45, 16, 0, 0, 38, 0, 0}, {false, false, false, false, false, false, false, false, false},        {},                                                                             false},
                                                       {0, 0, 0, 30, 99, 26, 2.6,  {{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}, {"s17", "s17", "s0"}, {"s17", "s0", "s25"}, {"s15", "s0", "s15"}, {"d15", "s15", "s15"}, {"s25", "s0", "s0"}}, {},                                                                                                                                                                                                                                                 {0, 3, 3, 2, 3, 4, 2, 2, 2, 4, 1}, {},                                     {3, 3, 3, 3, 3, 3, 2}, {45, 16, 0, 0, 38, 0, 0}, {false, false, false, false, false, false, false, false, false, false}, {},                                                                             false},
                                                       {1, 0, 1, 31, 0,  27, 2.61, {},                                                                                                                                                                                                                              {{{"s20", "s20", "s20"}, {"s18", "s18", "s18"}, {"s19", "s19", "s0"}, {"s19", "s19", "s19"}, {"t16", "s16", "s0"}, {"s17", "s17", "s0"}, {"s17", "s0", "s25"}, {"s15", "s0", "s15"}, {"d15", "s15", "s15"}, {"s25", "s0", "s0"}, {"s25", "", ""}}}, {0},                               {{0, 3, 3, 2, 3, 4, 2, 2, 2, 4, 1, 1}}, {0, 0, 0, 0, 0, 0, 0}, {0,  0,  0, 0, 0,  0, 0}, {},                                                                     {{false, false, false, false, false, false, false, false, false, false, true}}, false}};

    QVector<CCricketClass::CPlayerData> player2Data = {{0, 0, 0, 0,  0,  0,  0.0,  {},                                                                                                                                                                                                                              {},                                                                                                                                                                                                                                                 {0},                               {},                                     {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0},    {},                                                                     {},                                                                             false},
                                                       {0, 0, 0, 3,  0,  3,  3.0,  {{"s18", "s18", "s18"}},                                                                                                                                                                                                         {},                                                                                                                                                                                                                                                 {0, 3},                            {},                                     {0, 0, 0, 3, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0},    {false},                                                                {},                                                                             false},
                                                       {0, 0, 0, 6,  0,  6,  2.0,  {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}},                                                                                                                                                                                    {},                                                                                                                                                                                                                                                 {0, 3, 1},                         {},                                     {0, 0, 0, 3, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0},    {false, false},                                                         {},                                                                             false},
                                                       {0, 0, 0, 9,  0,  7,  2.33, {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}},                                                                                                                                                             {},                                                                                                                                                                                                                                                 {0, 3, 1, 3},                      {},                                     {0, 0, 3, 3, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0},    {false, false, false},                                                  {},                                                                             false},
                                                       {0, 0, 0, 12, 0,  10, 2.5,  {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}},                                                                                                                                      {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3},                   {},                                     {0, 2, 3, 3, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0},    {false, false, false, false},                                           {},                                                                             false},
                                                       {0, 0, 0, 15, 0,  12, 2.4,  {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}},                                                                                                                {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3, 2},                {},                                     {0, 2, 3, 3, 3, 1, 0}, {0, 0, 0, 0, 0, 0, 0},    {false, false, false, false, false},                                    {},                                                                             false},
                                                       {0, 0, 0, 18, 0,  13, 2.17, {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}, {"s16", "s0", "s0"}},                                                                                           {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3, 2, 1},             {},                                     {0, 3, 3, 3, 3, 1, 0}, {0, 0, 0, 0, 0, 0, 0},    {false, false, false, false, false, false},                             {},                                                                             false},
                                                       {0, 0, 0, 21, 0,  15, 2.14, {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}, {"s16", "s0", "s0"}, {"s20", "s20", "s0"}},                                                                     {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3, 2, 1, 2},          {},                                     {0, 3, 3, 3, 3, 3, 0}, {0, 0, 0, 0, 0, 0, 0},    {false, false, false, false, false, false, false},                      {},                                                                             false},
                                                       {0, 0, 0, 24, 0,  17, 2.13, {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}, {"s16", "s0", "s0"}, {"s20", "s20", "s0"}, {"s25", "s25", "s0"}},                                               {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3, 2, 1, 2, 2},       {},                                     {0, 3, 3, 3, 3, 3, 2}, {0, 0, 0, 0, 0, 0, 0},    {false, false, false, false, false, false, false, false},               {},                                                                             false},
                                                       {0, 0, 0, 27, 75, 21, 2.33, {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}, {"s16", "s0", "s0"}, {"s20", "s20", "s0"}, {"s25", "s25", "s0"}, {"d25", "s25", "s25"}},                        {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3, 2, 1, 2, 2, 4},    {},                                     {0, 3, 3, 3, 3, 3, 3}, {0, 0, 0, 0, 0, 0, 75},   {false, false, false, false, false, false, false, false, false},        {},                                                                             false},
                                                       {0, 0, 0, 30, 75, 24, 2.4,  {{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}, {"s16", "s0", "s0"}, {"s20", "s20", "s0"}, {"s25", "s25", "s0"}, {"d25", "s25", "s25"}, {"s15", "s15", "s15"}}, {},                                                                                                                                                                                                                                                 {0, 3, 1, 3, 3, 2, 1, 2, 2, 4, 3}, {},                                     {3, 3, 3, 3, 3, 3, 3}, {0, 0, 0, 0, 0, 0, 75},   {false, false, false, false, false, false, false, false, false, false}, {},                                                                             false},
                                                       {0, 0, 0, 30, 0,  24, 2.4,  {},                                                                                                                                                                                                                              {{{"s18", "s18", "s18"}, {"s20", "s0", "s0"}, {"s17", "s17", "s17"}, {"s19", "s16", "s16"}, {"s19", "s19", "s0"}, {"s16", "s0", "s0"}, {"s20", "s20", "s0"}, {"s25", "s25", "s0"}, {"d25", "s25", "s25"}, {"s15", "s15", "s15"}}},                  {0},                               {{0, 3, 1, 3, 3, 2, 1, 2, 2, 4, 3}},    {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0},    {},                                                                     {{false, false, false, false, false, false, false, false, false, false}},       true}};

    QVector<QVector<CCricketClass::CPlayerData>> cricketData = {player1Data, player2Data};
    mCricketData = CGameDataHandler::SGameData(mTimeStamp, true /*finished*/, settings, 0 /*winnerIdx*/, {}, cricketData);
  }

  void create_stats_test_data()
  {
    mStatsData.resize(mPlayers.size());
    mStatsData[0].GameDataX01.append(mX01Data.GameDataX01.at(0).back());
    mStatsData[0].GameDataCricket.append(mCricketData.GameDataCricket.at(0).back());
    mStatsData[0].PlayerName = mPlayers.at(0);
    mStatsData[1].GameDataX01.append(mX01Data.GameDataX01.at(1).back());
    mStatsData[1].GameDataCricket.append(mCricketData.GameDataCricket.at(1).back());
    mStatsData[1].PlayerName = mPlayers.at(1);
  }

  void TearDown() override
  {
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
  }

  bool player_exists(const QString & iPlayerName, const CGameDataHandler & iHandler) { return iHandler.player_exists(iPlayerName); }

  QStringList mPlayers = {"Player1", "Player2"};
  QString mTimeStamp = "2024-01-01T00:00:00Z";
  CGameDataHandler::SGameData mX01Data;
  CGameDataHandler::SGameData mCricketData;
  QVector<CGameDataHandler::SStatsData> mStatsData;
//  QCoreApplication * app;
};
