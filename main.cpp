#include <iostream>
#include <cmath>
using namespace std;
unsigned short converting(unsigned short *status, int size);
unsigned short detecting_the_sygment(string *coded_amp, signed short amp);
void standart_amps(unsigned short num_of_the_sygment, unsigned short *amp_standart_main, unsigned short *amp_standart_add, float *amp_correction, unsigned short *step_quant);
unsigned short detectin_quant_lvl(unsigned short *amp_standart_main, unsigned short *amp_standart_add, float *amp_correction, unsigned short *step_quant, string *coded_amp, signed short amp);

int main()
{
    system("cls");
    signed short amp; //amlitude of the signal
    cout << "Enter the signal amplitude: ";
    cin >> amp;
    system("cls");
    cout << "Amlitude: " << amp << endl;
    string *coded_amp = new string; //coded amplitude

    /*Step №1
     * Determin and coding polarity
     * of the input sygnal
     */
    *coded_amp = amp < 0? "0" : "1";

    /*
     * We'll make further calculations with
     * the amplitude took by module for comfort
     * Для удобаства дальнейшие расчёты будем
     * проводить с амплитудой взятой по модулю
     */
    amp = abs(amp);

    /*Step №2
     * determining and encoding the sygment number
     * within which amplitude of encoded sample
     * placed
     */
    unsigned short num_of_the_sygment
            = detecting_the_sygment(coded_amp, amp);
    cout << "\nSygment number: " << num_of_the_sygment << endl;
    /* Step №3
     * determining and encoding the quantization
     * level number in the found sygment
     */

    // Getting standart sygnals;
    unsigned short *amp_standart_main = new unsigned short; // Main standart sygnal
    unsigned short *amp_standart_add = new unsigned short[4]; // Additional standart sygnals
    float *amp_correction = new float;
    unsigned short *step_quant = new unsigned short;
    // determining standart sygnals
    standart_amps(num_of_the_sygment, amp_standart_main, amp_standart_add, amp_correction, step_quant);
    // Calculating the quantization level in the sygment
    unsigned short quantization_level =
            detectin_quant_lvl(amp_standart_main, amp_standart_add, amp_correction, step_quant, coded_amp, amp);
    cout << "Quantization level: " << quantization_level << endl;
    cout << "Answer: " << *coded_amp;
    delete coded_amp;
    cin.get();
    cin.get();
    return main();
}

unsigned short detecting_the_sygment(
        string *coded_amp,
        signed short amp
        ){
    /*
     * This fucntion determine the sygment number
     */
    unsigned short
            amp_standart = 128, //start standart amplitude
            status[3];
    cout << "\nStandart signals (main): ";
    for(int i = 0; i < 3; i++)
    {
        cout << amp_standart << " ";
        if(amp >= amp_standart) {
            amp_standart = (i == 0)?
                        amp_standart *= 4 :
                    amp_standart *= 2;
            *coded_amp += "1";
            status[i] = 1;
        }
        else if (amp < amp_standart){
            amp_standart = (i == 0)?
                        amp_standart /= 4 :
                    amp_standart /= 2;
            *coded_amp += "0";
            status[i] = 0;
        }
    }
    return converting(status,3);
}

unsigned short detectin_quant_lvl(
        unsigned short *amp_standart_main,
        unsigned short *amp_standart_add,
        float *amp_correction,
        unsigned short *step_quant,
        string *coded_amp,
        signed short amp
        ){
    /*
     * This function calculate the
     * quantization level in the sygment
     */
    unsigned short status[4];
    for(int i = 0; i < 4; i++)
    {
        if((*amp_standart_main + amp_standart_add[i]) < amp){
            cout << *amp_standart_main << " + " << amp_standart_add[i]
                    << " = " << *amp_standart_main + amp_standart_add[i]
                       << " < " << amp << "\ta" << i+4 << " = 1" << endl ;
            *amp_standart_main += amp_standart_add[i];
            *coded_amp += "1";
            status[i] = 1;

        } else {
            cout << *amp_standart_main << " + " << amp_standart_add[i]
                    << " = " << *amp_standart_main + amp_standart_add[i]
                       << " >= " << amp << "\ta" << i+4 << " = 0" << endl ;
            *coded_amp += "0";
            status[i] = 0;

        }
    }
    cout << "Output signal: " << *amp_standart_main << endl;
    cout << "Quantization mistake: " << amp - *amp_standart_main << endl;
    delete amp_standart_main;
    delete[] amp_standart_add;
    delete amp_correction;
    delete step_quant;
    return converting(status,4);
}

unsigned short converting(
        unsigned short *status,
        int size
        ){
    /*
     * This function makes conversation from
     * binary system to decimal
     */
    int num_of_the_sygment{0};
    for(int i = 0; i < size; i++){
        num_of_the_sygment +=
                status[i]*pow(2,size-1-i);
    }
    return num_of_the_sygment;
}

void standart_amps(
        unsigned short num_of_the_sygment,
        unsigned short *amp_standart_main,
        unsigned short *amp_standart_add,
        float *amp_correction,
        unsigned short *step_quant
        ){
    /*
     * This function makes calculation
     * of the standart sygnals
     */
    *amp_standart_main = 16;
    for (int i = 0; i < num_of_the_sygment - 1; i++)
        *amp_standart_main=(*amp_standart_main)*2;
    amp_standart_add[0] = {
        static_cast<unsigned short>(*amp_standart_main/2)
    };
    if(num_of_the_sygment == 0)
    *amp_standart_main = 0;
    cout << "\nStandart signals (additional): " << *amp_standart_main << " "
         <<amp_standart_add[0] << " ";
    for(int i = 1; i < 4; i++)
    {
        amp_standart_add[i] = amp_standart_add[i-1]/2;
        cout <<amp_standart_add[i] << " ";
    }

    *amp_correction = amp_standart_add[3]/2;
    cout << "\nCorrection signal: " << *amp_correction << endl;
    *step_quant = *amp_correction*2;
    cout << "Qantization step: " << *step_quant << endl;
}
