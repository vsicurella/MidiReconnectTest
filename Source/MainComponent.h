#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, juce::MidiInputCallback, Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
    void timerCallback() override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    
    OwnedArray<MidiInput> inputDevicesOpened;
    Array<MidiDeviceInfo> inputDevices;
    
    MidiBuffer msgQueue;

    std::unique_ptr<TextEditor> textBox;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
