#include "game_history_model.h"

CGameHistoryModel::CGameHistoryModel(CGameDataHandler & iGameDataHandler, QObject * iParent)
  : QAbstractTableModel{iParent}
  , mGameDataHandler(iGameDataHandler)
{

}
