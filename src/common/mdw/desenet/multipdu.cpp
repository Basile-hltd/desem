#include "multipdu.h"

MultiPDU::MultiPDU() : Frame(Frame::Mtu) {

    ePDU_count = 0;
    epdu_buffer.clear();

    setDestination(GATEWAY_ADDRESS);
    setType(FrameType::MPDU);

    std::uint8_t * pbuffer = buffer();

    pbuffer[HEADER_SIZE + 0] = pbuffer[HEADER_SIZE + 0] & 0x80; //clear 7 lsb in the first frame byte
    pbuffer[HEADER_SIZE + 0] = pbuffer[HEADER_SIZE + 0] | DEFAULT_DESENET_SLOT_NUMBER; //set 7 lsb in the first frame byte (sensor id)

    pbuffer[HEADER_SIZE + 1] = 0x00; // no ePDU for now

}

bool MultiPDU::Add_SV_ePDU(SvGroup group, SharedByteBuffer payload){

    if(HEADER_SIZE + 2 + epdu_buffer.size() + payload.size() > reservedLength()){
        return false;
    }

    uint8_t epdu_header = 0x00 | group << 3 | payload.size();
    epdu_buffer.push_back(epdu_header);

    for(size_t i=0; i < payload.size(); i++){
        epdu_buffer.push_back(payload[i]);
    }

    ePDU_count++;

    return true;
}

bool MultiPDU::Add_EV_ePDU(EvId EventId, SharedByteBuffer payload){

    if(HEADER_SIZE + 2 + epdu_buffer.size() + payload.size() > reservedLength()){
        return false;
    }

    uint8_t epdu_header = 0x80 | EventId << 3 | payload.size();
    epdu_buffer.push_back(epdu_header);

    for(size_t i=0; i < payload.size(); i++){
        epdu_buffer.push_back(payload[i]);
    }

    ePDU_count++;

    return true;
}

bool MultiPDU::Finalize(){

    if(HEADER_SIZE + 2 + epdu_buffer.size() > reservedLength()){
        return false;
    }

    std::uint8_t * pbuffer = buffer();

    setLength(HEADER_SIZE + 2 + epdu_buffer.size());

    pbuffer[HEADER_SIZE + 1] = ePDU_count; // set ePDU count

    for (size_t i = 0; i < epdu_buffer.size(); i++) {
        pbuffer[HEADER_SIZE + 2 + i] = epdu_buffer[i];
    }

    return true;
}

const std::uint8_t * MultiPDU::data(){
    return buffer();
}
