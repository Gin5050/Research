void Memory_fill(NODE *n_data, int *shf, vector<int> &r2_data);
//void PPP(double *PPP_CDF);

/* void PPP(double *PPP_CDF){ */
/*   int i, j, k; */
/*   int split = 0; */
/*   int ans; */
/*   double factorial = 0; */
/*   double temp_term = 0; */
/*   double sensor_num = 0; */
/*   double temp; */

/*   /\*--------------------Poison分布生成---------------------------------*\/ */
/*   if(exp(-Lambda) < DBL_MIN){ //Underflowの場合 */
/*     split = 2; */
/*     while(0){ */
/*       if(exp(Lambda / split) > DBL_MIN) */
/* 	break; */
/*       else */
/* 	split++; */
/*     } */
/*   } */
  
/*   for(i = 0; i < TEMP_NUM; i++){ */
/*     temp_term = 1;       */
/*     if(i != 0){ */
/*       for(j = 1; j <= i; j++){	 */
/*   	temp_term = (Lambda / (double)j) * temp_term * exp(-Lambda / (double)i); */
/*       } */
/*       PPP_CDF[i] = temp_term + PPP_CDF[i - 1]; */
/*     } */
/*     else{ */
/*       for(k = 0; k < split; k++){ */
/* 	temp_term = temp_term * exp(-Lambda / (double)split); */
/*       } */
/*       PPP_CDF[i] = temp_term; */
/*     } */
    
/*     if(PPP_CDF[i] > 1) */
/*       break;     */
/*     //cout << i << "\t" << PPP_CDF[i] << endl; */
/*   } */
/* } */

void Memory_fill(NODE *n_data, int *shf, vector<int> &r2_data){
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
  for(i = 0; i < ARRAY_LEN(n_data); i++){
    //cout << n_data[i].x << "\t" << n_data[i].y << endl;
    for(j = 0; j < MEMORY; j++){
      min = 1000000;
      min_num = -1;
      for(k = 0; k < ARRAY_LEN(n_data); k++){
	if(i != k){
	  if(n_data[i].x > 0 && n_data[i].y > 0){ //1
	    if(n_data[k].x > 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis = dis1;
	    }
	    else if(n_data[k].x > 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;	      
	    }
	    else if(n_data[k].x < 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;	      
	    }
	    else if(n_data[k].x < 0 && n_data[k].y < 0){
	      //cout << "k = " << n_data[k].x << "\t" << n_data[k].y << endl;
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));     
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	      if(dis3 < dis) dis = dis3;	      
	      if(dis4 < dis) dis = dis4;
	      //cout << dis1 << "\t" << dis2 << "\t" << dis3 << "\t" << dis4 << "\t" << dis << endl;
	    }
	  }
	  else if(n_data[i].x < 0 && n_data[i].y > 0){//2
	    if(n_data[k].x < 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis = dis1;
	    }
	    else if(n_data[k].x < 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	    }
	    else if(n_data[k].x > 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	    }
	    else if(n_data[k].x > 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	      if(dis3 < dis) dis = dis3;
	      if(dis4 < dis) dis = dis4;	   
	    }
	  }
	  else if(n_data[i].x > 0 && n_data[i].y < 0){//3
	    if(n_data[k].x > 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis = dis1;
	    }
	    else if(n_data[k].x > 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;	      
	    }
	    else if(n_data[k].x < 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	    }
	    else if(n_data[k].x < 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	      if(dis3 < dis) dis = dis3;
	      if(dis4 < dis) dis = dis4;
	    }
	  }
	  else if(n_data[i].x < 0 && n_data[i].y < 0){//4
	    if(n_data[k].x < 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis = dis1;
	    }
	    else if(n_data[k].x < 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	    }
	    else if(n_data[k].x > 0 && n_data[k].y < 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	    }
	    else if(n_data[k].x > 0 && n_data[k].y > 0){
	      dis1 = sqrt(pow(n_data[i].x - n_data[k].x, 2) + pow(n_data[i].y - n_data[k].y, 2));
	      dis2 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y), 2));
	      dis3 = sqrt(pow(n_data[i].x - (n_data[k].x), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      dis4 = sqrt(pow(n_data[i].x - (n_data[k].x + X_RANGE), 2) + pow(n_data[i].y - (n_data[k].y + Y_RANGE), 2));
	      if(dis1 < dis2) dis = dis1;
	      else dis = dis2;
	      if(dis3 < dis) dis = dis3;
	      if(dis4 < dis) dis = dis4;	 
	    }
	  }	  
	
	  if(j == 0){
	    if(dis < min){
	      min = dis;	    
	      min_num = k;
	    }
	  }
	  else{
	    if(dis < min && dis > min_temp){
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
  for(i = 0; i < ARRAY_LEN(n_data); i++){
    n_data[i].x += X_RANGE/2.0;
    n_data[i].y += Y_RANGE/2.0;
    if(n_data[i].x > SN && n_data[i].x < EN) r2_data.push_back(i);
  }

  //cout << endl << endl;
  /* for(i = 0; i < 1; i++){ */
  /*   cout << i << "\t" << n_data[i].x << "\t" << n_data[i].y << endl; */
  /*   //cout << i << endl; */
  /*   for(j = 0; j < MEMORY; j++){ */
  /*     dis = sqrt(pow(n_data[i].x - n_data[n_data[i].memory[j]].x, 2) + pow(n_data[i].y - n_data[n_data[i].memory[j]].y, 2)); */
  /*     cout << n_data[n_data[i].memory[j]].x << "\t" << n_data[n_data[i].memory[j]].y << endl; */
  /*     //cout << "\t" << n_data[i].memory[j] << endl; */
  /*   } */
  /* } */
}
