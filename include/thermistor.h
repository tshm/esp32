/** 温度センサー: KY-013 analog temperature sensor module */
class Thermistor
{
public:
  static float readTemperature(int pin)
  {
    int vo = analogRead(pin);
    /** Resistance of Thermistor */
    float Rth = R1 * (1023.0 / (float)vo - 1.0);
    float logRth = log(Rth);
    /** temperature in Kelvin */
    float K = (1.0 / (c1 + c2 * logRth + c3 * logRth * logRth * logRth));
    return K - 273.15;
  }

private:
  constexpr static float R1 = 10e3;
  /** steinhart-hart coeficients for thermistor */
  constexpr static float c1 = 0.001129148;
  constexpr static float c2 = 0.000234125;
  constexpr static float c3 = 0.0000000876741;
};
