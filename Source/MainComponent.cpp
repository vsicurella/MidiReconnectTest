#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    textBox.reset(new TextEditor());
    textBox->setReadOnly(true);
    textBox->setMultiLine(true);
    textBox->setReturnKeyStartsNewLine(true);
    addAndMakeVisible(textBox.get());
    
    setSize (600, 400);
    
    startTimer(750);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    textBox->setBounds(getLocalBounds().withTop(getHeight() * 0.5));
}

void MainComponent::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    msgQueue.addEvent(message, -1);
}

void MainComponent::timerCallback()
{
    auto availableInputs = MidiInput::getAvailableDevices();
    if (availableInputs != inputDevices)
    {
        Array<MidiDeviceInfo> newDevices;
        for (auto device : availableInputs)
            if (!inputDevices.contains(device))
            {
                inputDevices.add(device);
                auto newInput = inputDevicesOpened.add(MidiInput::openDevice(device.identifier, this));
                newInput->start();
                textBox->insertTextAtCaret("\nConnected to " + device.name);
            }
        
        for (auto device : inputDevices)
        {
            if (!availableInputs.contains(device))
            {
                textBox->insertTextAtCaret("\nLost connection to " + device.name);
                inputDevices.removeFirstMatchingValue(device);
                
                for (auto openInput: inputDevicesOpened)
                {
                    if (openInput->getDeviceInfo() == device)
                    {
                        openInput->stop();
                        inputDevicesOpened.removeObject(openInput);
                    }
                }
            }
        }
    }
    
    if (msgQueue.getNumEvents() > 0)
    {
        for (auto event : msgQueue)
            textBox->insertTextAtCaret("\n" + event.getMessage().getDescription());
        
        msgQueue.clear();
    }
}
