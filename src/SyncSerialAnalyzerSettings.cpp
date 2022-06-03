#include "SyncSerialAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

#include <memory>

void SyncSerialAnalyzerSettings::UpdateInterfaces()
{
    clockInterface->SetChannel( clockChannel );
    clockPolarityInterface->SetValue( clockPolarity );
    clockSpaceInterface->SetInteger( clockSpace );
    dataInterface->SetChannel( dataChannel );
    dataPolarityInterface->SetValue( dataPolarity );
}

void SyncSerialAnalyzerSettings::UpdateChannels( bool is_used )
{
    ClearChannels();
    AddChannel( clockChannel, "Clock", is_used );
    AddChannel( dataChannel, "Data", is_used );
}

SyncSerialAnalyzerSettings::SyncSerialAnalyzerSettings()
    : clockChannel( UNDEFINED_CHANNEL ), clockPolarity( false ), dataChannel( UNDEFINED_CHANNEL ), dataPolarity( false ), clockSpace( 3 )
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

    UpdateInterfaces();

    AddInterface( clockInterface.get() );
    AddInterface( clockPolarityInterface.get() );
    AddInterface( clockSpaceInterface.get() );
    AddInterface( dataInterface.get() );
    AddInterface( dataPolarityInterface.get() );

    AddExportOption( 0, "Export as text/csv file" );
    AddExportExtension( 0, "text", "txt" );
    AddExportExtension( 0, "csv", "csv" );

    UpdateChannels( false );
}

SyncSerialAnalyzerSettings::~SyncSerialAnalyzerSettings() = default;

bool SyncSerialAnalyzerSettings::SetSettingsFromInterfaces()
{
    clockChannel = clockInterface->GetChannel();
    clockPolarity = clockPolarityInterface->GetValue();
    clockSpace = clockSpaceInterface->GetInteger();
    dataChannel = dataInterface->GetChannel();
    dataPolarity = dataPolarityInterface->GetValue();

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

    UpdateChannels( true );

    UpdateInterfaces();
}

const char* SyncSerialAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << clockChannel;
    text_archive << clockPolarity;
    text_archive << clockSpace;
    text_archive << dataChannel;
    text_archive << dataPolarity;

    return SetReturnString( text_archive.GetString() );
}
