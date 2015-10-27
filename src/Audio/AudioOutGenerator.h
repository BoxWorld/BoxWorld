//
//  AudioOutGenerator.h
//  BoxWorld
//
//  Created by Han Maokun on 10/27/15.
//
//

#ifndef BoxWorld_AudioOutGenerator_h
#define BoxWorld_AudioOutGenerator_h

#include <fstream>
#include <iostream>

#define MAX_AUDIO_VOLUME 2147483648

static const float DEFAULT_DURATION = 60;
static const float DEFAULT_SAMPLE_RATE = 44100;

template <typename Word>
std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
{
    for (; size; --size, value >>= 8)
        outs.put( static_cast <char> (value & 0xFF) );
    
    return outs;
}

class AudioOutGenerator {
public:
    AudioOutGenerator(int tex_width, int tex_height, float duration = DEFAULT_DURATION) {
        mWavBuf = NULL;
        mDuration = duration;
        mTotalSamples = duration * DEFAULT_SAMPLE_RATE;
        mTexWidth = tex_width;
        mTexHeight = tex_height;
        mTexSamples = mTexWidth * mTexHeight;
        mNumBlocks = mTotalSamples/mTexSamples;
        initWavBuf();
    }
    
    virtual ~AudioOutGenerator() {
        if(mWavBuf[0]) delete mWavBuf;
        //if(mWavBuf[1]) delete mWavBuf;
    }
    
    int getNumBlocks() { return mNumBlocks; }
    
    int getTexSamples() { return mTexSamples; }
    
    float getSampleRate() { return DEFAULT_SAMPLE_RATE; }
    
    void writeToBuf(int offset, unsigned char* data){
        float *leftBuf  = mWavBuf[0];
        float *rightBuf = mWavBuf[1];
        for(int i=0; i<mTexSamples; i++) {
            leftBuf[offset + i]  = -1.0f + 2.0f * ((data[4*i + 0]) + 256.0f * (data[4*i + 1])) / 65535.0f;
            rightBuf[offset + i] = -1.0f + 2.0f * ((data[4*i + 2]) + 256.0f * (data[4*i + 3])) / 65535.0f;
        }
    }
    
    void writeWavFile() {
        ofstream f( "example.wav", ios::binary );
        // Write the file headers
        f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
        write_word( f,     16, 4 );  // no extension data
        write_word( f,      1, 2 );  // PCM - integer samples
        write_word( f,      2, 2 );  // two channels (stereo file)
        write_word( f,  44100, 4 );  // samples per second (Hz)
        write_word( f, 352800, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8
        write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
        write_word( f,     32, 2 );  // number of bits per sample (use a multiple of 8)
        
        // Write the data chunk header
        size_t data_chunk_pos = f.tellp();
        f << "data----";  // (chunk size to be filled in later)
        
        // Write the audio samples
        for (int n = 0; n < mTotalSamples; n++)
        {
            write_word( f, (long)(MAX_AUDIO_VOLUME * (*(mWavBuf[0]+n))), 4 );
            write_word( f, (long)(MAX_AUDIO_VOLUME * (*(mWavBuf[1]+n))), 4 );
        }
        
        // (We'll need the final file size to fix the chunk sizes above)
        size_t file_length = f.tellp();
        
        // Fix the data chunk header to contain the data size
        f.seekp( data_chunk_pos + 4 );
        write_word( f, file_length - data_chunk_pos + 8 );
        
        // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
        f.seekp( 0 + 4 );
        write_word( f, file_length - 8, 4 );
    }
    
private:
    void initWavBuf() {
        mWavBuf = new float*[2];

        for(int i = 0; i < 2; ++i)
            mWavBuf[i] = new float[mTotalSamples];
    }
    
    int mTotalSamples;
    int mTexSamples;
    int mNumBlocks;
    int mTexWidth, mTexHeight;
    float mDuration;
    float** mWavBuf;
};

#endif
