#include "SyncSerialSimulationDataGenerator.h"
#include "SyncSerialAnalyzerSettings.h"
#include "SyncSerialAnalyzer.h"

#include <AnalyzerHelpers.h>

SyncSerialSimulationDataGenerator::SyncSerialSimulationDataGenerator() = default;

SyncSerialSimulationDataGenerator::~SyncSerialSimulationDataGenerator() = default;

void SyncSerialSimulationDataGenerator::Initialize( U32 simulation_sample_rate, SyncSerialAnalyzerSettings* settings )
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    SyncSerialAnalyzer::setLoHi( settings->clockPolarity, &bitClock0, &bitClock1 );
    SyncSerialAnalyzer::setLoHi( settings->dataPolarity, &bitData0, &bitData1 );

    channels.Add( mSettings->clockChannel, simulation_sample_rate, bitClock0 );
    channels.Add( mSettings->dataChannel, simulation_sample_rate, bitData0 );

    mClockGenerator.Init( 100000, simulation_sample_rate );
}

U32 SyncSerialSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate,
                                                               SimulationChannelDescriptor** simulation_channels )
{
    U64 adjusted_largest_sample_requested =
        AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

    while( clockData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
    {
        int bits = rand() % 64 + 1;
        int blank = rand() % 100 + mSettings->clockSpace + 1;
        U64 value = ( ( U64 )rand() << 32 ) | ( ( U64 )rand() );

        channels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( blank ) );

        while( bits-- )
        {
            BitState bit = value & 1ull ? bitData1 : bitData0;
            value >>= 1;
            dataData.TransitionIfNeeded(bit);
            channels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 0.25f ) );
            clockData.Transition();
            channels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 0.5 ) );
            clockData.Transition();
            channels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 0.25f ) );
        }
        dataData.TransitionIfNeeded(bitData0);
    }

    *simulation_channels = channels.GetArray();
    return channels.GetCount();
}
