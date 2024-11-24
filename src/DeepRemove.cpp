#include "DeepRemove.h"

#include <string>

#include "DDImage/DeepFilterOp.h"
#include "DDImage/Knobs.h"

#include "DDImage/ChannelSet.h"
#include "DDImage/Channel.h"

#include <iostream>

using namespace DD::Image;

DeepRemove::DeepRemove(Node* node) : DeepFilterOp(node)
  , _operationKnob(0)
  , _customChannelsCountKnob(0)
  , _mainChannelsSetKnob(Mask_All)
  , _dynamicChannelsKnob(MAXCUSTOMKNOBS, Chan_Black)
  , _numDynamicKnobs(0)
  , _showWarning(false)
{
    inputs(1);
}

const char* DeepRemove::node_help() const {
    return ("Removes color channels from the deep image.\n" +
        std::string(CLASS) + "-" + getVersionInfoStr(VERSION)).c_str();
}

std::string DeepRemove::docsPath() const {
    return "https://github.com/JorgeHI/DeepRemove";
}

void DeepRemove::knobs(Knob_Callback f)
{
    Enumeration_knob(f, &_operationKnob, operationKnobNames, "operation");
    // Hidden knob to store the number of active custom channels
    Int_knob(f, &_customChannelsCountKnob, "customChannelsCount");
    SetFlags(f, Knob::INVISIBLE);
    // Default channelSet knob (This one can't be removed)
    Input_ChannelSet_knob(f, &_mainChannelsSetKnob, 0, "channels");
    // Button to add a new channelSet knob
    Button(f, "addChannel", "<b>+</b>");     
    SetFlags(f, Knob::KNOB_CHANGED_ALWAYS | Knob::SMALL_UI);
    // Button to remove the last custom channelSet knob
    Button(f, "removeChannel", "<b>-</b>");  
    SetFlags(f, Knob::KNOB_CHANGED_ALWAYS | Knob::SMALL_UI);
    ClearFlags(f, Knob::NO_RERENDER);  // Remove a channel knob affects to the render

    for (int i = 0; i < _dynamicChannelsKnob.size(); ++i) {
        // The first channel knob already exist. we start from idx 2
        std::string knobName = "channels" + std::to_string(i + 2);
        Input_ChannelSet_knob(f, &_dynamicChannelsKnob[i], 0, knobName.c_str(), "and");
        if (f.makeKnobs()) {
            SetFlags(f, Knob::HIDDEN);
        }
    }

    if (f.storeKnobs()) {
        _refreshDynamicChannels();
        DeepRemove::addWarningKnob(this->firstOp(), f);
    }
}

int DeepRemove::knob_changed(Knob* k) {
    if (k->is("addChannel")) {  // Add button pressed -> Show one more channel knob
        if (_customChannelsCountKnob < _dynamicChannelsKnob.size()) {
            _customChannelsCountKnob++;
            // Save the value in the knob
            knob("customChannelsCount")->set_value(_customChannelsCountKnob);
            _showWarning = false;
        }
        else {
            _showWarning = true;
        }
    }
    if (k->is("removeChannel")) {  // Remove button pressed -> Show one less channel knob
        if (_customChannelsCountKnob > 0) {
            _customChannelsCountKnob--;
            // Save the value in the knob
            knob("customChannelsCount")->set_value(_customChannelsCountKnob);
            // Reset the knob
            std::string knobName = "channels" + std::to_string(_customChannelsCountKnob + 2);
            knob(knobName.c_str())->reset_to_default();
            _showWarning = false;
        }
    }
    if (k == &Knob::showPanel || k->is("addChannel") || k->is("removeChannel")) {
        // Update visibility of custom channels knobs
        _refreshDynamicChannels();
        // Get last custom channel knobs
        std::string knobName = "channels" + std::to_string(MAXCUSTOMKNOBS + 1);
        // Add warning knob when required
        _numDynamicKnobs = replace_knobs(
            knob(knobName.c_str()), _numDynamicKnobs, addWarningKnob, this->firstOp());
        return 1;
    }
    return DeepFilterOp::knob_changed(k);
}

void DeepRemove::_refreshDynamicChannels() {
    int customChannelsCount = getCustomChannelsCount();
    for (int i = 0; i < _dynamicChannelsKnob.size(); ++i) {
        std::string knobName = "channels" + std::to_string(i + 2);
        if (i < customChannelsCount) {
            knob(knobName.c_str())->show();
        }
        else {
            knob(knobName.c_str())->hide();
        }
    }
}

void DeepRemove::addWarningKnob(void* p, Knob_Callback f) {
    if (((DeepRemove*)p)->_showWarning) {
        // Create a warning message knob
        std::string message = "<font color=red><b>You can not have more than "
            + std::to_string(MAXCUSTOMKNOBS) + " additional channels knobs.</b>";
        Text_knob(f, message.c_str());
        SetFlags(f, Knob::STARTLINE);
    }
}

void DeepRemove::_validate(bool for_real)
{
    if (input0() != nullptr) {
        // Validate connected input
        input0()->validate(for_real);
        // Store the available channels from the deep input.
        auto inChannels = input0()->deepInfo().channels();
        // Computed output channels by the user selection and operation mode
        auto outChannels = getOutputChannels(inChannels);
        // Build deep info to define output channels
        _deepInfo = DeepInfo(
            input0()->deepInfo().formats(),
            input0()->deepInfo().box(),
            outChannels
        );
    }
    else {
        _deepInfo = DeepInfo();
    }
}

ChannelSet DeepRemove::getSelectedChannels() {
    auto selectedChannels = _mainChannelsSetKnob;
    // Append extra channels knobs:
    for (int i = 0; i < _customChannelsCountKnob; ++i) {
        // Active custom channel knob (it's show) -> append channels
        selectedChannels += _dynamicChannelsKnob[i];
    }
    return selectedChannels;
}

ChannelSet DeepRemove::getOutputChannels(ChannelSet inChannels) {
    auto selectedChannels = getSelectedChannels();
    ChannelSet outputChannels = inChannels;
    if (_operationKnob == 0) { // Remove
        outputChannels -= inChannels.intersection(selectedChannels);
    }
    else {  // Keep
        outputChannels = inChannels.intersection(selectedChannels);
    }
    // Because we are working in deep -> deep channels should not be removed
    outputChannels += Mask_Deep;
    outputChannels += Chan_Z;
    return outputChannels;
}

bool DeepRemove::doDeepEngine(DD::Image::Box box, const ChannelSet& channels, DeepOutputPlane& plane) {
    if (!input0())
        return true;

    DeepPlane inPlane;

    if (!input0()->deepEngine(box, channels, inPlane))
        return false;

    DeepInPlaceOutputPlane outPlane(channels, box);
    outPlane.reserveSamples(inPlane.getTotalSampleCount());

    for (DD::Image::Box::iterator it = box.begin(), itEnd = box.end(); it != itEnd; ++it) {
        DeepPixel inPixel = inPlane.getPixel(it);
        size_t inPixelSamples = inPixel.getSampleCount();
        outPlane.setSampleCount(it, inPixelSamples);
        ChannelSet inPixelChannels = inPixel.channels();

        DeepOutputPixel outPixel = outPlane.getPixel(it);
        // copy all samples to DeepOutputPlane
        for (size_t inSample = 0; inSample < inPixelSamples; ++inSample)
        {
            foreach(channel, channels){
                float& outData = outPixel.getWritableUnorderedSample(inSample, channel);
                if (inPixelChannels.contains(channel)) {
                    const float& inData = inPixel.getUnorderedSample(inSample, channel);
                    outData = inData;
                }
                else {
                    outData = 0.0f; // If requested channel don't exist in input -> 0.0
                }
            }
        }
    }

    outPlane.reviseSamples();
    mFnAssert(outPlane.isComplete());
    plane = outPlane;

    return true;
}


static Op* build(Node* node) { return new DeepRemove(node); }
const Op::Description DeepRemove::d(::CLASS, "DeepRemove", build);
