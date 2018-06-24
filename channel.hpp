/*Channelクラス*/
#ifndef CHANNEL_HEAD //インクルードガード
#define CHANNEL_HEAD

class Channel{
public:

  static double pathLoss(double dis, double val){
    return (POWER_dBm + LOG_WAVE - 10 * val *log10(dis));
  }

  static double awgnQ(double sn){
    double nvar;
    double x,y;
    double noise;
  
    nvar= pow(10.0,((-1.0/10.0)*sn)) * 0.5; //分散の計算  
    x=0.0;
    while(x==0.0){
      x=randuni(mt);
    }
    y=randuni(mt);
    noise=sqrt(-2.0*nvar*log(x)) * cos(2.0*M_PI*y); //AWGN生成
   
    return noise;
  }

  // Box-Muller法に基づいて，各SNRに対するAWGNを作成する関数
  static double awgnI(double sn){
    double nvar;
    double x,y;
    double noise;
  
    nvar= pow(10.0,(-1.0/10.0)*sn) * 0.5; //分散の計算
    x=0.0;
    while(x==0.0){
      x=randuni(mt);
    }
    y=randuni(mt);
    noise=sqrt(-2.0*nvar*log(x)) * sin(2.0*M_PI*y); //AWGN生成
  
    return noise;
  }

  static complex<double> rayleigh(){
    double real1;
    double imag1;
    double nvar = 1.0 / 2.0;
    double x = 0;
    double y = randuni(mt);;
    double w = 0;
    double z = randuni(mt);

    while(x == 0.0) x = randuni(mt);
    real1 = sqrt(-2.0 * nvar * log(x)) * sin(2.0 * M_PI * y);
  
    while(w == 0.0) w = randuni(mt);
    imag1 = sqrt(-2.0 * nvar * log(w)) * cos(2.0 * M_PI * z);
    complex<double> fading(real1, imag1); //あとで確認
    
    return fading;
  }
};

#endif
