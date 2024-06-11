#pragma once

#include <gtest/gtest.h>
#include <QPointer>
#include "x01_mainwindow.h"
#include "dartboard_x01.h"
#include "mocks.h"

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

    if (mBox->mScoreSound.source().toString().toStdString() == expectedPath)
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

  CSettings mSettings;
  std::unique_ptr<CDartBoardMock> mDb;
  std::unique_ptr<CX01MainWindowMock> mMockWindow;
  std::unique_ptr<CX01GroupBox> mBox;
};
