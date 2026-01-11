#include <assert.h>
#include <array>
#include <list>
#include <map>
#include <vector>
#include <utility>
#include "platform-config.h"
#include "board/ledcontroller.h"
#include "desenet/frame.h"
#include "desenet/beacon.h"
#include "desenet/timeslotmanager.h"
#include "abstractapplication.h"
#include "networkentity.h"

using std::array;
using std::list;
using std::map;
using std::make_pair;
using std::bind;
using std::pair;
using std::vector;

using desenet::sensor::NetworkEntity;

NetworkEntity * NetworkEntity::_pInstance(nullptr);		// Instantiation of static attribute

NetworkEntity::NetworkEntity()
 : _pTimeSlotManager(nullptr),
   _pTransceiver(nullptr)
{
	assert(!_pInstance);		// Only one instance allowed
	_pInstance = this;
}

NetworkEntity::~NetworkEntity()
{
}

void NetworkEntity::initialize()
{
    applicationSyncList.clear();
    applicationPublishersArray.fill(nullptr);
}

void NetworkEntity::svSyncRequest(AbstractApplication* pAbstractApplication){
    applicationSyncList.push_back(pAbstractApplication);
}

void NetworkEntity::svPublishRequest(AbstractApplication* pAbstractApplication, SvGroup group){
    applicationPublishersArray[0] = pAbstractApplication;
    this->group = group;
}

void NetworkEntity::evPublishRequest(EvId id, const SharedByteBuffer & evData){
    eventElementList.push_back(EventElement(id, evData));
}

void NetworkEntity::initializeRelations(ITimeSlotManager & timeSlotManager, NetworkInterfaceDriver & transceiver)
{
	_pTimeSlotManager = &timeSlotManager;
    _pTransceiver = &transceiver;

    // TODO: Add additional initialization code here
    _pTimeSlotManager->setObserver(*this);

     // Set the receive callback between transceiver and network. Bind this pointer to member function
    transceiver.setReceptionHandler(std::bind(&NetworkEntity::onReceive, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

/**
 * This method does not automatically create an instance if there is none created so far.
 * It is up the the developer to create an instance of this class prior to access the instance() method.
 */
//static
NetworkEntity & NetworkEntity::instance()
{
	assert(_pInstance);
    return *_pInstance;
}

SlotNumber NetworkEntity::slotNumber() const
{
    return timeSlotManager().slotNumber();
}

void NetworkEntity::onTimeSlotSignal(const ITimeSlotManager & timeSlotManager, const ITimeSlotManager::SIG & signal)
{

    (void)timeSlotManager;

    switch (signal)
    {
    case ITimeSlotManager::CYCLE_START:
        std::cout << "cycle start" << std::endl;
        break;
    case ITimeSlotManager::OWN_SLOT_START:
    {
        std::cout << "slot start : number -> " << (int)(slotNumber()) << std::endl;

        SharedByteBuffer data(6);
        applicationPublishersArray[0]->svPublishIndication(group, data);

        MultiPDU mpdu_frame = MultiPDU();

        if(!mpdu_frame.Add_SV_ePDU(group, data)){
            std::cout << "MPDU_Frame Add_SV_ePDU error" <<std::endl;
        }

        while (!eventElementList.empty()) {
            EventElement front_element = eventElementList.front();

            if(!mpdu_frame.Add_EV_ePDU(front_element.id, front_element.data)){
                //mdpu frame is full
                break;
            }

            eventElementList.pop_front();

        }

        eventElementList.clear();

        if(!mpdu_frame.Finalize()){
            std::cout << "MPDU_Frame Finalize error" <<std::endl;
            return;
        }

        if(mpdu_frame.type() == FrameType::MPDU){
            _pTransceiver->transmit(mpdu_frame.data(), mpdu_frame.length());
        }
    }
    break;
    case ITimeSlotManager::OWN_SLOT_FINISH:
        std::cout << "slot end" << std::endl;
        break;
    case ITimeSlotManager::CYCLE_FINISH:
        std::cout << "cycle finish" << std::endl;
        break;
    }
}

/**
 * Called by the NetworkInterfaceDriver (layer below) after reception of a new frame
 */
void NetworkEntity::onReceive(NetworkInterfaceDriver & driver, const uint32_t receptionTime, const uint8_t * const buffer, size_t length)
{
    (void)(driver);
    (void)(receptionTime);
	Frame frame = Frame::useBuffer(buffer, length);

    desenet::FrameType frameType = frame.type();

    // TODO: Add your code here
    switch(frameType){
    case desenet::FrameType::Beacon:
        {
            desenet::Beacon beacon = desenet::Beacon(frame);
            timeSlotManager().onBeaconReceived(beacon.slotDuration());

            auto iterator = applicationSyncList.begin();

            while (iterator != applicationSyncList.end()) {
                (*iterator)->svSyncIndication(receptionTime);

                iterator++;
            }

        }
        break;
    case desenet::FrameType::Invalid:

        break;
    case desenet::FrameType::MPDU:

        break;
    }
}

board::LedController & NetworkEntity::ledController() const
{
    return board::LedController::instance();
}
