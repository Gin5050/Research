double Pathloss(double distance , double n);
complex<double> rayleigh();

//Pathloss ゴールドスミス参照
double Pathloss(double distance , double n){
  double pathloss;
  double k;

  //k = 20 * log10(waveLength /(4 * M_PI));
  pathloss = POWER_dBm + LOG_WAVE - 10 * n *log10(distance); //dB
  //pathloss = LOG_WAVE - 10 * n *log10(distance); //dB

  return pathloss;W
}

complex<double> rayleigh(){
  int i, j;

  double real1;
  double imag1;

  double nvar;
  double x,y;
  double w,z;

  nvar = 1.0 / 2.0;
  x = 0.0;

  while(x == 0.0) x = randuni(mt);
  y = randuni(mt);
  real1 = sqrt(-2.0 * nvar * log(x)) * sin(2.0 * M_PI * y);
  
  w = 0.0;
  while(w == 0.0) w = randuni(mt);
  z = randuni(mt);
  imag1 = sqrt(-2.0 * nvar * log(w)) * cos(2.0 * M_PI * z);

  complex<double> fading(real1, imag1);

  //temp_dis.push_back(abs(fading));
  //temp_dis.push_back(10 * log10(abs(fading) * abs(fading)));
  return fading;
}
