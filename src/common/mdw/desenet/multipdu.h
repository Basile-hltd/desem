#ifndef MULTIPDU_H
#define MULTIPDU_H

#include <iostream>
#include <vector>
#include "frame.h"
#include "platform-config.h"


class MultiPDU : public Frame {

    public:
        MultiPDU();

        bool Add_SV_ePDU(SvGroup group, SharedByteBuffer payload);
        bool Add_EV_ePDU(EvId EventId, SharedByteBuffer payload);

        bool Finalize();

        const uint8_t * data();

    private:
        uint8_t ePDU_count;
        std::vector<uint8_t> epdu_buffer;

};

#endif // MULTIPDU_H
