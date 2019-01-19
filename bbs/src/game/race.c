/*-------------------------------------------------------*/
/* race.c         ( NTHU CS MapleBBS Ver 3.10 )          */
/*-------------------------------------------------------*/
/* target : 賽馬場遊戲                                   */
/* create : 98/12/17                                     */
/* update : 01/04/21                                     */
/* author : SugarII (u861838@Oz.nthu.edu.tw)             */
/* recast : itoc.bbs@bbs.tnfsh.tn.edu.tw                 */
/*-------------------------------------------------------*/


#include "bbs.h"


#ifdef HAVE_GAME


static int pace[5];		/* 五匹馬跑了多遠 */


static void
out_song()
{
  static int count = 0;  

  /* 張雨˙一言難盡 */
  uschar *msg[13] = 
  {
    "妳給我一場戲  妳看著我入迷",
    "被妳從心裡剝落的感情  痛的不知怎麼捨去",
    "遲遲不能相信這感覺  像自己和自己分離",
    "而信誓旦旦的愛情  在那裡",
    "我一言難盡  忍不住傷心",
    "衡量不出愛或不愛之間的距離",
    "妳說妳的心  不再溫熱如昔",
    "從那裡開始  從那裡失去",
    "我一言難盡  忍不住傷心",
    "衡量不出愛或不愛之間的距離",
    "隱隱約約中  明白妳的決定",
    "不敢勉強妳  只好為難自己",
    "我為難我自己  我為難我自己"
  };
  move(b_lines - 2, 0);
  prints("\033[1;3%dm%s\033[m  籌碼還有 %d 元", time(0) % 7, msg[count], cuser.money);
  clrtoeol();
  if (++count == 13)
    count = 0;
}


static int			/* -1: 還沒分出勝負 0~4:贏的那匹馬 */
race_path(run, j, step)
  int run, j, step;
{
  int i;

  if (!step)
  {
    return -1;
  }
  else if (step < 0)
  {
    if (j + step < 0)
      j = -step;
    move(run + 9, (j + step) * 2 + 8);
    clrtoeol();
    pace[run] += step * 100;
    if (pace[run] < 1)
      pace[run] = 1;
    return -1;
  }

  /* step > 0 */
  move(run + 9, j * 2 + 8);
  for (i = 0; i < step; i++)
    outs("■");

  if (pace[run] + step * 100 > 3000)
    return run;
  return -1;
}


int
main_race()
{
  int money[5];			/* 五匹馬的押金 */
  int speed[5];			/* 五匹馬的速度 */
  int stop[5];			/* 五匹馬的暫停 */
  int bomb;			/* 是否使用炸彈 */
  int run;			/* 目前在計算的那匹 */
  int win;			/* 哪匹馬贏了 */
  int flag;			/* 事件發生次數 */
  int i, j, ch;
  char buf[60];
  char *racename[5] = {"赤兔", "的盧", "爪黃", "飛電", "汗血"};

  if (HAS_STATUS(STATUS_COINLOCK))
  {
    vmsg(msg_coinlock);
    return XEASY;
  }

  while (1)
  {
    vs_bar("賽馬場");
    out_song();
    bomb = 0;
    win = -1;
    flag = 0;
    for (i = 0; i < 5; i++)
    {
      pace[i] = 1;
      stop[i] = money[i] = 0;
      speed[i] = 100;
    }
    move(5, 0);
    outs("  \033[1m馬名：\033[m");
    for (i = 0; i < 5; i++)
      prints("     %d. \033[1;3%dm%s\033[m", i + 1, i + 1, racename[i]);
    outs("\n  \033[1m速度：\033[m\n  \033[1m賭金：\033[m\n\n");

    for (i = 0; i < 5; i++)
      prints("%d.\033[1;3%dm%s\033[m║\n", i + 1, i + 1, racename[i]);
    outs("───╨──┴──┴──┴──┴──┴──┴──┴──┴──┴──╜");

    while (1)
    {
      /* 決定各匹馬賭注 */
      ch = vget(2, 0, "您要押哪匹馬(1-5)？[S]開始 [Q]離開：", buf, 3, DOECHO);
      i = buf[0];
      if (!ch || i == 's')
      {
	if (money[0] || money[1] || money[2] || money[3] || money[4])
	  break;
	addmoney(money[0] + money[1] + money[2] + money[3] + money[4]);	/* 還錢 */
	goto abort_game;
      }
      else if (i < '1' || i > '5')
      {
	addmoney(money[0] + money[1] + money[2] + money[3] + money[4]);	/* 還錢 */
	goto abort_game;
      }

      ch = vget(3, 0, "要押多少賭金？", buf, 6, DOECHO);
      j = atoi(buf);
      if (!ch)
      {
	if (money[0] || money[1] || money[2] || money[3] || money[4])
	  break;
	addmoney(money[0] + money[1] + money[2] + money[3] + money[4]);	/* 還錢 */
	goto abort_game;
      }
      if (j < 1 || j > cuser.money)
      {
	addmoney(money[0] + money[1] + money[2] + money[3] + money[4]);	/* 還錢 */
	goto abort_game;
      }

      money[i - '1'] += j;
      cuser.money -= j;

      move(7, 15);
      clrtoeol();
      outs("\033[1m");
      for (i = 0; i < 5; i++)
	prints("     \033[3%dm%7d", i + 1, money[i]);
      outs("\033[m");
      out_song();
    }

    /* 開始遊戲 */
    move(3, 0);
    clrtoeol();
    move(2, 0);
    clrtoeol();
    outs("-== 請按 \033[1;36mk\033[m 為您選的勁駒加油，按 \033[1;36mz\033[m 可丟出炸彈(只有一次機會) ==-");

    while (win < 0)
    {
      move(6, 15);
      clrtoeol();
      outs("\033[1m");
      for (i = 0; i < 5; i++)
      {
	if (stop[i] < 1)
	  speed[i] += rnd(20) - (speed[i] + 170) / 30;
	if (speed[i] < 0)
	  speed[i] = 0;
	prints("     \033[3%dm%7d", i + 1, speed[i]);
      }
      outs("\033[m");

      do
      {
	ch = igetch();
      } while (ch != 'k' && (ch != 'z' || bomb));

      run = rnd(5);		/* 選擇事件發生對象 */
      flag %= 5;		/* 列印事件於螢幕上 */
      move(15 + flag, 0);
      clrtoeol();

      if (ch == 'z')		/* 丟炸彈 */
      {
	stop[run] = 3;
	prints("\033[1m保齡球砸到\033[3%dm%s\033[37m停止前進三次，速度 = 0\033[m",
	  run + 1, racename[run]);
	speed[run] = 0;
	flag++;
	bomb = 1;
      }
      else if (rnd(12) == 0)	/* 特殊事件 */
      {
	prints("\033[1;3%dm%s\033[36m", run + 1, racename[run]);

	switch (rnd(14))
	{
	case 0:
	  outs("服下威而剛，速度 x1.5\033[m");
	  speed[run] *= 1.5;
	  break;
	case 1:
	  outs("使出熊的爆發力，前進五格\033[m");
	  win = race_path(run, pace[run] / 100, 5);
	  pace[run] += 500;
	  break;
	case 2:
	  outs("踩到地雷，速度減半\033[m");
	  speed[run] /= 2;
	  break;
	case 3:
	  outs("踩到香蕉皮滑倒，暫停二次\033[m");
	  stop[run] += 2;
	  break;
	case 4:
	  outs("請神上身，暫停四次，速度加倍\033[m");
	  stop[run] += 4;
	  speed[run] *= 2;
	  break;
	case 5:
	  outs("唱出大魔法咒，使其他人暫停三次\033[m");
	  for (i = 0; i < 5 && i != run; i++)
	    stop[i] += 3;
	  break;
	case 6:
	  outs("聽見 badboy 的加油聲，速度 +100\033[m");
	  speed[run] += 100;
	  break;
	case 7:
	  outs("使出鋼鐵變身，前進三格，速度 +30\033[m");
	  win = race_path(run, pace[run] / 100, 3);
	  speed[run] += 30;
	  break;
	case 8:
	  outs("衰神上身速度減半，旁邊暫停二次\033[m");
	  speed[run] /= 2;
	  if (run > 0)
	    stop[run - 1] += 2;
	  if (run < 4)
	    stop[run + 1] += 2;
	  break;
	case 9:
	  outs("被詛咒，回到起點\033[m");
	  win = race_path(run, pace[run] / 100, -30);
	  break;
	case 10:
	  if (pace[0] + pace[1] + pace[2] + pace[3] + pace[4] > 6000)
	  {
	    outs("\033[5m使出凱化薰陶，所有人回到起點\033[m");
	    for (i = 0; i < 5; i++)
	      win = race_path(i, pace[i] / 100, -30);
	  }
	  else
	  {
	    outs("使出企鵝彈跳，速度 x1.3，其他人減半\033[m");
	    for (i = 0; i < 5 && i != run; i++)
	      speed[i] /= 2;
	    speed[run] *= 1.3;
	  }
	  break;
	case 11:
	  if (money[run])
	  {
	    outs("撿到很多錢，暫停一次\033[m");
	    addmoney(money[run]);
	    out_song();
	    stop[run]++;
	  }
	  else
	  {
	    outs("整匹馬爽起來了，速度 +50\033[m");
	    speed[run] += 50;
	  }
	  break;
	case 12:
	  j = rnd(5);
	  prints("愛上了[3%dm%s[36m，速度跟牠一樣", j + 1, racename[j]);
	  speed[run] = speed[j];
	  break;
	case 13:
	  if (money[run] > 0)
	  {
	    outs("的賭金 x1.5，賺啦！\033[m");
	    money[run] *= 1.5;
	    move(7, 15);
	    clrtoeol();
	    outs("\033[1m");
	    for (i = 0; i < 5; i++)
	      prints("     \033[3%dm%7d ", i + 1, money[i]);
	    outs("\033[m");
	  }
	  else
	  {
	    outs("鞋子掉了，退後三格\033[m");
	    race_path(run, pace[run] / 100, -3);
	  }
	  break;
	}
      }
      else		/* 往前跑 */
      {
	if (stop[run])
	{
	  prints("\033[1;3%dm%s\033[37m 爬不起來\033[m", run + 1, racename[run]);
	  stop[run]--;
	}
	else
	{
	  prints("\033[1;3%dm%s\033[37m 拚命奔跑\033[m", run + 1, racename[run]);
	  i = pace[run] / 100;
	  win = race_path(run, i, (pace[run] + speed[run]) / 100 - i);
	  pace[run] += speed[run];
	}
      }
      flag++;
    }
    move(b_lines - 1, 0);
    prints("\033[1;35m★ \033[37m遊戲結束 \033[35m★ \033[37m獲勝的是\033[3%dm %s \033[m",
      win + 1, racename[win]);
    if (money[win])
    {
      money[win] += money[win] * (pace[win] - (pace[0] + pace[1] + pace[2] + pace[3] + pace[4]) / 5) / 500;
      sprintf(buf, "恭喜您押中了，獲得獎金 %d 元", money[win]);
      addmoney(money[win]);
    }
    else
    {
      strcpy(buf, "抱歉...您沒押中ㄛ~~~");
    }
    vmsg(buf);
  }

abort_game:
  return 0;
}
#endif				/* HAVE_GAME */
