/*Utilクラス*/
#ifndef CALCUTIL_HEAD
#define CALCUTIL_HEAD

class CalcUtile
{
public:
  CalcUtile() {}

  static void shuffle(int *sh, int size){
    int i, j;
    int temp;
    int temp1;
    int temp2;
    uniform_int_distribution<> randuni(0, size - 1);

    for(i = 0; i < size; i++) sh[i] = i;
    for(i = 0; i < size; i++){
      temp = randuni(mt);
      temp1 = sh[i];
      sh[i] = sh[temp];
      sh[temp] = temp1;
    }
  } 
  
  static double NodesDstance(int x1, int y1, int x2, int y2)
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

  static void Memory_fill(NODE *n_data, int N)
  {
    int i, j, k, l;
    int min_num = -1;
    int locate = 0;
    double min = 10000;
    double min_temp = -1;
    double dis1 = 0;
    double dis2 = 0;
    double dis3 = 0;
    double dis4 = 0;
    double dis = 0;

    vector<double> v_dis;

    //n_data[0].x = 599.9; n_data[0].y = 599.9;
    for (i = 0; i < N; i++)
    {
      //cout << n_data[i].x << "\t" << n_data[i].y << endl;
      for (j = 0; j < MEMORY; j++)
      {
        min = 1000000;
        min_num = -1;
        for (k = 0; k < N; k++)
        {
          if (i != k)
          {
            if (n_data[i].x > 0 && n_data[i].y > 0)
            { //1
              if (n_data[k].x > 0 && n_data[k].y > 0)
              {//1-1
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis = dis1;
              }
              else if (n_data[k].x > 0 && n_data[k].y < 0)
              {//1-2
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x < 0 && n_data[k].y > 0)
              {//1-3
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x < 0 && n_data[k].y < 0)
              {//1-4
                //cout << "k = " << n_data[k].x << "\t" << n_data[k].y << endl;
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
                if (dis3 < dis)
                  dis = dis3;
                if (dis4 < dis)
                  dis = dis4;
                //cout << dis1 << "\t" << dis2 << "\t" << dis3 << "\t" << dis4 << "\t" << dis << endl;
              }
            }
            else if (n_data[i].x < 0 && n_data[i].y > 0)
            { //2
              if (n_data[k].x < 0 && n_data[k].y > 0)
              {//2-1
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis = dis1;
              }
              else if (n_data[k].x < 0 && n_data[k].y < 0)
              {//2-2
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x > 0 && n_data[k].y > 0)
              {//2-3
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x > 0 && n_data[k].y < 0)
              {//2-4
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
                if (dis3 < dis)
                  dis = dis3;
                if (dis4 < dis)
                  dis = dis4;
              }
            }
            else if (n_data[i].x > 0 && n_data[i].y < 0)
            { //3
              if (n_data[k].x > 0 && n_data[k].y < 0)
              {//3-1
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis = dis1;
              }
              else if (n_data[k].x > 0 && n_data[k].y > 0)
              {//3-2
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x < 0 && n_data[k].y < 0)
              {//3-3
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x < 0 && n_data[k].y > 0)
              {//3-4
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
                if (dis3 < dis)
                  dis = dis3;
                if (dis4 < dis)
                  dis = dis4;
              }
            }
            else if (n_data[i].x < 0 && n_data[i].y < 0)
            { //4
              if (n_data[k].x < 0 && n_data[k].y < 0)
              {//4-1
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis = dis1;
              }
              else if (n_data[k].x < 0 && n_data[k].y > 0)
              {//4-2
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x > 0 && n_data[k].y < 0)
              {//4-3
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
              }
              else if (n_data[k].x > 0 && n_data[k].y > 0)
              {//4-4
                dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
                dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
                dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
                if (dis1 < dis2)
                  dis = dis1;
                else
                  dis = dis2;
                if (dis3 < dis)
                  dis = dis3;
                if (dis4 < dis)
                  dis = dis4;
              }
            }

            if (j == 0)
            {
              if (dis < min)
              {
                min = dis;
                min_num = k;
              }
            }
            else
            {
              if (dis < min && dis > min_temp)
              {
                min = dis;
                min_num = k;
              }
            }
          }
        }
        min_temp = min;
        n_data[i].memory[j] = min_num;
      }
    }
    for (i = 0; i < N; i++)
    {
      n_data[i].x += X_RANGE / 2.0;
      n_data[i].y += Y_RANGE / 2.0;
      // if (n_data[i].x > SN && n_data[i].x < EN)
      //   r2_data.push_back(i);
    }
  }

  ~CalcUtile() {}
};

#endif
