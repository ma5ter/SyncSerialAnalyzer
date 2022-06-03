#ifndef SYNCSERIAL_ANALYZER_SETTINGS
#define SYNCSERIAL_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SyncSerialAnalyzerSettings : public AnalyzerSettings
{
public:
	SyncSerialAnalyzerSettings();
	virtual ~SyncSerialAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //SYNCSERIAL_ANALYZER_SETTINGS
