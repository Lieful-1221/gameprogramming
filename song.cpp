#include <stdio.h>
#include <math.h>
#include <windows.h>

int calc_frequency(int octave, int inx) {
    double do_scale = 32.7032;
    double ratio = pow(2., 1./12.);
    double temp;
    int i;
    
    temp = do_scale * pow(2, octave - 1);
    for (i = 0; i < inx; i++) {
        temp *= ratio;
    }
    return (int)(temp + 0.5);
}

int main(void) {
    int melody[] = {
        7, 4, 4, 5, 2, 2, 0, 2, 4, 5, 7, 7, 7, 
        4, 4, 4, 7, 7, 7, 7, 4, 2, 4, 0,
        2, 4, 5, 7, 7, 7, 2, 2, 2, 2, 4, 5, 4, 2, 4, 0,
        7, 7, 7, 4, 4, 4, 5, 4, 2, 4, 0, 7, 7, 7
    };
    
    int beats[] = {
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000, 
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000, 
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000,
        500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 1000
    };
    
    int i;
    int melody_length = sizeof(melody) / sizeof(melody[0]);
    int freq;

    for (i = 0; i < melody_length; i++) {
        freq = calc_frequency(4, melody[i]);
        Beep(freq, beats[i]);
    }
    
    return 0;
}