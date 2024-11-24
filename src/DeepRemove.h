#ifndef DDImage_DeepRemove_h
#define DDImage_DeepRemove_h

#include "DDImage/DeepFilterOp.h"
#include "DDImage/Knobs.h"

#include "DDImage/ChannelSet.h"
#include "DDImage/Channel.h"

#include <vector>

using namespace DD::Image;

static const char* CLASS = "DeepRemove";
static const std::tuple<int, int, int, int> VERSION(1, 0, 0, 0);
static const int MAXCUSTOMKNOBS = 10;

static const char* const operationKnobNames[] = { "remove", "keep", 0 };

class DeepRemove : public DeepFilterOp {
	protected:
		int _operationKnob;  ///< operation to apply: remove or keep.
		int _customChannelsCountKnob;  ///< The number of channels created by the user.
		ChannelSet _mainChannelsSetKnob;  ///< Main channel knob
		std::vector<ChannelSet> _dynamicChannelsKnob;  ///< List of custom channels
		int _numDynamicKnobs;  ///< Tracks the warning Knobs to know if is currently created
		bool _showWarning;  ///< True to create a dynamic text knob with a warning message, False otherwise

		/**
		 * @brief validates the deep input and builds the output channels.
		 */
		void _validate(bool for_real) override;

		/**
		 * @brief Updates the visibility of extra user channels knobs.
		 *
		 * Updates the visibility status of the channelSet knobs based on the
		 * customChannelsCountKnob value.
		 */
		void _refreshDynamicChannels();
	public:
		// Public class members
		static const Description d;

		// Publich class methods
		DeepRemove(Node* node);

		const char* node_help() const override;

		/**
		 * @brief overrides the parent function to return the link to github.
		 *
		 * @return Node's github page url.
		 */
		std::string docsPath() const override;

		// If it's desired to display the node version in the properties panel -> uncomment
		//std::string getLibraryName() const override { return std::string(CLASS); }
		//VersionInfo getLibraryVersion() const override { return VERSION; }

		const char* Class() const override { return CLASS; }

		Op* op() override { return this; }

		/**
		 * @brief Builds and fills the knobs of the node.
		 */
		void knobs(Knob_Callback f) override;

		/**
		 * @brief Updates the status of custom channels knobs.
		 *
		 * Modifies the number of channels knobs when the user press the add
		 * or remove button and refresh the visibility of them.
		 * If the user tries to generate more than MAXCUSTOMKNOBS value, shows
		 * the warning knob.
		 */
		int knob_changed(Knob*) override;

		/**
		 * @brief Returns the number of custom channels currently active.
		 *
		 * Because we have buttons knobs with knob change always, we can not trust
		 * in the value of the class member. We need to access to the value of the knob
		 * directly. This function is used on refreshing the visibility of the knobs.
		 */
		int getCustomChannelsCount() const { return knob("customChannelsCount")->get_value(); }

		/**
		 * @brief Creates a text knob that shows a warning.
		 * 
		 * Creates a dynamic knob after the last custom channel knob to show a warning
		 * letting the user know that the maximum number of channels knobs is MAXCUSTOMKNOBS.
		 */
		static void addWarningKnob(void* p, Knob_Callback f);

		/**
		 * @brief Returns the channels selected by the user.
		 *
		 * Convines all the channels knobs currently active to return an unique
		 * ChannelSet value with all the channels added to it.
		 *
		 * @return Channels selected by the user in the knobs.
		 */
		ChannelSet getSelectedChannels();

		/**
		 * @brief Returns the final list of channels that the node is having like output.
		 *
		 * Checks the current operation mode of the node and builds the right
		 * ChannelSet based on the inputs channels and the selected channels by the user.
		 *
		 * @param inChannels The list of channels that the input 0 is providing.
		 *
		 * @return The current output ChannelSet of the node.
		 */
		ChannelSet getOutputChannels(ChannelSet inChannels);

		bool doDeepEngine(DD::Image::Box box, const ChannelSet& channels, DeepOutputPlane& plane) override;
};

#endif
