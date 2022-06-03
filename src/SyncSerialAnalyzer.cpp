#include "SyncSerialAnalyzer.h"
#include "SyncSerialAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

SyncSerialAnalyzer::SyncSerialAnalyzer() : Analyzer2(), settings( new SyncSerialAnalyzerSettings() ), mSimulationInitilized( false )
{
    SetAnalyzerSettings( settings.get() );
    setLoHi( settings->clockPolarity, &bitClock0, &bitClock1 );
    setLoHi( settings->dataPolarity, &bitData0, &bitData1 );
}

SyncSerialAnalyzer::~SyncSerialAnalyzer()
{
    KillThread();
}

void SyncSerialAnalyzer::SetupResults()
{
    results.reset( new SyncSerialAnalyzerResults( this, settings.get() ) );
    SetAnalyzerResults( results.get() );
    results->AddChannelBubblesWillAppearOn( settings->clockChannel );
}

[[noreturn]] void SyncSerialAnalyzer::WorkerThread()
{
    sampleRateHz = GetSampleRate();

    clockChannelData = GetAnalyzerChannelData( settings->clockChannel );
    dataChannelData = GetAnalyzerChannelData( settings->dataChannel );

    int word_bit = 0;
    U64 clock_samples;
    U64 word_value;
    U64 frame_starting_sample;

    for( ;; )
    {
        CheckIfThreadShouldExit();

        if( !word_bit )
        {
            AdvanceToNext1( clockChannelData );
            frame_starting_sample = clockChannelData->GetSampleNumber();
            word_value = 0;
            clock_samples = 0;
        }

        U64 samples1 = AdvanceToNext0( clockChannelData );
        U64 falling_edge_sample_number = SyncPositions( clockChannelData, dataChannelData );
        U64 samples0 = AdvanceToNext1( clockChannelData );

        U64 bit;
        AnalyzerResults::MarkerType bit_display;
        AnalyzerResults::MarkerType clock_display = AnalyzerResults::Dot;

        if( dataChannelData->GetBitState() == bitData0 )
        {
            bit = 0;
            bit_display = AnalyzerResults::Zero;
        }
        else
        {
            bit = 1;
            bit_display = AnalyzerResults::One;
        }

        word_value <<= 1;
        word_value |= bit;
        word_bit++;

        if( !clock_samples )
        {
            clock_samples = samples1;
        }
        else if( clock_samples * settings->clockSpace < samples0 )
        {
            // we have a word to save
            if( true /* default stop bit */ )
            {
                word_value >>= 1;
                word_bit--;
            }

            Frame frame;
            frame.mType = 0;
            frame.mFlags = 0;
            frame.mData2 = word_bit;
            frame.mStartingSampleInclusive = ( S64 )frame_starting_sample;
            frame.mEndingSampleInclusive = ( S64 )falling_edge_sample_number;

            if( true /* default is little-endian */ )
            {
                U64 accumulator = 0;
                for( ; word_bit; word_bit-- )
                {
                    accumulator <<= 1;
                    if( word_value & 1ull )
                    {
                        accumulator |= 1ull;
                    }
                    word_value >>= 1;
                }
                word_value = accumulator;
            }
            else
            {
                word_bit = 0;
            }

            frame.mData1 = word_value;

            results->AddFrame( frame );
            results->CommitResults();
            ReportProgress( frame.mEndingSampleInclusive );

            bit_display = clock_display = AnalyzerResults::Stop;
        }

        U64 marker_sample_number = falling_edge_sample_number - ( samples1 >> 1 );

        results->AddMarker( marker_sample_number, clock_display, settings->clockChannel );
        results->AddMarker( marker_sample_number, bit_display, settings->dataChannel );
    }
}

U64 SyncSerialAnalyzer::SyncPositions( AnalyzerChannelData* from, AnalyzerChannelData* to )
{
    U64 sample_number = from->GetSampleNumber();
    to->AdvanceToAbsPosition( sample_number );
    return sample_number;
}

U64 SyncSerialAnalyzer::AdvanceToNext0( AnalyzerChannelData* channel_data ) const
{
    U64 sample_number = channel_data->GetSampleNumber();
    if( channel_data->GetBitState() == bitClock1 )
        channel_data->AdvanceToNextEdge();
    return channel_data->GetSampleNumber() - sample_number;
}

U64 SyncSerialAnalyzer::AdvanceToNext1( AnalyzerChannelData* channel_data ) const
{
    U64 sample_number = channel_data->GetSampleNumber();
    if( channel_data->GetBitState() == bitClock0 )
        channel_data->AdvanceToNextEdge();
    return channel_data->GetSampleNumber() - sample_number;
}

void SyncSerialAnalyzer::setLoHi( bool inverted, BitState* bit0, BitState* bit1 )
{
    if( inverted )
    {
        *bit0 = BIT_HIGH;
        *bit1 = BIT_LOW;
    }
    else
    {
        *bit0 = BIT_LOW;
        *bit1 = BIT_HIGH;
    }
}

BitState SyncSerialAnalyzer::getLoHi( bool inverted, BitState state )
{
    if( inverted )
    {
        return state == BIT_LOW ? BIT_HIGH : BIT_LOW;
    }
    else
    {
        return state == BIT_HIGH ? BIT_HIGH : BIT_LOW;
    }
}

bool SyncSerialAnalyzer::NeedsRerun()
{
    return false;
}

U32 SyncSerialAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate,
                                                SimulationChannelDescriptor** simulation_channels )
{
    if( mSimulationInitilized == false )
    {
        mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), settings.get() );
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SyncSerialAnalyzer::GetMinimumSampleRateHz()
{
    return 0; // settings->mBitRate * 4;
}

const char* SyncSerialAnalyzer::GetAnalyzerName() const
{
    return "Synchronous Serial Analyzer";
}

const char* GetAnalyzerName()
{
    return "Synchronous Serial Analyzer";
}

Analyzer* CreateAnalyzer()
{
    return new SyncSerialAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}