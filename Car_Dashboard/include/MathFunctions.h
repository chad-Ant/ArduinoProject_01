#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS 1

#include "../config/ExternalLibConfig.h"
#include "../config/DataDictionary.h"

inline void normalize(float &num, float lowerBound, float upperBound){
    num = 
}

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

int reciprocal(float num){
    int exponent = 0;
    float mantissa = frexp(num, &exponent);
    //normalize to [1,2)
    mantissa *= 2.0;
    exponent -= 1;
    
    float guess = num * ;
    for (int i = 0; i < 3; i++){ //allow 3 iterations
        guess *= 2 - num * guess;
    }
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