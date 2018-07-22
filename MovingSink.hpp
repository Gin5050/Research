/*MovingSinkクラス*/
#ifndef MOVINGSINK_HEAD
#define MOVINGSINK_HEAD

class MovingSink
{
public:
  int recPackets;
  int recCount;
  int recBits;
  int countBits;
  int connectedNode;
  double x;
  double y;
  complex<double> fading;
  complex<double> m_prev;

  MovingSink()
  {
    x = 0;
    y = 0;
    recPackets = 0;
    recCount = 0;
    recBits = 0;
    countBits = 0;
    connectedNode = EMPTY;
    fading = complex<double>(0, 0);
    m_prev = complex<double>(0, 0);
  }

  void initialization()
  {
    x = 0;
    y = Y_RANGE / 2.0;
  }

  void receiveProcess(NODE *n_data, double t_count, ModeMemory *modeMemo)
  {
    int minNode = EMPTY;
    double sinr = 0;
    double fadingDb = 0;

    //送信端末がない場合終了
    if (CalcUtile::searchTx(x, y, n_data, modeMemo) == EMPTY)
    {
      initialazeRecProcess();
      return;
    }
    //送信端末内でもっとも距離が近いノード選択
    minNode = CalcUtile::MinNode(x, y, modeMemo->Trans_node, n_data);

    if (recCount == 0 && connectedNode == EMPTY)
    {
      sinr = CalcUtile::calcSinrJakes(n_data, x, y, minNode, modeMemo, t_count);
      connectedNode = minNode;
      m_prev = CalcUtile::getConsteration(n_data, sinr, minNode, t_count);
    }
    if (connectedNode == minNode)
    {
      if (recCount % BITS_PER_1us == 0)
      {
        sinr = CalcUtile::calcSinrJakes(n_data, x, y, minNode, modeMemo, t_count);
        recBits += DBPSK(sinr, n_data[minNode].jakes(t_count));
      }

      if (recBits == PACKETSIZE)
      {
        recPackets++;
        initialazeRecProcess();
        //return;
      }
    }
    if (recCount == BITS_COUNT - 4)
    { //あとで
      avbSinr.push_back(sinr);
      ber += (1 - (recBits / (double)PACKETSIZE));
      initialazeRecProcess();
      return;
    }
    recCount++;
  }

  int DBPSK(double sinr, complex<double> channelNum)
  {
    int i;
    double p_d = 0;
    complex<double> m_temp;

    //DBPSK 1us分の受信
    m_temp = complex<double>(1.0, 0);
    m_temp = m_temp * channelNum;
    m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));
    p_d = abs(arg(m_prev) - arg(m_temp));
    m_prev = m_temp;
    if (cos(p_d) < 0)
    { //1bitでも誤った場合は終了
      return 0;
    }
    return 1;
  }

  void initialazeRecProcess()
  {
    recBits = 0;
    connectedNode = EMPTY;
    recCount = 0;
    m_prev = complex<double>(0, 0);
  }

  ~MovingSink() {}
};

#endif
