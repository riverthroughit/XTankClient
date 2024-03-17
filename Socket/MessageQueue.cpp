#include "MessageQueue.h"
#include "Socket/SocketClient.h"

void MsgSendQueue::SendLoginReq()
{

	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();

	XTankMsg::LoginReq loginReqMsg;
	loginReqMsg.mutable_playerid()->CopyFrom(playerId);

	PushMsg({ XTankMsg::LOGIN_REQ ,std::make_shared<XTankMsg::LoginReq>(loginReqMsg) });
}

void MsgSendQueue::SendLogoutReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::LogoutReq logoutReq;
	logoutReq.mutable_playerid()->CopyFrom(playerId);

	PushMsg({XTankMsg::LOGOUT_REQ ,std::make_shared<XTankMsg::LogoutReq>(logoutReq)});
}


void MsgSendQueue::SendJoinRoomReq(int roomId)
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::RoomJoinReq joinReq;
	joinReq.set_roomid(roomId);
	joinReq.mutable_playerid()->CopyFrom(playerId);

	PushMsg({ XTankMsg::ROOM_JOIN_REQ ,std::make_shared<XTankMsg::RoomJoinReq>(joinReq) });
}

void MsgSendQueue::SendCreateRoomReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::RoomCreateReq createReq;
	createReq.mutable_playerid()->CopyFrom(playerId);

	PushMsg({XTankMsg::ROOM_CREATE_REQ ,std::make_shared<XTankMsg::RoomCreateReq>(createReq)});
}

void MsgSendQueue::SendPlayerReadyReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::PlayerReadyReq req;
	req.mutable_playerid()->CopyFrom(playerId);

	PushMsg({XTankMsg::PLAYER_READY_REQ ,std::make_shared<XTankMsg::PlayerReadyReq>(req)});
}

void MsgSendQueue::SendPlayerCutInReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::PlayerCutInReq req;
	req.mutable_playerid()->CopyFrom(playerId);

	PushMsg({ XTankMsg::PLAYER_CUT_IN_REQ ,std::make_shared<XTankMsg::PlayerCutInReq>(req) });
}

void MsgSendQueue::SendPlayerReadyCancelReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::PlayerReadyCancelReq req;
	req.mutable_playerid()->CopyFrom(playerId);

	PushMsg({XTankMsg::PLAYER_READY_CANCEL_REQ ,std::make_shared<XTankMsg::PlayerReadyCancelReq>(req)});
}

void MsgSendQueue::SendExitRoomReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::RoomExitReq exitReq;
	exitReq.mutable_playerid()->CopyFrom(playerId);

	PushMsg({XTankMsg::ROOM_EXIT_REQ ,std::make_shared<XTankMsg::RoomExitReq>(exitReq)});
}

void MsgSendQueue::SendGameReadyAck()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::GameReadyAck ack;
	ack.mutable_playerid()->CopyFrom(playerId);

	PushMsg({ XTankMsg::GAME_READY_ACK ,std::make_shared<XTankMsg::GameReadyAck>(ack) });
}

void MsgSendQueue::SendPlayerInputNtf(int frameId, int cmdId)
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::PlayerInputNtf inputNtf;
	inputNtf.mutable_playerid()->CopyFrom(playerId);

	inputNtf.set_frameid(frameId);
	inputNtf.set_playercmd(cmdId);

	PushMsg({XTankMsg::PLAYER_INPUT_NTF ,std::make_shared<XTankMsg::PlayerInputNtf>(inputNtf)});
}

void MsgSendQueue::SendPlayerChaseUpNtf()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::PlayerChaseUpNtf ntf;
	ntf.mutable_playerid()->CopyFrom(playerId);

	PushMsg({ XTankMsg::PLAYER_CHASE_UP_NTF ,std::make_shared<XTankMsg::PlayerChaseUpNtf>(ntf) });
}

void MsgSendQueue::SendExitGameReq()
{
	XTankMsg::PlayerId playerId = SocketClient::Instance().GetPlayerId();
	XTankMsg::PlayerExitReq exitReq;
	exitReq.mutable_playerid()->CopyFrom(playerId);

	PushMsg({XTankMsg::PLAYER_EXIT_REQ ,std::make_shared<XTankMsg::PlayerExitReq>(exitReq)});
}
