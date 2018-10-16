#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <complex>
#include <random>
#include <fstream>
#include <list>
#include <cfloat>

using namespace std;;

#include "Definition.hpp"
#include "Util.hpp"
#include "Channel.hpp"
#include "ModeMemory.hpp"
#include "LtCode.hpp"
#include "Node.hpp"
#include "CalcUtil.hpp"
#include "MovingSink.hpp"
#include "Print.hpp"
#include "Operator.hpp"

int main(){
  int i, j;
  int nowround;
  double cnt = 1.0;
  double temp;
  double p_sleep;
  double t_count;  
  Operater *operate;  
  ModeMemory *modeMemo;
  vector<double> PPP_CDF(TEMP_NUM);

  //Todo
  //各スリープ確率ごとの結果表示

    Print::printBasicInfo();

    for(p_sleep = 0.5; p_sleep < 0.9; p_sleep += 0.1){    
      for(nowround = 0; nowround < REPEATNUM; nowround++){
        
        //操作用オブジェクト生成
        operate = new Operater((CalcUtile::PPP(PPP_CDF, randuni(mt))), p_sleep);
        
        //Nodeの状態記憶用オブジェクト生成
        modeMemo = new ModeMemory();
        
        //Nodesを初期化
        operate->initialaizeNodes();

        //車両を初期化
        operate->initialazeCar();
        
        t_count = 0;

        //観測開始
        while (t_count < OBSERVE)
        {          
          //各Nodesの処理分岐
          operate->processNodes(t_count, modeMemo);
         
          //車両受信処理
          operate->carReceiveProcess(t_count, modeMemo);

          //Nodeの状態を更新
          operate->updateNodes(modeMemo);

          //LT 復号
        /*   if(abs(((double)(t_count / OBSERVE) * 100) - cnt) < 1.0){
             cout << ((t_count / OBSERVE) * 100) << endl;
             cnt+=1.0;
          } */

          t_count += TCOUNT;
        }
        //結果print
        operate->printNodeDetail(p_sleep);
        operate->printRecAndTransPacket();
        Print::printInspectDBPSK();
        
        delete operate;
        delete modeMemo;
      }
    }
    return 0;
  }