#include "dartboard_base.h"
#include "dartboard_segment.h"

#include <array>
#include <cmath>
#include <utility>

namespace
{
constexpr double kPi = 3.14159265358979323846;
constexpr double kCenter = 400.0;

QPointF point_at(const double iRadius, const double iAngle)
{
  const double radians = iAngle * kPi / 180.0;
  return {kCenter + iRadius * std::cos(radians),
          kCenter - iRadius * std::sin(radians)};
}

QPainterPath annular_sector(const double iInnerRadius, const double iOuterRadius,
                            const double iStartAngle, const double iSpan = 18.0)
{
  QPainterPath path;
  path.moveTo(point_at(iInnerRadius, iStartAngle));
  path.lineTo(point_at(iOuterRadius, iStartAngle));
  path.arcTo(QRectF(kCenter - iOuterRadius, kCenter - iOuterRadius,
                    2.0 * iOuterRadius, 2.0 * iOuterRadius),
             iStartAngle, iSpan);
  path.lineTo(point_at(iInnerRadius, iStartAngle + iSpan));
  path.arcTo(QRectF(kCenter - iInnerRadius, kCenter - iInnerRadius,
                    2.0 * iInnerRadius, 2.0 * iInnerRadius),
             iStartAngle + iSpan, -iSpan);
  path.closeSubpath();
  return path;
}

QPainterPath single_sector(const double iStartAngle)
{
  QPainterPath path = annular_sector(40.0, 180.0, iStartAngle);
  path.addPath(annular_sector(200.0, 320.0, iStartAngle));
  return path;
}
} // namespace

#ifndef TESTING
CDartBoard::CDartBoard(CDartBoardView * iGraphicsViewDartBoard, CSettings iSettings)
  : mSettings(std::move(iSettings))
  , mSoundHandler(CSoundHandler::instance())
{
  mScene = new QGraphicsScene(0, 0, 800, 800, iGraphicsViewDartBoard);
  mScene->setSceneRect(0, 0, 800, 800);
  iGraphicsViewDartBoard->setRenderHint(QPainter::SmoothPixmapTransform, true);
  iGraphicsViewDartBoard->setRenderHint(QPainter::Antialiasing, true);
  iGraphicsViewDartBoard->setScene(mScene);
  iGraphicsViewDartBoard->show();

  constexpr std::array<double, 20> angles{
    81, 243, 45, 279, 207, 315, 153, 9, 117, 171,
    333, 135, 189, 225, 351, 99, 27, 261, 297, 63
  };

  QPainterPath bull;
  bull.addEllipse(QRectF(360, 360, 80, 80));
  bull.addEllipse(QRectF(380, 380, 40, 40));

  QPainterPath noScore;
  noScore.addEllipse(QRectF(0, 0, 800, 800));
  noScore.addEllipse(QRectF(58, 58, 684, 684));

  mS20 = new CDartBoardSegment(this, single_sector(angles[0]), 20);
  mS19 = new CDartBoardSegment(this, single_sector(angles[1]), 19, "beige");
  mS18 = new CDartBoardSegment(this, single_sector(angles[2]), 18);
  mS17 = new CDartBoardSegment(this, single_sector(angles[3]), 17, "beige");
  mS16 = new CDartBoardSegment(this, single_sector(angles[4]), 16, "beige");
  mS15 = new CDartBoardSegment(this, single_sector(angles[5]), 15, "beige");
  mS14 = new CDartBoardSegment(this, single_sector(angles[6]), 14);
  mS13 = new CDartBoardSegment(this, single_sector(angles[7]), 13);
  mS12 = new CDartBoardSegment(this, single_sector(angles[8]), 12);
  mS11 = new CDartBoardSegment(this, single_sector(angles[9]), 11, "beige");
  mS10 = new CDartBoardSegment(this, single_sector(angles[10]), 10);
  mS9 = new CDartBoardSegment(this, single_sector(angles[11]), 9, "beige");
  mS8 = new CDartBoardSegment(this, single_sector(angles[12]), 8);
  mS7 = new CDartBoardSegment(this, single_sector(angles[13]), 7);
  mS6 = new CDartBoardSegment(this, single_sector(angles[14]), 6, "beige");
  mS5 = new CDartBoardSegment(this, single_sector(angles[15]), 5, "beige");
  mS4 = new CDartBoardSegment(this, single_sector(angles[16]), 4, "beige");
  mS3 = new CDartBoardSegment(this, single_sector(angles[17]), 3);
  mS2 = new CDartBoardSegment(this, single_sector(angles[18]), 2);
  mS1 = new CDartBoardSegment(this, single_sector(angles[19]), 1, "beige");

  mSBull = new CDartBoardSegment(this, bull, 25, "green");
  QPainterPath doubleBull;
  doubleBull.addEllipse(QRectF(380, 380, 40, 40));
  mDBull = new CDartBoardSegment(this, doubleBull, 50, "red", 'd');

  mD20 = new CDartBoardSegment(this, annular_sector(320, 340, angles[0]), 40, "red", 'd');
  mD19 = new CDartBoardSegment(this, annular_sector(320, 340, angles[1]), 38, "green", 'd');
  mD18 = new CDartBoardSegment(this, annular_sector(320, 340, angles[2]), 36, "red", 'd');
  mD17 = new CDartBoardSegment(this, annular_sector(320, 340, angles[3]), 34, "green", 'd');
  mD16 = new CDartBoardSegment(this, annular_sector(320, 340, angles[4]), 32, "green", 'd');
  mD15 = new CDartBoardSegment(this, annular_sector(320, 340, angles[5]), 30, "green", 'd');
  mD14 = new CDartBoardSegment(this, annular_sector(320, 340, angles[6]), 28, "red", 'd');
  mD13 = new CDartBoardSegment(this, annular_sector(320, 340, angles[7]), 26, "red", 'd');
  mD12 = new CDartBoardSegment(this, annular_sector(320, 340, angles[8]), 24, "red", 'd');
  mD11 = new CDartBoardSegment(this, annular_sector(320, 340, angles[9]), 22, "green", 'd');
  mD10 = new CDartBoardSegment(this, annular_sector(320, 340, angles[10]), 20, "red", 'd');
  mD9 = new CDartBoardSegment(this, annular_sector(320, 340, angles[11]), 18, "green", 'd');
  mD8 = new CDartBoardSegment(this, annular_sector(320, 340, angles[12]), 16, "red", 'd');
  mD7 = new CDartBoardSegment(this, annular_sector(320, 340, angles[13]), 14, "red", 'd');
  mD6 = new CDartBoardSegment(this, annular_sector(320, 340, angles[14]), 12, "green", 'd');
  mD5 = new CDartBoardSegment(this, annular_sector(320, 340, angles[15]), 10, "green", 'd');
  mD4 = new CDartBoardSegment(this, annular_sector(320, 340, angles[16]), 8, "green", 'd');
  mD3 = new CDartBoardSegment(this, annular_sector(320, 340, angles[17]), 6, "red", 'd');
  mD2 = new CDartBoardSegment(this, annular_sector(320, 340, angles[18]), 4, "red", 'd');
  mD1 = new CDartBoardSegment(this, annular_sector(320, 340, angles[19]), 2, "green", 'd');

  mT20 = new CDartBoardSegment(this, annular_sector(180, 200, angles[0]), 60, "red", 't');
  mT19 = new CDartBoardSegment(this, annular_sector(180, 200, angles[1]), 57, "green", 't');
  mT18 = new CDartBoardSegment(this, annular_sector(180, 200, angles[2]), 54, "red", 't');
  mT17 = new CDartBoardSegment(this, annular_sector(180, 200, angles[3]), 51, "green", 't');
  mT16 = new CDartBoardSegment(this, annular_sector(180, 200, angles[4]), 48, "green", 't');
  mT15 = new CDartBoardSegment(this, annular_sector(180, 200, angles[5]), 45, "green", 't');
  mT14 = new CDartBoardSegment(this, annular_sector(180, 200, angles[6]), 42, "red", 't');
  mT13 = new CDartBoardSegment(this, annular_sector(180, 200, angles[7]), 39, "red", 't');
  mT12 = new CDartBoardSegment(this, annular_sector(180, 200, angles[8]), 36, "red", 't');
  mT11 = new CDartBoardSegment(this, annular_sector(180, 200, angles[9]), 33, "green", 't');
  mT10 = new CDartBoardSegment(this, annular_sector(180, 200, angles[10]), 30, "red", 't');
  mT9 = new CDartBoardSegment(this, annular_sector(180, 200, angles[11]), 27, "green", 't');
  mT8 = new CDartBoardSegment(this, annular_sector(180, 200, angles[12]), 24, "red", 't');
  mT7 = new CDartBoardSegment(this, annular_sector(180, 200, angles[13]), 21, "red", 't');
  mT6 = new CDartBoardSegment(this, annular_sector(180, 200, angles[14]), 18, "green", 't');
  mT5 = new CDartBoardSegment(this, annular_sector(180, 200, angles[15]), 15, "green", 't');
  mT4 = new CDartBoardSegment(this, annular_sector(180, 200, angles[16]), 12, "green", 't');
  mT3 = new CDartBoardSegment(this, annular_sector(180, 200, angles[17]), 9, "red", 't');
  mT2 = new CDartBoardSegment(this, annular_sector(180, 200, angles[18]), 6, "red", 't');
  mT1 = new CDartBoardSegment(this, annular_sector(180, 200, angles[19]), 3, "green", 't');
  mNoScore = new CDartBoardSegment(this, noScore);

  for (CDartBoardSegment * segment : {
         mNoScore, mS20, mS19, mS18, mS17, mS16, mS15, mS14, mS13, mS12,
         mS11, mS10, mS9, mS8, mS7, mS6, mS5, mS4, mS3, mS2, mS1, mSBull,
         mD20, mD19, mD18, mD17, mD16, mD15, mD14, mD13, mD12, mD11, mD10,
         mD9, mD8, mD7, mD6, mD5, mD4, mD3, mD2, mD1, mDBull, mT20, mT19,
         mT18, mT17, mT16, mT15, mT14, mT13, mT12, mT11, mT10, mT9, mT8,
         mT7, mT6, mT5, mT4, mT3, mT2, mT1
       })
  {
    mScene->addItem(segment);
  }

  mLabels->setElementId("label");
  mLabels->setZValue(1);
  mLabels->setScale(0.98);
  mLabels->setPos(10, 10);
  mScene->addItem(mLabels);
}
#endif

CDartBoard::~CDartBoard()
{
#ifndef TESTING
  for (const CDartBoardSegment * segment : {
         mS20, mS19, mS18, mS17, mS16, mS15, mS14, mS13, mS12, mS11,
         mS10, mS9, mS8, mS7, mS6, mS5, mS4, mS3, mS2, mS1, mSBull,
         mD20, mD19, mD18, mD17, mD16, mD15, mD14, mD13, mD12, mD11, mD10,
         mD9, mD8, mD7, mD6, mD5, mD4, mD3, mD2, mD1, mDBull, mT20, mT19,
         mT18, mT17, mT16, mT15, mT14, mT13, mT12, mT11, mT10, mT9, mT8,
         mT7, mT6, mT5, mT4, mT3, mT2, mT1, mNoScore
       })
  {
    delete segment;
  }
  delete mLabels;
  delete mScene;
#endif
}

void CDartBoard::play_game_shot_sound()
{
#ifndef TESTING
  mSoundHandler.play_game_shot_sound();
#endif
}
