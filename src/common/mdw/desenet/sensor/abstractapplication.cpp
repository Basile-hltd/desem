#include "desenet/sensor/net.h"
#include "abstractapplication.h"

using desenet::sensor::AbstractApplication;

/*********************************************************************************************************
* TODO: Implement the protected methods of the AbstractApplication class here. From these you have to    *
* call the different methods on your DESENET Controller Entity implementation...                         *
*********************************************************************************************************/

// Default implementations.

void AbstractApplication::svSyncRequest()
{
    // TODO: Register application using the network entity
    Net::instance().entity().svSyncRequest(this);

    return;
}

bool AbstractApplication::svPublishRequest(SvGroup group)
{
    // TODO: Register application for the provided group using the network entity
    Net::instance().entity().svPublishRequest(this, group);

    return false;
}

void AbstractApplication::evPublishRequest(EvId id, const SharedByteBuffer & evData)
{
    // TODO: Publish event data
    Net::instance().entity().evPublishRequest(id, evData);

    return;
}

/**
 * Default callback method for syncs
 */
void AbstractApplication::svSyncIndication(desenet::NetworkTime nt)
{
    return;
}

/**
 * Default callback method for SV publishes
 */
SharedByteBuffer::sizeType AbstractApplication::svPublishIndication(SvGroup, SharedByteBuffer &)
{
	return 0;
}
