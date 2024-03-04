#pragma once

#include <System.hpp>

class SFOutput {
   public:
    enum class Logic {
        NORMAL = 0,
        REVERSE = 1
    };

    typedef struct {
        bool state;
        uint16_t time;
    } SequenceStep;

    typedef struct {
        const SequenceStep* section;
        uint8_t length;
    } Sequence;

    enum class Procedure {
        MANUAL,
        BLINKING,
        NUMBER_BLINK,
        Sequence
    };
    enum class Cycle {
        OFF = 0,
        ON = 1
    };

    SFOutput() = default;
    SFOutput(GPIO_TypeDef * port, uint32_t pin, Logic logic = Logic::NORMAL);

    uint16_t getPin(){
        return pin;
    }

    void init();
    void iterate();

    bool read();

    bool get();
    void set(bool state);
    void ON();
    void OFF();
    void toggle();

    void blink(uint32_t time, Cycle cycl = Cycle::ON);
    void blink(uint32_t on_time, uint32_t off_time, Cycle cycl = Cycle::ON);

    void blinkNumber(int number, uint32_t time, Cycle cycl = Cycle::ON);
    void blinkNumber(int number, uint32_t on_time, uint32_t off_time, Cycle cycl = Cycle::ON);

    void timeON(uint32_t timeon);
    void timeOFF(uint32_t timeoff);
    void timeSET(uint32_t time, bool startvalue);
    void timeTOG(uint32_t time);

    void playSequence(Sequence* sequnce, uint16_t number = 1);
    void stopSequence();

    Procedure getMode();

   private:
    bool pinStateRead();
    void pinStateWrite(uint8_t state);
    uint32_t getTick();

    Procedure procedure;
    Cycle cycle;
    uint32_t on_time;
    uint32_t off_time;
    uint32_t lastiterate;

    int numberblink;

    Sequence* sequence;
    int16_t sequence_position;
    uint16_t sequence_number;
    uint16_t sequence_time_to_go;
    uint32_t sequence_last_iterate;

    GPIO_TypeDef * port;
    uint32_t pin;

    Logic logic;

    bool state;
};
