#include "ECS/Util/ECSUtil.h"

#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/RollbackComponent.h"


bool IsCmdBufferEmpty(SocketComponent& socketComp)
{
	return socketComp.playersCmdsBuffer.empty();
}

bool HasCurCmd(SocketComponent& socketComp)
{
	return socketComp.hasCurCmd;
}

PlayersCommand GetSocketCurCmd(SocketComponent& socketComp)
{
	if (socketComp.hasCurCmd) {
		socketComp.hasCurCmd = false;
		return socketComp.curPlayersCmd;
	}
	
	return PlayersCommand{};
}

bool IsReachPredictLimit(RollbackComponent& rollbackComp)
{
	return rollbackComp.predCmdDeq.size() >= rollbackComp.maxPredictTimes;
}


