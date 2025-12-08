#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 * NOTE: Done
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    decks[0] = decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 * NOTE: DONE
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks...";
    for (int i = 0; i < 2; i++) { //No need do delete every little part in decks[i] because the AudioTrack distructor is called.
        if (decks[i] != nullptr)
            delete decks[i];
        decks[i] = nullptr;
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> clone = track.clone();
    if (!clone) {//Checks if clone is nullptr and if so enters.
        std::cout << "[ERROR] Track: '" << track.get_title() << "' failed to clone" << std::endl;
        return -1;
    }
    int target = 1 - active_deck;
    if(decks[active_deck] == nullptr){
        target = 0;
    } 
    std::cout << "[Deck Switch] Target deck: " << target << std::endl;
    if (decks[target] != nullptr) {
        delete decks[target];
        decks[target] = nullptr;
    }
    //checks for need to sync_bpm
    clone->load();
    clone->analyze_beatgrid();
    if ((auto_sync) && decks[active_deck] != nullptr) {
        if (!(can_mix_tracks(clone))){
            sync_bpm(clone);
        }
    } else if(auto_sync && decks[active_deck] == nullptr){
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty." << std::endl;
    }
    AudioTrack* clone_ptr = clone.release();
    std::cout << "[Load Complete] '" << clone_ptr->get_title() << "' is now loaded on deck " << target << std::endl;
    // if (decks[active_deck] != nullptr) {
    //     std::cout << "[Unload] Unloading previous deck " << active_deck << " " << decks[active_deck]->get_title() << std::endl;
    //     delete decks[active_deck];
    //     decks[active_deck] = nullptr;
    // }
    decks[target] = clone_ptr;
    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;
    return target;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * NOTE: DONE
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (decks[active_deck] == nullptr) {
        return false;
    }
    if (!(track.operator bool())) {
        return false;
    }
    int active_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int diff = std::abs(active_bpm - new_bpm);
    return (diff <= bpm_tolerance);
}

/**
 * TODO: Implement sync_bpm method
 * NOTE: DONE
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const { //Const fun but changes bpm??
    if ((decks[active_deck] != nullptr) && (track.operator bool())) {
        int origin_BPM = track->get_bpm();
        int track_BPM = decks[active_deck]->get_bpm();
        int avg = (origin_BPM + track_BPM) / 2;
        track->set_bpm(avg);
        std::cout << "[Sync BPM] Syncing BPM from " << origin_BPM << " to " << avg << std::endl;
    }
}
