/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.

  Library to generate a simple LFO to generate a Sine or triangle wave
*/
#ifndef RoxLFO_h
#define RoxLFO_h

const uint8_t RoxSineWaveForm[256] = {128,131,134,137,140,143,146,149,153,156,159,162,165,168,171,174,177,180,182,185,188,191,194,196,199,201,204,207,209,211,214,216,218,220,223,225,227,229,231,232,234,236,238,239,241,242,243,245,246,247,248,249,250,251,252,253,253,254,254,255,255,255,255,255,255,255,255,255,255,254,254,253,253,252,251,251,250,249,248,247,245,244,243,241,240,238,237,235,233,232,230,228,226,224,222,219,217,215,213,210,208,205,203,200,198,195,192,189,187,184,181,178,175,172,169,166,163,160,157,154,151,148,145,142,139,135,132,129,126,123,120,116,113,110,107,104,101,98,95,92,89,86,83,80,77,74,71,68,66,63,60,57,55,52,50,47,45,42,40,38,36,33,31,29,27,25,23,22,20,18,17,15,14,12,11,10,8,7,6,5,4,4,3,2,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,2,3,4,5,6,7,8,9,10,12,13,14,16,17,19,21,23,24,26,28,30,32,35,37,39,41,44,46,48,51,54,56,59,61,64,67,70,73,75,78,81,84,87,90,93,96,99,102,106,109,112,115,118,121,124,127};
const uint8_t RoxTriangleWaveForm[256] = {128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,255,253,251,249,247,245,243,241,239,237,235,233,231,229,227,225,223,221,219,217,215,213,211,209,207,205,203,201,199,197,195,193,191,189,187,185,183,181,179,177,175,173,171,169,167,165,163,161,159,157,155,153,151,149,147,145,143,141,139,137,135,133,131,129,127,125,123,121,119,117,115,113,111,109,107,105,103,101,99,97,95,93,91,89,87,85,83,81,79,77,75,73,71,69,67,65,63,61,59,57,55,53,51,49,47,45,43,41,39,37,35,33,31,29,27,25,23,21,19,17,15,13,11,9,7,5,3,1,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126};
class RoxLFO {
  private:
    unsigned long prevTime = 0;
    unsigned long speed = 0;
    uint8_t value = 0;
    bool reversed = false;
    bool isTriangle = false;
    void (*callback)(uint8_t value);
    void hzToMicros(float t_value){
      float t = 1.000 / t_value;
      speed = ((t*1000)*1000)/512;
    }
  public:
    RoxLFO(){
      stop();
    }
    void begin(void (*fptr)(uint8_t value)){
      callback = fptr;
    }
    void update(){
      if(active() && (unsigned long)(micros() - prevTime) >= speed){
        prevTime = micros();
        if(isTriangle){
          callback(RoxTriangleWaveForm[value++]);
        } else {
          callback(RoxSineWaveForm[value++]);
        }
      }
    }
    void setTriangle(){
      isTriangle = true;
      reset();
    }
    void setSine(){
      isTriangle = false;
      reset();
    }
    bool active(){
      return prevTime>0;
    }
    void startMicros(unsigned long t_speed){
      if(t_speed==0){
        stop();
      } else if(t_speed>=48 && t_speed<=3906){
        speed = t_speed;
        prevTime = micros();
      }
    }
    void start(float t_speed){
      if(t_speed==0){
        stop();
      } else if(t_speed>=0.1 && t_speed<=40.0){
        hzToMicros(t_speed);
        prevTime = micros();
      }
    }
    void stop(){
      prevTime = 0;
    }
    void reset(){
      reversed = false;
      value = 0;
    }
    unsigned long getMicros(){
      return speed;
    }
};
#endif
