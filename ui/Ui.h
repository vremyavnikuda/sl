//
// Created by vremyavnikuda on 9/26/24.
//

#ifndef UI_H
#define UI_H
#include <string>
#include <vector>


class Ui {
public:
    void display_audio_spectrum(int spectrum_height, int col);

    static void display_interface(const std::vector<std::string> &playlist, int &track_index);
    //TODO: пока что не знаю что будет делать данный метод
    //но чет делать он будет =)))
    //void interface_display();
};


#endif //UI_H
