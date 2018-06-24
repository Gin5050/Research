/*MovingSinkクラス*/
#ifndef MOVINGSINK_HEAD
#define MOVINGSINK_HEAD

class MovingSink{
 public:
  int recPackets;
  int recCount;
  int recBits;
  int connectedNode;
  double x;
  double y;
  complex<double> fading;
  complex<double> m_prev;
  
  MovingSink(){
    x = 0;
    y = 0;
    recPackets = 0;
    recCount = 0;
    recBits = 0;
    connectedNode = 0;
    fading = complex<double> (0,0);
    m_prev = complex<double> (0,0);  
  }

  void initialization(){
    x = 0;
    y = Y_RANGE / 2.0;    
  }

  void receiveProcess(Calculator *calc, NODE *n_data, double t_count){
    int minNode = EMPTY;
    double sinr = 0;
    double fadingDb = 0;
    
    if(calc->searchTx(x, y, n_data) == EMPTY){
      return;
    }
    minNode = CalcUtile::MinNode(x, y, calc->getTransNodes(), n_data);
    sinr = calc->calcSinr(n_data, x, y, minNode);
    fadingDb = 10 * log10(abs(n_data[minNode].jakes(t_count)) * abs(n_data[minNode].jakes(t_count)));
    if(recCount == 0){
      connectedNode = minNode;
      m_prev = CalcUtile::getConsteration(n_data, sinr, minNode, t_count);
    }
    recCount++;
    recBits += DBPSK(sinr, n_data[minNode].channel_num, minNode);
    if(connectedNode == minNode && recBits == PACKETSIZE){
      recPackets++;     
      initialazeRecProcess();
      return;
    }
    if(recCount == PACKETSIZE / DATARATE){ //あとで確認
      initialazeRecProcess();
      return;
    }
  }

  int DBPSK(double sinr, complex<double> channelNum, int txId){
    int i;
    double p_d = 0; 
    complex<double> m_temp;

    //DBPSK 1us分の受信
    for(i = 0; i < DATARATE * TCOUNT; i++){
      m_temp = complex<double>(1.0,0);
      m_temp = m_temp * channelNum;
      m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));     
      p_d = abs(arg(m_prev) - arg(m_temp));
      m_prev = m_temp;
      if(cos(p_d) < 0){ //1bitでも誤った場合は終了
	break;
      }
    }
    return i;
  }

  void initialazeRecProcess(){
    recBits = 0;
    connectedNode = EMPTY;
    recCount = 0;
    m_prev = complex<double> (0,0); 
  }
  
  ~MovingSink(){}
};

#endif
