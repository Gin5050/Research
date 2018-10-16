/*ノードクラス*/
#ifndef NODE_HEAD
#define NODE_HEAD

class NODE
{
public:
  int id;
  int state;
  int mode;
  int next_mode;
  int next_state;
  int prevState;
  int rec_time;
  int connect;
  int rec_cnt;
  int transCount;
  int memory[MEMORY];
  double x;
  double y;
  double activetime;
  double sleep_time;
  double csma_time;
  double ca_time;
  double tx_time;
  double rx_time;
  double be_end;
  double re_be_end;
  double txEnd;
  double phase0[WAVENUM];
  double arrival_angle[WAVENUM];
  complex<double> fading;
  complex<double> m_prev;
  complex<double> channel_num;
  complex<double> I;

  NODE()
  {
    id = -1;
    state = FALSE;
    next_state = FALSE;
    prevState = FALSE;
    mode = SLEEP;
    next_mode = FALSE;
    rec_time = 0;
    connect = EMPTY;
    rec_cnt = 0;
    transCount = 0;
    x = 0;
    y = 0;
    activetime = 0;
    sleep_time = 0;
    ca_time = 0;
    tx_time = 0;
    rx_time = 0;
    be_end = 0;
    re_be_end = 0;
    fading = complex<double>(0, 0);
    m_prev = complex<double>(0, 0);
    channel_num = complex<double>(0, 0);
    I = complex<double>(0, 1);
  }
  ~NODE() {}

  void Initialaze(int num)
  {
    int i;
    uniform_real_distribution<> randX(0, X_RANGE);
    uniform_real_distribution<> randY(0, Y_RANGE);
    uniform_real_distribution<> randSint(0, Sint);
    uniform_real_distribution<> PHASE(0, 2 * M_PI);

    id = num;
    next_mode = SLEEP;
    x = randX(mt);
    y = randY(mt);
    activetime = randSint(mt);

    for (i = 0; i < WAVENUM; i++)
    {
      phase0[i] = PHASE(mt);
      arrival_angle[i] = PHASE(mt) + 2 * M_PI * ((double)i / (double)WAVENUM);
    }
  }

  /*---------------------各モード処理-------------------------*/
  void sleepProcess(double t_count, double p_sleep)
  {
    double temp;
    uniform_int_distribution<> randCW(0, CW - 1);

    sleep_time += TCOUNT;
    if (abs(t_count - activetime) < TCOUNT)
    {
      if (state == SLEEP_TEMP)
      {
        next_mode = RECEIVE;
        return;
      }
      temp = randuni(mt);
      if (temp < p_sleep)
      {
        next_mode = SLEEP;
        next_state = SLEEP;
        activetime = t_count + Sint;
        return;
      }
      next_mode = CSMA;
      next_state = CSMA;
      prevState = SLEEP;
      ca_time = SIFS + TIMESLOT * randCW(mt);
    }
  }

  void beaconProcess(double t_count, ModeMemory *modeMemo)
  {
    if (abs(be_end - t_count) < TCOUNT)
    {
      next_mode = Re_Be_ACK;
      prevState = BEACON;
      state = Re_Be_ACK;
      re_be_end = t_count + BEACON_TIME + CW * TIMESLOT + SIFS;
    }
    tx_time += TCOUNT;
  }

  void csmaCaProcess(double t_count, int flag, ModeMemory *modeMemo)
  {
    csma_time += TCOUNT;
    if (flag == FALSE)
    {
      return;
    }
    if (ca_time > TCOUNT)
    {
      ca_time -= TCOUNT;
      return;
    }
    switch (prevState)
    {
    case SLEEP:
      CsmaCaToBeacon(t_count, modeMemo);
      break;

    case Re_Be_ACK:
      CsmaCaToTransmit(t_count, modeMemo);
      break;
    }
  }

  void txProcess(double t_count, ModeMemory *modeMemo)
  {
    if (abs(txEnd - t_count) < TCOUNT)
    {
      next_mode = SLEEP;
      next_state = SLEEP;
      prevState = TRANSMIT;
      activetime = Sint + t_count;
      //cout << "End Tx\t" << t_count << "\t" << id << endl;
    }
    tx_time += TCOUNT;
  }

  void Re_Be_ACK_Process(double t_count, NODE *n_data, int minNode, double sinr)
  {
    uniform_int_distribution<> randCW(0, CW - 1);

    rx_time += TCOUNT;
    if (abs(re_be_end - t_count) < TCOUNT)
    {
      next_mode = CSMA;
      next_state = CSMA;
      prevState = Re_Be_ACK;
      ca_time = SIFS + TIMESLOT * randCW(mt);
      rec_time = 0;
    }
    /*受信処理*/
    if (minNode == EMPTY)
    {
      return;
    }
    DBPSK(sinr, minNode);
    if (rec_cnt == PACKETSIZE)
    {
      /*Senderからの信号受信処理*/
      return;
    }
  }

  /*----------------------------------------------------------*/
  /*------------------Mode移行処理-----------------------------*/
  void CsmaCaToTransmit(double t_count, ModeMemory *modeMemo)
  {
    next_mode = TRANSMIT;
    next_state = TRANSMIT;
    txEnd = t_count + ((double)PACKETSIZE / DATARATE);
    transCount++;
    //modeMemo->addTrans(id);
  }

  void CsmaCaToBeacon(double t_count, ModeMemory *modeMemo)
  {
    next_mode = BEACON;
    next_state = BEACON;
    be_end = t_count + BEACON_TIME; //Beacon送信時間は1ms
    //modeMemo->addBeacon(id);
  }
  /*----------------------------------------------------------*/

   //LT符号
  LtCode ltEncode() 
  {
    LtCode code;
    code.defineEdge(memory, MEMORY);   

    return code;
  }

  void DBPSK(double sinr, int txId)
  {
    int i;
    double p_d = 0;
    complex<double> x;
    complex<double> m_temp;

    if (rec_time == 0)
    {
      x = Channel::rayleigh();
      fading = x;
      connect = txId;

      //1シンボル遅延
      m_temp = complex<double>(1.0, 0);
      m_temp = m_temp * x;
      m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));
      m_prev = m_temp;
    }
    rec_time++;

    //DBPSK 1us分の受信
    for (i = 0; i < DATARATE * TCOUNT; i++)
    {
      m_temp = complex<double>(1.0, 0);
      m_temp = m_temp * fading;
      m_temp += complex<double>(Channel::awgnQ(sinr), Channel::awgnI(sinr));
      p_d = abs(arg(m_prev) - arg(m_temp));
      m_prev = m_temp;
      if (cos(p_d) < 0)
      { //1bitでも誤った場合は終了
        break;
      }
    }
    rec_cnt += i;
  }

  complex<double> jakes(double t_count)
  {
    int i;
    channel_num = complex<double>(0, 0);

    for (i = 0; i < WAVENUM; i++)
    {
      channel_num += exp(I * (phase0[i] + 2 * M_PI * DOPPLER * cos(arrival_angle[i]) * t_count));
    }
    channel_num = channel_num / (sqrt(WAVENUM));
    return channel_num;
  }
};

#endif
