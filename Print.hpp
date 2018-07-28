/*Print専用クラス*/
#ifndef PRINT_HEAD
#define PRINT_HEAD

class Print
{
public:
  static void printBasicInfo()
  {
    cout << "---------------------------------------------------------" << endl;
    cout << "RepeatNum = " << REPEATNUM << "\tObserbation time = " << OBSERVE << "\tSint = " << Sint << endl;
    cout << "PACKETSIZE = " << PACKETSIZE << "\tBITS_COUNT = " << BITS_COUNT
         << "\tBits_per1u = " << BITS_PER_1us << endl;
    cout << "x range = " << X_RANGE << "\ty range = " << Y_RANGE << endl;
    cout << "DENSITY = " << DENSITY << "\tLambda = " << Lambda << endl;
    cout << "---------------------------------------------------------\n"
         << endl;

    cout << "#Sleep\t\t"
         << "#avrSleepTime\t\t"
         << "#avrTxTime\t\t"
         << "#avrCsmaTime\t\t" << endl;
    cout << "\t\t"
         << "#RecPackets\t\t"
         << "#TransPackets\t\t"
         << "# #ofNodes" << endl;
    cout << "\t\t"
         << "#advSinr\t\t"
         << "#BER\t\t\t"
         << endl
         << endl;
  }

  static void printNodeDetail(double sleep, NODE *n_data, int N)
  {
    double sleepTime = 0;
    double csmaTime = 0;
    double txTime = 0;

    cout << sleep << "\t\t";
    for (int i = 0; i < N; i++)
    {
      sleepTime += n_data[i].sleep_time;
      csmaTime += n_data[i].csma_time;
      txTime += n_data[i].tx_time;
    }
    sleepTime = sleepTime / (double)N;
    csmaTime = csmaTime / (double)N;
    txTime = txTime / (double)N;

    cout << fixed << setprecision(8) << sleepTime << "\t\t" << txTime << "\t\t" << csmaTime << endl;
    cout << defaultfloat;
  }

  static void printNodesMode(NODE *n_data, int N, double t_count)
  {
    for (int i = 0; i < N; i++)
    {
      if ((n_data[i].mode != SLEEP) && (n_data[i].mode != Re_Be_ACK))
      {
        cout << t_count << "\t" << i << "\t" << n_data[i].mode << endl;
      }
    }
  }

  static void printTransNodes(int N, double t_count, ModeMemory *modeMemo, NODE *n_data)
  {

    for (int i = 0; i < N; i++)
    {
      if (n_data[i].next_mode != n_data[i].mode)
      {
        if (n_data[i].next_mode == TRANSMIT)
        {
          cout << "Start Tx\t" << t_count << "\t" << i << "\t" << modeMemo->Trans_node.size() << endl;
        }
      }
    }
    // list<int>::iterator it = modeMemo->Trans_node.begin();
    // while(it != modeMemo->Trans_node.end()){
    //   cout << t_count << "\t" << *it << endl;
    //   ++it;
    // }
  }

  static void printInspectDBPSK()
  {
    double avb = 0;

    for (int i = 0; i < avbSinr.size(); i++)
    {
      avb += avbSinr[i];
    }
    cout << fixed << setprecision(8);
    cout << "\t\t" << avb / (double)avbSinr.size() << "\t\t" << ber / (double)avbSinr.size() << endl;
    cout << defaultfloat;
  }

  static void printNodesActiveTime(NODE *n_data, int N)
  {
    for (int i = 0; i < N; i++)
    {
      cout << i << "\t" << n_data[i].activetime << endl;
    }
  }

  static void printRecAndTransPacket(NODE *n_data, MovingSink *car, int N)
  {
    int cnt = 0;

    cout << "\t\t" << setfill(' ') << setw(8) << left << car->recPackets << "\t\t";
    for (int i = 0; i < N; i++)
    {
      cnt += n_data[i].transCount;
    }
    cout << setfill(' ') << setw(8) << left << cnt << "\t\t" << N << endl;
    cout << defaultfloat;
  }

  static void printNodeProcess(NODE *n_data, int N, double t_count)
  {

    //cout << t_count << endl;
    for (int i = 0; i < 1; i++)
    {
      if ((abs(n_data[i].activetime - t_count) < TCOUNT))
      {
        cout << "id =" << n_data[i].id << "\tActiveTime = " << n_data[i].activetime << endl;
      }
      if (n_data[i].ca_time < TCOUNT && n_data[i].prevState == CSMA)
      {
        cout << "CSMA\t"
             << "id =" << n_data[i].id << "\tca_tome = " << n_data[i].ca_time << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << endl;
      }
      if (abs(n_data[i].be_end - t_count) < TCOUNT)
      {
        cout << "BEACON\t"
             << "id =" << n_data[i].id << "\tTime = " << t_count << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << endl;
      }
      if (abs(n_data[i].re_be_end - t_count) < TCOUNT)
      {
        cout << "Re_be_ACK\t"
             << "id =" << n_data[i].id << "\tTime = " << t_count << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << endl;
      }
      if (n_data[i].next_mode != n_data[i].mode)
      {
        cout << "Mode Change\t"
             << "id =" << n_data[i].id << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << "\tprevState = " << n_data[i].prevState << endl;
      }
    }
  }

  static void printListSize(ModeMemory *modeMemo)
  {
    cout << "All_trans = " << modeMemo->All_trans.size() << endl;
    cout << "Trans = " << modeMemo->Trans_node.size() << endl;
    cout << "Beacon = " << modeMemo->Beacon_node.size() << endl;
    cout << "ACK = " << modeMemo->ACK_node.size() << endl;
  }
};

#endif
