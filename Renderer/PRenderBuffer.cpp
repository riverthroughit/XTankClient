#include "PRenderBuffer.h"

std::pair<unsigned int, std::pair<BufferMap, BufferMap>> PRenderBuffer::GetFrameIdAndReadBuffers()
{
	std::shared_lock sharedLock(wrMutex);
	return { frameId,{ renderBuffer[preIndex],renderBuffer[curIndex] } };
}

void PRenderBuffer::WriteToBuffer(const PRenderData& data)
{
	std::shared_lock sharedLock(wrMutex);
	renderBuffer[writeIndex].insert({ data.entityId,data });
}

void PRenderBuffer::SwapPRenderBuffer()
{
	std::unique_lock sharedLock(wrMutex);

	preIndex = (preIndex + 1) % 3;
	curIndex = (curIndex + 1) % 3;
	writeIndex = (writeIndex + 1) % 3;

	renderBuffer[writeIndex].clear();
}
