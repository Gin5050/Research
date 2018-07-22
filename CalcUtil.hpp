/*Utilクラス*/
#ifndef CALCUTIL_HEAD
#define CALCUTIL_HEAD

class CalcUtile
{
public:
  CalcUtile() {}

  static double NodesDistance(int x1, int y1, int x2, int y2)
  {
    return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
  }

  static int MinNode(double x, double y, list<int> nodes, NODE *n_data)
  {
    list<int>::iterator it;
    double distance;
    int min = 10000000;
    int nodeId = EMPTY;

    //if(nodes.empty() == TRUE) return EMPTY;
    /*他端末からの送信信号の受信電力を測定*/
    it = nodes.begin();
    while (it != nodes.end())
    {
      distance = NodesDistance(x, n_data[*it].x, y, n_data[*it].y);
      if (distance < min)
      {
        min = distance;
        nodeId = *it;
      }
      it++;
    }
    return nodeId;
  }

  static int CarrierSense(NODE *n_data, int id, ModeMemory *modeMemo)
  {
    list<int>::iterator it;
    double distance = 0;
    double CSLevel;
    complex<double> channel_coeff;

    /*他端末からの送信信号の受信電力を測定*/
    it = modeMemo->All_trans.begin();
    //cout << modeMemo->getAllTransNodes().size() << endl;
    while (it != modeMemo->All_trans.end())
    {
      if (id != *it)
      {
        distance = CalcUtile::NodesDistance(n_data[id].x, n_data[id].y, n_data[*it].x, n_data[*it].y);
        channel_coeff = Channel::rayleigh();
        CSLevel = Channel::pathLoss(distance, PATHLOSS_num) + abs(channel_coeff) * abs(channel_coeff);
        if (CSLevel > CA_dBm)
        {
          break; //CAレベルを超えていれば
        }
      }
      ++it;
    }

    if (it != modeMemo->All_trans.end())
    {
      return FALSE;
    }
    return TRUE;
  }

  static int searchBeacon(double x, double y, NODE *n_data, ModeMemory *modeMemo)
  {
    int nodeId = CalcUtile::MinNode(x, y, modeMemo->Beacon_node, n_data);

    if (nodeId == EMPTY)
    {
      return EMPTY;
    }
    return nodeId;
  }

  static int searchAck(double x, double y, NODE *n_data, ModeMemory *modeMemo)
  {
    int nodeId = CalcUtile::MinNode(x, y, modeMemo->ACK_node, n_data);

    if (nodeId == EMPTY)
    {
      return EMPTY;
    }
    return nodeId;
  }

  static int searchTx(double x, double y, NODE *n_data, ModeMemory *modeMemo)
  {
    int nodeId = EMPTY;
    nodeId += modeMemo->Trans_node.size();
    //CalcUtile::MinNode(x, y, modeMemo->Trans_node, n_data);

    if (nodeId == EMPTY)
    {
      return EMPTY;
    }
    return nodeId;
  }

  static double calcSinr(NODE *n_data, double x, double y, int minNode, ModeMemory *modeMemo)
  {
    list<int>::iterator it = modeMemo->All_trans.begin();
    complex<double> chnlCoeff;
    double distance = 0;
    double loss = 0;
    double fading = 0;
    double interference = 0;
    //int min = CalcUtile::MinNode(n_data, id, All_trans);

    while (it != modeMemo->All_trans.end())
    {
      if (*it != minNode)
      {
        distance = CalcUtile::NodesDistance(x, y, n_data[*it].x, n_data[*it].y);
        loss = Channel::pathLoss(distance, PATHLOSS_num);
        chnlCoeff = Channel::rayleigh();
        fading = 10 * log10(abs(chnlCoeff) * abs(chnlCoeff));
        interference += (pow(10, (fading + loss) / 10.0));
      }
      it++;
    }
    distance = CalcUtile::NodesDistance(x, y, n_data[minNode].x, n_data[minNode].y);
    loss = Channel::pathLoss(distance, PATHLOSS_num);
    chnlCoeff = Channel::rayleigh();
    ;
    fading = 10 * log10(abs(chnlCoeff) * abs(chnlCoeff));

    return (loss + fading - (10 * log10(interference + NOISE)));
  }

  static double calcSinrJakes(NODE *n_data, double x, double y, int minNode, ModeMemory *modeMemo, double t_count)
  {
    list<int>::iterator it = modeMemo->All_trans.begin();
    complex<double> chnlCoeff;
    double distance = 0;
    double loss = 0;
    double fading = 0;
    double interference = 0;

    while (it != modeMemo->All_trans.end())
    {
      if (*it != minNode)
      {
        distance = CalcUtile::NodesDistance(x, y, n_data[*it].x, n_data[*it].y);
        loss = Channel::pathLoss(distance, PATHLOSS_num);
        chnlCoeff = n_data[*it].jakes(t_count);
        fading = 10 * log10(abs(chnlCoeff) * abs(chnlCoeff));
        interference += (pow(10, (fading + loss) / 10.0));
      }
      it++;
    }
    distance = CalcUtile::NodesDistance(x, y, n_data[minNode].x, n_data[minNode].y);
    loss = Channel::pathLoss(distance, PATHLOSS_num);
    return (loss - (10 * log10(interference + NOISE)));
  }

  static complex<double> getConsteration(NODE *n_data, double sinr, int txId, double t_count)
  {
    complex<double> x;
    complex<double> m_temp;

    x = n_data[txId].jakes(t_count);

    //1シンボル遅延
    m_temp = complex<double>(1.0, 0);
    m_temp = m_temp * x;
    m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));
    return m_temp;
  }

  static int PPP(vector<double> &PPP_CDF, double randomValue)
  {
    int i, j, k;
    int split = 1;
    double factorial = 0;
    double temp_term = 0;

    /*--------------------Poison分布生成---------------------------------*/
    if (exp(-Lambda) < DBL_MIN)
    { //Underflowの場合
      split = 2;
      while (0)
      {
        if (exp(Lambda / split) > DBL_MIN)
          break;
        else
          split++;
      }
    }

    for (i = 0; i < TEMP_NUM; i++)
    {
      temp_term = 1;
      if (i != 0)
      {
        for (j = 1; j <= i; j++)
        {
          temp_term = (Lambda / (double)j) * temp_term * exp(-Lambda / (double)i);
        }
        PPP_CDF[i] = temp_term + PPP_CDF[i - 1];
      }
      else
      {
        for (k = 0; k < split; k++)
        {
          temp_term = temp_term * exp(-Lambda / (double)split);
          //cout << temp_term << endl;
        }
        PPP_CDF[i] = temp_term;
      }

      //cout << PPP_CDF[i] << "\t" << temp_term << endl;
      if (isinf(PPP_CDF[i]) == TRUE)
      {
        cout << "inf = " << PPP_CDF[i] << endl;
      }

      if (PPP_CDF[i] > 1)
        break;
    }

    for (i = 0; PPP_CDF.size(); i++)
    {
      if (randomValue < PPP_CDF[i])
        return i;
    }
    return EMPTY;
  }

  ~CalcUtile() {}
};

#endif
