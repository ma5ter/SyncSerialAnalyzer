#ifndef SYNCSERIAL_SIMULATION_DATA_GENERATOR
#define SYNCSERIAL_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
#include "AnalyzerHelpers.h"
class SyncSerialAnalyzerSettings;

class SyncSerialSimulationDataGenerator
{
  public:
    SyncSerialSimulationDataGenerator();
    ~SyncSerialSimulationDataGenerator();

    void Initialize( U32 simulation_sample_rate, SyncSerialAnalyzerSettings* settings );
    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );

  protected:
    SyncSerialAnalyzerSettings* mSettings;
    U32 mSimulationSampleRateHz;

  private:
    ClockGenerator mClockGenerator;

    SimulationChannelDescriptorGroup channels;
    SimulationChannelDescriptor clockData;
    SimulationChannelDescriptor dataData;
    BitState bitClock0;
    BitState bitClock1;
    BitState bitData0;
    BitState bitData1;

};
#endif // SYNCSERIAL_SIMULATION_DATA_GENERATOR