#include "NXP_I2C.hpp"

const uint8_t TCA95x5_DEFAULT_ADDRESS = 0x20;

typedef union {
    uint16_t raw = 0;
    struct {
        uint8_t port_0;
        uint8_t port_1;
    };

    struct {
        bool i00 : 1;
        bool i01 : 1;
        bool i02 : 1;
        bool i03 : 1;
        bool i04 : 1;
        bool i05 : 1;
        bool i06 : 1;
        bool i07 : 1;
        bool i10 : 1;
        bool i11 : 1;
        bool i12 : 1;
        bool i13 : 1;
        bool i14 : 1;
        bool i15 : 1;
        bool i16 : 1;
        bool i17 : 1;
    };
} tca95x5_input_status_t;

typedef union {
    uint16_t raw = 0xFFFF;
    struct {
        uint8_t port_0;
        uint8_t port_1;
    };
    struct {
        bool o00 : 1;
        bool o01 : 1;
        bool o02 : 1;
        bool o03 : 1;
        bool o04 : 1;
        bool o05 : 1;
        bool o06 : 1;
        bool o07 : 1;
        bool o10 : 1;
        bool o11 : 1;
        bool o12 : 1;
        bool o13 : 1;
        bool o14 : 1;
        bool o15 : 1;
        bool o16 : 1;
        bool o17 : 1;
    };
} tca95x5_output_config_t;

typedef union {
    uint16_t raw = 0xFFFF;
    struct {
        uint8_t port_0;
        uint8_t port_1;
    };
    struct {
        bool p00_is_input : 1;
        bool p01_is_input : 1;
        bool p02_is_input : 1;
        bool p03_is_input : 1;
        bool p04_is_input : 1;
        bool p05_is_input : 1;
        bool p06_is_input : 1;
        bool p07_is_input : 1;
        bool p10_is_input : 1;
        bool p11_is_input : 1;
        bool p12_is_input : 1;
        bool p13_is_input : 1;
        bool p14_is_input : 1;
        bool p15_is_input : 1;
        bool p16_is_input : 1;
        bool p17_is_input : 1;
    };
} tca95x5_mode_config_t;

typedef union {
    uint16_t raw = 0;
    struct {
        uint8_t port_0;
        uint8_t port_1;
    };
    struct {
        bool p00_invert_enabled : 1;
        bool p01_invert_enabled : 1;
        bool p02_invert_enabled : 1;
        bool p03_invert_enabled : 1;
        bool p04_invert_enabled : 1;
        bool p05_invert_enabled : 1;
        bool p06_invert_enabled : 1;
        bool p07_invert_enabled : 1;
        bool p10_invert_enabled : 1;
        bool p11_invert_enabled : 1;
        bool p12_invert_enabled : 1;
        bool p13_invert_enabled : 1;
        bool p14_invert_enabled : 1;
        bool p15_invert_enabled : 1;
        bool p16_invert_enabled : 1;
        bool p17_invert_enabled : 1;
    };
} tca95x5_polarity_config_t;

typedef struct {
    tca95x5_output_config_t output;
    tca95x5_mode_config_t mode;
    tca95x5_polarity_config_t polarity;
} tca95x5_config_t;

/**
 * Directionality for I/O pins
 * Pins are initially configured as inputs upon restart.
 */
typedef enum TCA95x5_PIN_MODE { TCA95x5_INPUT = 1, TCA95x5_OUTPUT = 0 } tca95x5_pin_mode_t;

/**
 * Output state of pins.
 * This configuration does not affect pins configured as inputs.
 */
typedef enum TCA95x5_PIN_STATE {
    TCA95x5_LOW = 0,
    TCA95x5_HIGH = 1,
} tca95x5_pin_output_state_t;

/**
 * Polarity of input pins.
 * Pins configured as inverted will display the oppposite state in the input
 * register.
 */
typedef enum TCA95x5_PIN_POLARITY {
    TCA95x5_NORMAL = 0,
    TCA95x5_INVERTED = 1,
} tca95x5_pin_polarity_t;

typedef enum { P00 = 0, P01 = 1, P02 = 2, P03 = 3, P04 = 4, P05 = 5, P06 = 6, P07 = 7, P10 = 8, P11 = 9, P12 = 10, P13 = 11, P14 = 12, P15 = 13, P16 = 14, P17 = 15 } tca95x5_pin_t;

///////////////////////////////////////////////////////////////////////////////

class TCA95x5 {
   public:

    TCA95x5(NXP_I2C& Wire);
    NXP_I2C& Wire;

    void begin(uint8_t address = TCA95x5_DEFAULT_ADDRESS);
    void begin(bool a0, bool a1, bool a2);

    void write(tca95x5_config_t config);
    void write(tca95x5_mode_config_t config);
    void write(tca95x5_output_config_t config);
    void write(tca95x5_polarity_config_t config);

    //
    void read(tca95x5_config_t &config);
    void read(tca95x5_mode_config_t &config);
    void read(tca95x5_output_config_t &config);
    void read(tca95x5_polarity_config_t &config);

    //
    void read(tca95x5_input_status_t &status);

    tca95x5_config_t get_config();
    tca95x5_mode_config_t get_mode_config();
    tca95x5_output_config_t get_output_config();
    tca95x5_polarity_config_t get_polarity_config();
    tca95x5_input_status_t get_input_status();

    void pin_mode(tca95x5_config_t &config, size_t pin_id, tca95x5_pin_mode_t mode);
    void pin_write(tca95x5_config_t &config, size_t pin_id, tca95x5_pin_output_state_t state);

   private:
    /**
     * Register map of the IO expander.
     */
    typedef enum TCA95x5_REGISTER {
        INPUT_0 = 0x00,         // Actual pin states for port 0
        INPUT_1 = 0x01,         // Actual pin states for port 1
        OUTPUT_0 = 0x02,        // Output states for port 0
        OUTPUT_1 = 0x03,        // Output states for port 1
        POLARITY_INV_0 = 0x04,  // Polarity for inputs on port 0
        POLARITY_INV_1 = 0x05,  // Polarity for inputs on port 1
        CONFIG_0 = 0x06,        // Pin mode/direction for port 0
        CONFIG_1 = 0x07,        // Pin mode/direction for port 1
    } tca95x5_reg_address_t;

    uint8_t _device_address;

    void set_address(bool a0, bool a1, bool a2);
    bool read(uint8_t *output, tca95x5_reg_address_t address, uint8_t length = 2);
    bool write(uint8_t *input, tca95x5_reg_address_t address, uint8_t length = 2);
};
