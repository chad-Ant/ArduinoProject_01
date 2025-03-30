#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

inline void transpose2x2(float &array[2][2]){
    swap(array[0][1],array[1][0]);
}

inline void inverse2x2(float &array[2][2]){
    float det = array[0][0] * array[1][1] - array[0][1] * array[1][0];
    if (det == 0){
        array = {{NAN,NAN},{NAN,NAN}};
        return;
    }
    float detReciprocal = 1/det;
    array = {{array[1][1] * detReciprocal, -array[0][1] * detReciprocal},{-array[1][0] * detReciprocal, array[0][0] * detReciprocal}};
}

inline void reciprocal(float &num, int maxIteration, float tolerance){
    float guess = num * 
    do{

    } while ();
}

enum FilterWSize{
    SIZE_8 = 8,
    SIZE_16 = 16,
    SIZE_32 = 32
};

class SimpleMovingAverage{
    private:
        FilterWSize windowSize;
        float* outputBuffer;
        uint8_t bufferIndex;
        float reciprocalDivisor;
        float sum;
        bool outputValid;

    public:
        SimpleMovingAverage(const FilterWSize wSize);
        ~SimpleMovingAverage();

        SimpleMovingAverage(const SimpleMovingAverage&) = delete;
        SimpleMovingAverage& operator = (const SimpleMovingAverage&) = delete;
        bool calculate(float &input,float &output);
        void reset();
};

/*
class KalmanFilter_2D{
    private:
        float x[2]; //measurement fusion estimate
        float P[2][2]; //noise cov estimate
        float Q[2][2]; //process noise cov
        float F[2][2]; //state transition coeff
        float dt; // time step
    public:
        float R1;
        float R2;
        KalmanFilter_2Sources(float x[2],float P,float Q,float F,float H);
        ~KalmanFilter_2Sources();

        KalmanFilter_2Sources(const KalmanFilter_2Sources&) = delete;
        KalmanFilter_2Sources& operator = (const KalmanFilter_2Sources&) = delete;
        bool predict();
        bool adjust(float &output);
}
*/
#endif