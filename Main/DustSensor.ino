#define COV_RATIO 0.2 //ug/mmm / mv
#define NO_DUST_VOLTAGE 400 //mv
#define SYS_VOLTAGE 3300 //mv

void initDust() {
  digitalWrite(iled, LOW); //iled default closed
}

int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if (flag_first == 0) {
    flag_first = 1;
    for(i = 0, sum = 0; i < _buff_max; i++) {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  } else {
    sum -= _buff[0];
    for (i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}

void warmUpDust() {
  uint32_t time = getTime() + 10;
  while (time > getTime()) {
    getDust();
    delay(50);
  }
}

float getDust() {
  //variables
  float density, voltage;
  int adcvalue;

  //get adcvalue
  digitalWrite(iled, HIGH);
  delayMicroseconds(280);
  adcvalue = analogRead(vout);
  digitalWrite(iled, LOW);
  adcvalue = Filter(adcvalue);

  //covert voltage (mv)
  voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;

  //voltage to density
  if (voltage >= NO_DUST_VOLTAGE) {
    voltage -= NO_DUST_VOLTAGE;
    
    density = voltage * COV_RATIO;
  } else {
    density = 0;
  }
  
  return density;
}