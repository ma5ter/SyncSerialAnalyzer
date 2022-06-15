#include "SyncSerialAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

#include <memory>

SyncSerialAnalyzerSettings::SyncSerialAnalyzerSettings()
    : clockChannel( UNDEFINED_CHANNEL ),
      clockPolarity( false ),
      dataChannel( UNDEFINED_CHANNEL ),
      dataPolarity( false ),
      clockSpace( 2 ),
      startBit( false ),
      stopBit( false ),
      bigEndian( false )
{
    clockInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    clockInterface->SetTitleAndTooltip( "Clock", "Clock for synchronous serial" );

    clockPolarityInterface = std::make_unique<AnalyzerSettingInterfaceBool>();
    clockPolarityInterface->SetTitleAndTooltip( "Inverted", "Clock polarity is inverted" );

    clockSpaceInterface = std::make_unique<AnalyzerSettingInterfaceInteger>();
    clockSpaceInterface->SetTitleAndTooltip( "Clock space", "Clock space in previous clocks to start a new symbol" );
    clockSpaceInterface->SetMin( 2 );
    clockSpaceInterface->SetMax( 10000 );

    dataInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    dataInterface->SetTitleAndTooltip( "Data", "Data for synchronous serial" );

    dataPolarityInterface = std::make_unique<AnalyzerSettingInterfaceBool>();
    dataPolarityInterface->SetTitleAndTooltip( "Inverted", "Data polarity is inverted" );

    startBitInterface = std::make_unique<AnalyzerSettingInterfaceBool>();
    startBitInterface->SetTitleAndTooltip( "Start bit", "Start bit" );

    stopBitInterface = std::make_unique<AnalyzerSettingInterfaceBool>();
    stopBitInterface->SetTitleAndTooltip( "Stop bit", "Stop bit" );

    bigEndianInterface = std::make_unique<AnalyzerSettingInterfaceBool>();
    bigEndianInterface->SetTitleAndTooltip( "Big-endian", "Big-endian" );

    SetInterfacesFromSettings();

    AddInterface( clockInterface.get() );
    AddInterface( clockPolarityInterface.get() );
    AddInterface( clockSpaceInterface.get() );
    AddInterface( dataInterface.get() );
    AddInterface( dataPolarityInterface.get() );
    AddInterface( startBitInterface.get() );
    AddInterface( stopBitInterface.get() );
    AddInterface( bigEndianInterface.get() );

    AddExportOption( 0, "Export as text/csv file" );
    AddExportExtension( 0, "text", "txt" );
    AddExportExtension( 0, "csv", "csv" );

    UpdateChannels( false );
}

SyncSerialAnalyzerSettings::~SyncSerialAnalyzerSettings() = default;

void SyncSerialAnalyzerSettings::UpdateChannels( bool is_used )
{
    ClearChannels();
    AddChannel( clockChannel, "Clock", is_used );
    AddChannel( dataChannel, "Data", is_used );
}

void SyncSerialAnalyzerSettings::SetInterfacesFromSettings()
{
    clockInterface->SetChannel( clockChannel );
    clockPolarityInterface->SetValue( clockPolarity );
    clockSpaceInterface->SetInteger( clockSpace );
    dataInterface->SetChannel( dataChannel );
    dataPolarityInterface->SetValue( dataPolarity );
    startBitInterface->SetValue( startBit );
    stopBitInterface->SetValue( stopBit );
    bigEndianInterface->SetValue( bigEndian );
}

bool SyncSerialAnalyzerSettings::SetSettingsFromInterfaces()
{
    clockChannel = clockInterface->GetChannel();
    clockPolarity = clockPolarityInterface->GetValue();
    clockSpace = clockSpaceInterface->GetInteger();
    dataChannel = dataInterface->GetChannel();
    dataPolarity = dataPolarityInterface->GetValue();
    startBit = startBitInterface->GetValue();
    stopBit = stopBitInterface->GetValue();
    bigEndian = bigEndianInterface->GetValue();

    UpdateChannels( true );

    return true;
}

void SyncSerialAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    text_archive >> clockChannel;
    text_archive >> clockPolarity;
    text_archive >> clockSpace;
    text_archive >> dataChannel;
    text_archive >> dataPolarity;
    text_archive >> startBit;
    text_archive >> stopBit;
    text_archive >> bigEndian;

    UpdateChannels( true );

    SetInterfacesFromSettings();
}

const char* SyncSerialAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << clockChannel;
    text_archive << clockPolarity;
    text_archive << clockSpace;
    text_archive << dataChannel;
    text_archive << dataPolarity;
    text_archive << startBit;
    text_archive << stopBit;
    text_archive << bigEndian;

    return SetReturnString( text_archive.GetString() );
}
