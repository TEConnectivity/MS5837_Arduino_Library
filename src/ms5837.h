#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MS5837_COEFFICIENT_COUNT 7

#define MS5837_CONVERSION_TIME_OSR_256 1
#define MS5837_CONVERSION_TIME_OSR_512 2
#define MS5837_CONVERSION_TIME_OSR_1024 3
#define MS5837_CONVERSION_TIME_OSR_2048 5
#define MS5837_CONVERSION_TIME_OSR_4096 9
#define MS5837_CONVERSION_TIME_OSR_8192 17

// Enum
enum ms5837_resolution_osr {
  ms5837_resolution_osr_256 = 0,
  ms5837_resolution_osr_512,
  ms5837_resolution_osr_1024,
  ms5837_resolution_osr_2048,
  ms5837_resolution_osr_4096,
  ms5837_resolution_osr_8192
};

enum ms5837_status {
  ms5837_status_ok,
  ms5837_status_no_i2c_acknowledge,
  ms5837_status_i2c_transfer_error,
  ms5837_status_crc_error
};

enum ms5837_status_code {
  ms5837_STATUS_OK = 0,
  ms5837_STATUS_ERR_OVERFLOW = 1,
  ms5837_STATUS_ERR_TIMEOUT = 4
};

// Functions
class ms5837 {

public:
  ms5837();

  /**
   * \brief Perform initial configuration. Has to be called once.
   */
  void begin();

  /**
  * \brief Check whether MS5837 device is connected
  *
  * \return bool : status of MS5837
  *       - true : Device is present
  *       - false : Device is not acknowledging I2C address
  */
  boolean is_connected(void);

  /**
  * \brief Reset the MS5837 device
  *
  * \return ms5837_status : status of MS5837
  *       - ms5837_status_ok : I2C transfer completed successfully
  *       - ms5837_status_i2c_transfer_error : Problem with i2c transfer
  *       - ms5837_status_no_i2c_acknowledge : I2C did not acknowledge
  */
  enum ms5837_status reset(void);

  /**
  * \brief Set  ADC resolution.
  *
  * \param[in] ms5837_resolution_osr : Resolution requested
  *
  */
  void set_resolution(enum ms5837_resolution_osr res);

  /**
  * \brief Reads the temperature and pressure ADC value and compute the
  * compensated values.
  *
  * \param[out] float* : Celsius Degree temperature value
  * \param[out] float* : mbar pressure value
  *
  * \return ms5837_status : status of MS5837
  *       - ms5837_status_ok : I2C transfer completed successfully
  *       - ms5837_status_i2c_transfer_error : Problem with i2c transfer
  *       - ms5837_status_no_i2c_acknowledge : I2C did not acknowledge
  *       - ms5837_status_crc_error : CRC check error on on the PROM
  * coefficients
  */
  enum ms5837_status read_temperature_and_pressure(float *temperature,
                                                   float *pressure);

private:
  enum ms5837_status write_command(uint8_t cmd);
  enum ms5837_status read_eeprom_coeff(uint8_t command, uint16_t *coeff);
  boolean crc_check(uint16_t *n_prom, uint8_t crc);
  enum ms5837_status conversion_and_read_adc(uint8_t cmd, uint32_t *adc);
  enum ms5837_status read_eeprom(void);

  uint16_t eeprom_coeff[MS5837_COEFFICIENT_COUNT + 1];
  bool coeff_read = false;
  enum ms5837_status ms5837_write_command(uint8_t);
  enum ms5837_status ms5837_read_eeprom_coeff(uint8_t, uint16_t *);
  enum ms5837_status ms5837_read_eeprom(void);
  enum ms5837_status ms5837_conversion_and_read_adc(uint8_t, uint32_t *);

  enum ms5837_resolution_osr ms5837_resolution_osr;
  uint32_t conversion_time[6] = {
      MS5837_CONVERSION_TIME_OSR_256,  MS5837_CONVERSION_TIME_OSR_512,
      MS5837_CONVERSION_TIME_OSR_1024, MS5837_CONVERSION_TIME_OSR_2048,
      MS5837_CONVERSION_TIME_OSR_4096, MS5837_CONVERSION_TIME_OSR_8192};
};
