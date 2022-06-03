#ifndef SYNCSERIAL_SIMULATION_DATA_GENERATOR
#define SYNCSERIAL_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class SyncSerialAnalyzerSettings;

class SyncSerialSimulationDataGenerator
{
public:
	SyncSerialSimulationDataGenerator();
	~SyncSerialSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, SyncSerialAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	SyncSerialAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //SYNCSERIAL_SIMULATION_DATA_GENERATOR