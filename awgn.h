double awgn(double sn);

// Box-Muller法に基づいて，各SNRに対するAWGNを作成する関数
double awgnQ(double sn)
{
  double nvar;
  double x,y;
  double noise;
  
  nvar= pow(10.0,((-1.0/10.0)*sn)) * 0.5; //分散の計算
  //nvar = sqrt(NOISE);
  x=0.0;
  while(x==0.0)	x=randuni(mt);
  y=randuni(mt);
  noise=sqrt(-2.0*nvar*log(x)) * cos(2.0*M_PI*y); //AWGN生成
  //noise = nvar * sqrt(-2.0*log(x)) * cos(2.0*M_PI*y);
  
  return noise;
}


// Box-Muller法に基づいて，各SNRに対するAWGNを作成する関数
double awgnI(double sn)
{
  double nvar;
  double x,y;
  double noise;
  
  nvar= pow(10.0,(-1.0/10.0)*sn) * 0.5; //分散の計算
  //nvar = sqrt(NOISE);
  x=0.0;
  while(x==0.0)	x=randuni(mt);
  y=randuni(mt);
  noise=sqrt(-2.0*nvar*log(x)) * sin(2.0*M_PI*y); //AWGN生成
  //noise = nvar * sqrt(-2.0*log(x)) * sin(2.0*M_PI*y);
  
  return noise;
}
