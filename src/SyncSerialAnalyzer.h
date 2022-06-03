#ifndef SYNCSERIAL_ANALYZER_H
#define SYNCSERIAL_ANALYZER_H

#include <Analyzer.h>
#include "SyncSerialAnalyzerResults.h"
#include "SyncSerialSimulationDataGenerator.h"

class SyncSerialAnalyzerSettings;
class ANALYZER_EXPORT SyncSerialAnalyzer : public Analyzer2
{
public:
	SyncSerialAnalyzer();
	virtual ~SyncSerialAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< SyncSerialAnalyzerSettings > mSettings;
	std::auto_ptr< SyncSerialAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	SyncSerialSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //SYNCSERIAL_ANALYZER_H
