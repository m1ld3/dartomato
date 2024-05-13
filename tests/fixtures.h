#pragma once

#include <gtest/gtest.h>
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

  std::unique_ptr<CX01MainWindowMock> mMockWindow;
  std::unique_ptr<CDartBoardX01> mDartBoard;
};
