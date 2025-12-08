#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    if (cache.get(track.get_title()) != nullptr) {//HIT, adds 1 to the accsess count and return 1
        return 1;
    }
    //MISS - need to create a clone.
    PointerWrapper<AudioTrack> cloned = track.clone();
    //IF 2 : cloend is a nullptr, returns -2 because needs to return some int.
    if (cloned.get() == nullptr) {
        std::cerr << "[ERROR] Track: " << track.get_title() << " failed to clone" << std::endl; 
        //Handle clone failures gracefully by logging [ERROR] Track: "<title>" failed
        //to clone and returning appropriate failure code without corrupting cache state
        return -2;
    }
    //ELSE
    cloned->load();
    cloned->analyze_beatgrid();
    //Cloned is already wrapped there is no need to wrap it again.
    if (cache.put(std::move(cloned))){ //Why MOVE? because put passes by Value which tries to copy the PointerWrapper<AudioTrack>, 
                                        //that is a problem since there are no copy constructor only move ones.
        return -1;
    }
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title); //If this function needs to do accsess_count++ than this needs to change to a simple cahce.get()
}
