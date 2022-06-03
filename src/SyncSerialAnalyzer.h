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
    ~SyncSerialAnalyzer() override;

    void SetupResults() override;
    [[noreturn]] void WorkerThread() override;

    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels ) override;
    U32 GetMinimumSampleRateHz() override;

    const char* GetAnalyzerName() const override;
    bool NeedsRerun() override;

    static void setLoHi( bool inverted, BitState* bit0, BitState* bit1 );
    static U64 SyncPositions( AnalyzerChannelData* from, AnalyzerChannelData* to );
    static BitState getLoHi( bool inverted, BitState state );

  protected: // vars
    std::unique_ptr<SyncSerialAnalyzerSettings> settings;
    std::unique_ptr<SyncSerialAnalyzerResults> results;
    AnalyzerChannelData* clockChannelData;
    AnalyzerChannelData* dataChannelData;

    SyncSerialSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    // Serial analysis vars:
    U32 sampleRateHz;
    U32 mStartOfStopBitOffset;
    U32 mEndOfStopBitOffset;
    BitState bitClock0;
    BitState bitClock1;
    BitState bitData0;
    BitState bitData1;

  private:
    U64 AdvanceToNext0( AnalyzerChannelData* channel_data ) const;
    U64 AdvanceToNext1( AnalyzerChannelData* channel_data ) const;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif // SYNCSERIAL_ANALYZER_H
