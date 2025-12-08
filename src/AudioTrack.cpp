#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
            int duration, int bpm, size_t waveform_samples): 
            title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
            waveform_data(nullptr), waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {
    // TODO: Implement the destructor 
    if (waveform_data != nullptr)
        delete[] waveform_data;
    waveform_data = nullptr;
    // Your code here...
}

AudioTrack::AudioTrack(const AudioTrack& other) :
    // TODO: Implement the copy constructor
      
    //Member INIT List:
    title(other.get_title()), 
    artists(other.get_artists()),//Vectors already have their deep copy mech so no need to loop around the copy
    duration_seconds(other.get_duration()),
    bpm(other.get_bpm()),
    waveform_data(nullptr), waveform_size(other.waveform_size) {
    if (other.waveform_data != nullptr) {
        this->waveform_data = new double [waveform_size];
        for (size_t i = 0; i < waveform_size; ++i) {
            this->waveform_data[i] = other.waveform_data[i];
        }
    }
    //Else - wavefrom_data is allready nullptr so there is no nned for an else, only an "if" to make sure we are "safe".
}

AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    // TODO: Implement the copy assignment operator
    if (this == &other){ //First checks if a copy operator is even needed.
        return *this;
    }
    delete[] waveform_data;
    waveform_data = nullptr;
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;
    
    if (other.waveform_data != nullptr) {
        waveform_data = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; i++) {
            waveform_data[i] = other.waveform_data[i];
        }
    }
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    return *this;
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept :
// TODO: Implement the move constructor
title(std::move(other.title)), artists(std::move(other.artists)), duration_seconds(other.duration_seconds), bpm(other.bpm), waveform_data(other.waveform_data), waveform_size(other.waveform_size)
{
    // Your code here...
    other.waveform_data = nullptr;
    other.waveform_size = 0;
    std::cout << "AudioTrack move constructor called for: " << title << std::endl;
    
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    // TODO: Implement the move assignment operator
    // Your code here...
    if (&other == this) {
        return *this;
    }
    delete[] waveform_data;
    title = std::move(other.title);
    artists = std::move(other.artists);
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;
    waveform_data = other.waveform_data;
    other.waveform_size = 0;
    other.waveform_data = nullptr;
    std::cout << "AudioTrack move assignment called for: " << title << std::endl;
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}