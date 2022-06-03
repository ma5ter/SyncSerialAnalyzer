#ifndef SYNCSERIAL_ANALYZER_SETTINGS
#define SYNCSERIAL_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SyncSerialAnalyzerSettings : public AnalyzerSettings
{
  public:
    SyncSerialAnalyzerSettings();
    ~SyncSerialAnalyzerSettings() override;

    bool SetSettingsFromInterfaces() override;
    void UpdateInterfaces();
    void LoadSettings( const char* settings ) override;
    const char* SaveSettings() override;

    Channel clockChannel;
    Channel dataChannel;
    bool clockPolarity;
    bool dataPolarity;

    int clockSpace;

  protected:
    std::unique_ptr<AnalyzerSettingInterfaceChannel> clockInterface;
    std::unique_ptr<AnalyzerSettingInterfaceBool> clockPolarityInterface;
    std::unique_ptr<AnalyzerSettingInterfaceInteger> clockSpaceInterface;
    std::unique_ptr<AnalyzerSettingInterfaceChannel> dataInterface;
    std::unique_ptr<AnalyzerSettingInterfaceBool> dataPolarityInterface;
    void UpdateChannels( bool is_used );
};

#endif // SYNCSERIAL_ANALYZER_SETTINGS
