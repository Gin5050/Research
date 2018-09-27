//Nodeに関連した処理を行う．
#ifndef OPERATOR_HEAD
#define OPERATOR_HEAD

class Operater
{
public:
  int N;
  int minNode;
  NODE *n_data;
  MovingSink *car;
  double sinr;
  double p_sleep;

  Operater(int nodeNum, double sleep)
  {
    n_data = new NODE[nodeNum];
    N = nodeNum;
    car = new MovingSink(); //受信車両オブジェクト生成
    minNode = EMPTY;
    sinr = 0;
    p_sleep = sleep;
  }

  void initialaizeNodes()
  {
    for (int i = 0; i < N; i++)
    {
      n_data[i].Initialaze(i);
    }

    //Nodeのメモリに一定範囲内のノード情報追加
    CalcUtile::Memory_fill(n_data, N);
  }

  void processNodes(double t_count, ModeMemory *modeMemo)
  {

    for (int i = 0; i < N; i++)
    {
      switch (n_data[i].mode)
      {
      case SLEEP:
        n_data[i].sleepProcess(t_count, p_sleep);
        break;

      case CSMA:
        n_data[i].csmaCaProcess(t_count, CalcUtile::CarrierSense(n_data, i, modeMemo), modeMemo);
        break;

      case BEACON:
        n_data[i].beaconProcess(t_count, modeMemo);
        break;

      case Re_Be_ACK:
        minNode = CalcUtile::searchAck(n_data[i].x, n_data[i].y, n_data, modeMemo);
        sinr = CalcUtile::calcSinr(n_data, n_data[i].x, n_data[i].y, minNode, modeMemo);
        n_data[i].Re_Be_ACK_Process(t_count, n_data, minNode, sinr);
        break;

      case TRANSMIT:
        n_data[i].txProcess(t_count, modeMemo);
        break;
      }
    }
  }

  void initialazeCar()
  {
    car->initialization();
  }

  void carReceiveProcess(double t_count, ModeMemory *modeMemo)
  {
    car->x += (TCOUNT * V_m);
    car->receiveProcess(n_data, t_count, modeMemo);
  }

  void updateNodes(ModeMemory *modeMemo)
  {
    for (int i = 0; i < N; i++)
    {
      if (n_data[i].next_mode != n_data[i].mode)
      {
        switch (n_data[i].next_mode)
        {
        case BEACON:
          modeMemo->addBeacon(i);
          break;

        case TRANSMIT:
          modeMemo->addTrans(i);
          break;

        case ACK:
          modeMemo->addAck(i);
          break;
        }
        switch (n_data[i].mode)
        {
        case BEACON:
          modeMemo->deleteBeacon(i);
          break;

        case TRANSMIT:
          modeMemo->deleteTrans(i);
          break;

        case ACK:
          modeMemo->deleteACKnodes(i);
          break;
        }
      }
      n_data[i].mode = n_data[i].next_mode;
    }
  }

  void printRecAndTransPacket()
  {
    Print::printRecAndTransPacket(n_data, car, N);
  }

  void printNodeProcess(double t_count)
  {
    Print::printNodeProcess(n_data, N, t_count);
  }

  void printListSize(ModeMemory *modeMemo)
  {
    Print::printListSize(modeMemo);
  }

  void printBasicInfo()
  {
    Print::printBasicInfo();
  }

  void printTransNodes(double t_count, ModeMemory *modeMemo)
  {
    Print::printTransNodes(N, t_count, modeMemo, n_data);
  }

  void printNodeDetail(double sleep)
  {
    Print::printNodeDetail(sleep, n_data, N);
  }

  int getNodeNum()
  {
    return N;
  }

  ~Operater(){};
};

#endif
