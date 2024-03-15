#include "PRenderBuffer.h"

std::vector<PRenderData> PRenderBuffer::GetReadBuffer()
{
	std::shared_lock sharedLock(wrMutex);
	return renderBuffer[readIndex];
}

void PRenderBuffer::WriteToBuffer(const PRenderData& data)
{
	std::shared_lock sharedLock(wrMutex);
	renderBuffer[writeIndex].push_back(data);
}

void PRenderBuffer::SwapPRenderBuffer()
{
	std::unique_lock sharedLock(wrMutex);

	readIndex = (readIndex + 1) % 2;
	writeIndex = (writeIndex + 1) % 2;

	renderBuffer[writeIndex].clear();
}
