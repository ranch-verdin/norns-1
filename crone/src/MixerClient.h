//
// Created by emb on 11/28/18.
//

#ifndef CRONE_MIXERCLIENT_H
#define CRONE_MIXERCLIENT_H

#include "Client.h"
#include "Utilities.h"
#include "Bus.h"
#include "effects/StereoCompressor.h"
#include "effects/ZitaReverb.h"

namespace  crone {
    class MixerClient: public Client<6, 6> {

    public:
        enum { MAX_BUF_SIZE = 2048 };
        typedef enum { SOURCE_ADC=0, SOURCE_CUT=1, SOURCE_EXT=2 } SourceId;
        typedef enum { SINK_DAC=0, SINK_CUT=1, SINK_EXT=2 } SinkId;
        typedef Bus<2, MAX_BUF_SIZE> StereoBus;

    public:
        MixerClient();
        void handleCommand(Commands::CommandPacket *p) override;
    private:
        void process(jack_nframes_t numFrames) override;
        void setSampleRate(jack_nframes_t) override;
    private:
        void processFx(size_t numFrames);
        void setFxDefaults();
    private:
        // processors
        StereoCompressor comp;
        ZitaReverb reverb;

        // busses
        struct BusList {
            // sources
            StereoBus adc_source;
            StereoBus cut_source;
            StereoBus ext_source;
            // sinks
            StereoBus ext_sink;
            StereoBus cut_sink;
            StereoBus dac_sink;
            // fx I/O
            StereoBus ins_in;
            StereoBus ins_out;
            StereoBus aux_in;
            StereoBus aux_out;
            // monitor mix
            StereoBus adc_monitor;
            BusList();
        };
        BusList bus;

        // levels
        struct SmoothLevelList{
        public:
            // "master" I/O levels
            LogRamp adc;
            LogRamp dac;
            LogRamp ext;
            LogRamp cut;
            LogRamp monitor;
            // softcut input levels
            LogRamp adc_cut;
            LogRamp ext_cut;
            // aux send levels
            LogRamp monitor_aux;
            LogRamp cut_aux;
            LogRamp ext_aux;
            // FX return / mix levels
            LogRamp aux;
            LogRamp ins_mix;
            SmoothLevelList();
            void setSampleRate(float sr);
        };
        SmoothLevelList smoothLevels;

        struct StaticLevelList {
            // 2x2 matrix for monitor mix
            float monitor_mix[4];
            StaticLevelList();
        };
        StaticLevelList staticLevels;

        // other state
        struct EnabledList {
            bool reverb;
            bool comp;
            EnabledList();
        };
        EnabledList enabled;


    };
}

#endif //CRONE_MIXERCLIENT_H