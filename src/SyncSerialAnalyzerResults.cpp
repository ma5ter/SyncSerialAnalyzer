#include "SyncSerialAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "SyncSerialAnalyzer.h"
#include "SyncSerialAnalyzerSettings.h"
#include <iostream>
#include <fstream>

SyncSerialAnalyzerResults::SyncSerialAnalyzerResults( SyncSerialAnalyzer* analyzer, SyncSerialAnalyzerSettings* settings )
    : AnalyzerResults(), mSettings( settings ), mAnalyzer( analyzer )
{
}

SyncSerialAnalyzerResults::~SyncSerialAnalyzerResults() = default;

void SyncSerialAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
    ClearResultStrings();
    Frame frame = GetFrame( frame_index );

    char number_str[ 128 ];
    AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str, sizeof( number_str ) );
    AddResultString( number_str );
}

void SyncSerialAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
    std::ofstream file_stream( file, std::ios::out );

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    file_stream << "Time [s],Value" << std::endl;

    U64 num_frames = GetNumFrames();
    for( U32 i = 0; i < num_frames; i++ )
    {
        Frame frame = GetFrame( i );

        char time_str[ 128 ];
        AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, sizeof( time_str ) );

        char number_str[ 128 ];
        AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str, sizeof( number_str ) );

        file_stream << time_str << "," << number_str << std::endl;

        if( UpdateExportProgressAndCheckForCancel( i, num_frames ) )
        {
            file_stream.close();
            return;
        }
    }

    file_stream.close();
}

void SyncSerialAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
    Frame frame = GetFrame( frame_index );
    ClearTabularText();

    char number_str[ 128 ];
    AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str, sizeof( number_str ) );
    AddTabularText( number_str );
#endif
}

void SyncSerialAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
    // not supported
}

void SyncSerialAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
    // not supported
}