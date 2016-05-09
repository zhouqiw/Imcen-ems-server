#include "server.h"

/* 初始化table为usr数据库 */
void init_usr_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "insert into usr(fd, sin_family, sin_port, sin_addr) \
	values ( 0,0,0,'0')");
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("init:%s\n", errmsg);
		exit (0);
	}

}
/*  删除所有fd用户信息   */
void del_allusr_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "delete from usr");
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_allusr:%s\n", errmsg);
		exit (0);
	}
}

/*  删除fd用户信息   */
void del_usr_sqlite3 (sqlite3 * db, int fd)
{
	int ret = -1;
	sprintf (sql, "delete from usr where fd =%d ", fd);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_usr:%s\n", errmsg);
		exit (0);
	}
}
/*  删除所以usr_online用户信息   */
void del_allusr_online_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "delete from usr_online");
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_allusr_online:%s\n", errmsg);
		exit (0);
	}
}
/*  删除所以dev_online用户信息   */
void del_alldev_online_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "delete from dev_online");
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_alldev_online:%s\n", errmsg);
		exit (0);
	}
}
/*  删除usr_online中fd用户信息   */
void del_usr_online_by_id_sqlite3 (sqlite3 * db,int fd)
{
	int ret = -1;
	if(fd<1)
		return;
	sprintf (sql, "delete from usr_online where fd =%d", fd);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_usr_online_by_id:%s\n", errmsg);
		exit (0);
	}
}
/*  删除dev_online中fd用户信息   */
void del_dev_online_by_id_sqlite3 (sqlite3 * db,int fd)
{
	int ret = -1;
	if(fd<1)
		return;
	sprintf (sql, "delete from dev_online where fd =%d", fd);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_dev_online_by_id:%s\n", errmsg);
		exit (0);
	}
}

/*  删除usr_online中tel用户信息   */
void del_usr_online_by_tel_sqlite3 (sqlite3 * db,char *tel)
{
	int ret = -1;
	if(tel==NULL)
		return;
	sprintf (sql, "delete from usr_online where tel =%s", tel);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_usr_online:%s\n", errmsg);
		exit (0);
	}
}
/*  删除dev_online中dev用户信息   */
void del_dev_online_by_dev_sqlite3 (sqlite3 * db,char *dev)
{
	int ret = -1;
	if(dev==NULL)
		return;
	sprintf (sql, "delete from dev_online where d_id ='%s'", dev);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("del_dev_online_by_dev:%s\n", errmsg);
		exit (0);
	}
}

//向table为usr的数据库中插入数据
void insert_usr_sqlite3 (sqlite3 * db, usrIP_info_t cli)
{
	int ret = -1;
	sprintf (sql, "insert into usr(fd, sin_family, sin_port, sin_addr) \
	values ( %d,%d,%d,'%s')", cli.fd, ntohs (cli.usr.sin_family), ntohs (cli.usr.sin_port), inet_ntoa (cli.usr.sin_addr));
ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("select :%s\n", errmsg);
		exit (0);
	}

}
//向table为user_info的数据库中查询数据获取设备列表
void get_devlist_by_tel_sqlite3 (sqlite3 * db, dev_list_t info,char (*dev)[5][10])
{
	int ret = -1, i;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from user_info where tel=%s ", info.tel);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		for (i = 0; i < ncolumn-3; i++) {
			PRINT ("%s\t", result[i+1]);
		}
		PRINT ("\n");
		for (i = 1; i < nrow + 1; i++) {
			strncpy((*dev)[0] , result[i * ncolumn + 1],10);
			strncpy((*dev)[1] , result[i * ncolumn + 2],10);
			strncpy((*dev)[2] , result[i * ncolumn + 3],10);
			strncpy((*dev)[3] , result[i * ncolumn + 4],10);
			strncpy((*dev)[4] , result[i * ncolumn + 5],10);
			PRINT ("%s%s%s%s%s\n", (*dev)[0],(*dev)[1],(*dev)[2],(*dev)[3],(*dev)[4]);
		}	
	}
}
//向table为user_online的数据库中插入数据
int blind_dev_by_tel_sqlite3 (sqlite3 * db, blind_dev_t info)
{
	int ret = -1, i;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	char dev[5][10];
	sprintf (sql, "select * from user_info where tel=%s ", info.tel);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		for (i = 0; i < ncolumn-3; i++) {
			PRINT ("%s\t", result[i+1]);
		}
		PRINT ("\n");
		for (i = 1; i < nrow + 1; i++) {
			strncpy(dev[0] , result[i * ncolumn + 1],10);
			strncpy(dev[1] , result[i * ncolumn + 2],10);
			strncpy(dev[2] , result[i * ncolumn + 3],10);
			strncpy(dev[3] , result[i * ncolumn + 4],10);
			strncpy(dev[4] , result[i * ncolumn + 5],10);
			PRINT ("%s%s%s%s%s\n", dev[0],dev[1],dev[2],dev[3],dev[4]);
		}	
		for(i=0;i<5;i++)
		{
			if(strncmp(dev[i],info.device_id,9)==0)
				return 0;
			if(strncmp(dev[i],"xxxxxxxx",8)==0)
				break;
		}
		if(i==5){
			return 0;
		}else{
			sprintf (sql, "update user_info set dev%d ='%s' where tel=%s ", i+1,info.device_id,info.tel);
			ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
			memset(sql,0,SQL_SIZE);
			if (SQLITE_OK != ret) {
				PRINT ("update user_info :%s\n", errmsg);
				exit (0);
			}
			return 1;
		}
	}else{
		return 0;
	}
}

//向table为usr_online的数据库中插入数据
void insert_usr_online_sqlite3 (sqlite3 * db, usrIP_info_t cli,user_info_t info)
{
	int ret = -1;
	sprintf (sql, "insert into usr_online(tel,fd, sin_family, sin_port, sin_addr) \
	values ( '%s',%d,%d,%d,'%s')", info.tel,cli.fd, ntohs (cli.usr.sin_family), ntohs (cli.usr.sin_port), inet_ntoa (cli.usr.sin_addr));
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("insert:%s\n", errmsg);
		exit (0);
	}
}
//向table为dev_online的数据库中插入数据
void insert_dev_online_sqlite3 (sqlite3 * db, usrIP_info_t cli,environment_t info)
{
	int ret = -1;
	sprintf (sql, "insert into dev_online(d_id,fd, sin_family, sin_port, sin_addr) \
	values ( '%s',%d,%d,%d,'%s')", info.device_id,cli.fd, ntohs (cli.usr.sin_family), ntohs (cli.usr.sin_port), inet_ntoa (cli.usr.sin_addr));
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("insert:%s\n", errmsg);
		exit (0);
	}
}
/*   向数据库插入用户信息 
     传入数据库名称和要插入的结构体信息（用户注册登录结构体）
     再跟据相应的device_id插入到相应的数据表中
*/
void insert_user_sqlite3 (sqlite3 * db, user_info_t  info)
{
	int ret = -1 ;
	sprintf (sql, "insert into user_info(tel ,dev1, dev2, dev3, dev4,dev5, code)values ( '%s', 'xxxxxxxx', 'xxxxxxxx','xxxxxxxx', 'xxxxxxxx', 'xxxxxxxx', %d)",info.tel,info.code);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("insert:%s\n", errmsg);
		exit (0);
	}
}


/*   向数据库插入环境监控数据信息 
     传入数据库名称和要插入的结构体信息（监控终端环境信息结构体）
     再跟据相应的device_id插入到相应的数据表中
*/
void insert_dev_env_sqlite3 (sqlite3 * db, environment_t env)
{
	int ret = -1 ;
	char s1[13] = "dev_";
	char device_id[9];
	strncpy(device_id,env.device_id,9);
	strcat (s1, device_id);
	sprintf (sql, "insert into %s(device_id ,date, time, temp, hum,pm2_5, tvoc, sound,lx,hcho )values ( '%s', '%s', '%s',%.1f, %.1f, %.3f, %.3f,%.1f,%.1f,%.3f)",s1,env.device_id ,env.date,env.time,env.temp, env.hum,env.pm2_5, env.tvoc, env.sound,env.lx,env.hcho );
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("insert:%s\n", errmsg);
		exit (0);
	}
}


//创建table为usr_online的数据库
void create_usr_online_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "create table  if not exists usr_online (tel varchar(12),fd int, sin_family int, sin_port int, sin_addr varchar(17)) ");

	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("create:%s\n", errmsg);
		exit (0);
	}

}
//创建table为dev_online的数据库
void create_dev_online_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "create table  if not exists dev_online (d_id varchar(12),fd int, sin_family int, sin_port int, sin_addr varchar(17)) ");

	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("create:%s\n", errmsg);
		exit (0);
	}

}
//创建table为usr的数据库
void create_usr_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "create table  if not exists usr(fd int, sin_family int, \
	sin_port int, sin_addr varchar(17)) ");

	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("create:%s\n", errmsg);
		exit (0);
	}

}
//创建table为user_info的数据库专用于存放用户注册登录信息
void create_user_info_sqlite3 (sqlite3 * db)
{
	int ret = -1;
	sprintf (sql, "create table if not exists user_info ( tel varchar(12),dev1 varchar(12),dev2 varchar(12),dev3 varchar(12),dev4 varchar(12),dev5 varchar(12),code int)");
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("create:%s\n", errmsg);
		exit (0);
	}
//	PRINT("创建user_info成功\n");
}
//创建table为device_id的数据库
void create_dev_sqlite3 (sqlite3 * db,  environment_t env)
{
	int ret = -1;
	char s1[13] = "dev_";
	char device_id[9];
	strncpy(device_id,env.device_id,9);
	strcat (s1, device_id);
	sprintf (sql, "create table if not exists %s ( device_id varchar(13),date varchar(12),time varchar(6),temp double,hum double,pm2_5 double,tvoc double,sound double,lx double,hcho double)", s1);
	ret = sqlite3_exec (db, sql, NULL, NULL, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("create:%s\n", errmsg);
		exit (0);
	}

}

//在table为usr的数据库中查询fd的用户并返回用户信息结构体 
usrIP_info_t *select_usr1_sqlite3 (sqlite3 * db, usrIP_info_t * cli, int fd)
{
	int ret = -1, i;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from usr where fd=%d ", fd);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		for (i = 0; i < ncolumn; i++) {
			PRINT ("%s\t", result[i]);
		}
		PRINT ("\n");
		for (i = 1; i < nrow + 1; i++) {
			cli->fd = atoi (result[i * ncolumn]);
			cli->usr.sin_family = atoi (result[i * ncolumn + 1]);
			cli->usr.sin_port = atoi (result[i * ncolumn + 2]);
			cli->usr.sin_addr.s_addr = inet_addr (result[i * ncolumn + 3]);
			PRINT ("%d\t", cli->fd);
			PRINT ("%d\t\t", cli->usr.sin_family);
			PRINT ("%d\t\t", cli->usr.sin_port);
			PRINT ("%s\n", inet_ntoa (cli->usr.sin_addr));
		}	
		return cli;
	
	}else{
		return NULL;
	}
}
//在table为usr_online的数据库中获取所有的fd
int select_usr_online_sqlite3 (sqlite3 * db, int *s,char (*hb)[BUFSIZ][12] )
{
	int ret = -1, i=0;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from usr_online ");
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		//打印查询结果
		PRINT ("遍历数据库查看在线用户\n");
		PRINT ("%s\t\t%s\n", result[0],result[1]);
		for (i = 1; i < nrow + 1; i++) {
			s[i - 1] = atoi (result[i * ncolumn+1]);
			strncpy((*hb)[i- 1],result[i*ncolumn],11);
			PRINT ("%s\t%d\n",(*hb)[i-1], s[i - 1]);
		}
		return i-1;
	}
	return 0;
}
//查询在线设备是否存在
int is_online_dev_sqlite3 (sqlite3 * db, char * info)
{
	int ret = -1;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from dev_online where d_id = '%s'", info);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow==0)
		return 0;
	else
		return 1;
}

//在table为dev_online的数据库中获取所有的fd
int select_dev_online_sqlite3 (sqlite3 * db, int *s,char (*hb)[BUFSIZ][12] )
{
	int ret = -1, i=0;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from dev_online ");
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		//打印查询结果
		PRINT ("遍历数据库查看在线设备\n");
		PRINT ("%s\t\t%s\n", result[0],result[1]);
		for (i = 1; i < nrow + 1; i++) {
			s[i - 1] = atoi (result[i * ncolumn+1]);
			strncpy((*hb)[i- 1],result[i*ncolumn],11);
			PRINT ("%s\t%d\n",(*hb)[i-1], s[i - 1]);
		}
		return i-1;
	}
	return 0;
}


//在table为usr的数据库中获取所有的fd
int select_fd_from_online_tel_sqlite3 (sqlite3 * db, int *s,char *d_id)
{
	int ret = -1, i,j,k,num,num_t=0;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	char (*tel)[BUFSIZ][12] ;
	int fd_t[BUFSIZ]={0};
	if((tel=(char (*)[BUFSIZ][12])malloc(sizeof(char [BUFSIZ][12])))==NULL)
	{
		perror("malloc error");
		exit(0);
	}

	sprintf (sql, "select * from usr_online");
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		//打印查询结果
		PRINT ("%s\n", result[0]);
		for (i = 1; i < nrow + 1; i++) {
			strncpy((*tel)[i - 1] , result[i * ncolumn],12);
			fd_t[i-1]=atoi(result[i*ncolumn+1]);
			PRINT ("%s\n", (*tel)[i - 1]);
		}
		num=i-1;

		for(j=0;j<num;j++)
		{
			sprintf (sql, "select * from user_info where tel=%s",(*tel)[j]);
			ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
			memset(sql,0,SQL_SIZE);
			if (SQLITE_OK != ret) {
				PRINT ("sqlite3_get_table:%s\n", errmsg);
				exit (0);
			}
			if(nrow>0)
			{
				//打印查询结果
				PRINT ("%s\t\tdev_id\n", result[0]);
				for (i = 1; i < nrow + 1; i++) {
					for(k=0;k<5;k++)
					{
						if(strncmp(result[i*ncolumn+1+k],d_id,9)==0)
						{
							PRINT ("%s\t\t", (*tel)[j]);
							PRINT ("%s\n", d_id);
							s[num_t++]=fd_t[j];
						}
					}

				}	
			}
		}
		free(tel);
		return num_t;
	}	
	free(tel);
	return 0;
}


//在table为usr的数据库中获取所有的fd
int *select_usr_sqlite3 (sqlite3 * db, int *s)
{
	int ret = -1, i;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from usr ");
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
	{
		//打印查询结果
		PRINT ("%s\t", result[0]);
		for (i = 1; i < nrow + 1; i++) {
			s[i - 1] = atoi (result[i * ncolumn]);
			PRINT ("%d\t", s[i - 1]);
		}
		PRINT ("\n");
		return s;
	}
	return NULL;
}

//查询账户是否存在
int select_dev_reg_sqlite3 (sqlite3 * db, user_info_t * info)
{
	int ret = -1;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from user_info where tel = %s", info->tel);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow==0)
		return 0;
	else
		return 1;
}
//查询在线设备是否存在
int select1_dev_online_sqlite3 (sqlite3 * db,  environment_t info)
{
	int ret = -1;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from dev_online where d_id = '%s'", info.device_id);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow==0)
		return 1;
	else
		return 0;


}

//在table为device_id的数据库中查询密码
int select_device_code_sqlite3 (sqlite3 * db, user_info_t* info)
{
	int ret = -1;
	char **result;
	int nrow = -1;
	int ncolumn = -1;
	sprintf (sql, "select * from user_info where tel=%s",info->tel);
	ret = sqlite3_get_table (db, sql, &result, &nrow, &ncolumn, &errmsg);
	memset(sql,0,SQL_SIZE);
	if (SQLITE_OK != ret) {
		PRINT ("sqlite3_get_table:%s\n", errmsg);
		exit (0);
	}
	if(nrow>0)
		return atoi (result[2 * ncolumn - 2]);
	else
		return 0;
}


#if 1
int cli_date_handler (int fd)
{
	PRINT ("进入fd=%d客户 \n", fd);
	usrIP_info_t *clip;
	int ret = -1 ;
	char buf[BUFSIZ] = { 0 };
	char buf1[BUFSIZ] = { 0 };
	if ((clip = (usrIP_info_t *) malloc (sizeof (usrIP_info_t))) == NULL) {
		perror ("malloc error");
		exit (1);
	}
	PRINT ("在数据库中获取fd=%d 的用户信息\n", fd);

	select_usr1_sqlite3 (db, clip, fd);
	do {
		bzero(buf,BUFSIZ);
		ret = read (clip->fd, buf, BUFSIZ - 1);
	} while (ret == -1 && EINTR == errno);
	if (ret == 0||ret==-1) {
		PRINT ("客户端fd=%d退出\n", fd);
		del_usr_online_by_id_sqlite3(db,fd);
		return 5;
	}
/*	if (ret == -1) {
		perror ("read error");
		exit (1);
	}
*/
	PRINT ("收到消息%s\n", buf);
	int msg;
	msg = unpack_type (buf);
	PRINT ("消息类型为%d \n", unpack_type (buf));
	//PRINT ("消息%s \n", buf);
	switch (msg) {
		//处理数据
		case MSG_NONE://处理客户端退出问题
		{
			PRINT ("客户端（fd=%d）退出！\n", fd);
			close (fd);
			del_usr_online_by_id_sqlite3(db,fd);
			del_usr_sqlite3 (db, fd);
			break;
		}
		case MSG_REGEST_LOAD://注册登录消息
		{
			PRINT ("接收用户登录注册消息!\n");
			user_info_t *m1 = NULL;
			if ((m1 = (user_info_t *) malloc (sizeof (user_info_t))) == NULL) {
				perror ("malloc error");
				exit (1);
			}
			strcpy (buf1, unpack_json (buf));
			bzero(buf,BUFSIZ);
			PRINT ("解包为json 数据类型%s\n", buf1);
			unpack_user_info_t (m1, buf1);
			bzero(buf1,BUFSIZ);
			switch (m1->r_l) {
			case 0:
				{
					PRINT ("用户注册...\n");
					//遍历数据库中是否存在该用户
					if (select_dev_reg_sqlite3 (db, m1)) {
						PRINT ("用户存在...\n");
						m1->key = 0;	//用户已存在
						strcpy (buf, pack_user_info_t (m1));
						do {
							ret = write (fd, buf, strlen (buf));
							PRINT ("%s\n", buf);
							bzero(buf,BUFSIZ);
						} while (ret == -1 && EINTR == errno);
						if (ret == -1) {
							perror ("write error");
							exit (1);
						}
					}			//FIXME!!!!(待补充)返回用户已存在信息
					else {
						PRINT ("用户不存在(注册成功)...\n");	//用户不存在
						m1->key = 1;	//用户不存在
						strcpy (buf, pack_user_info_t (m1));
						insert_user_sqlite3 (db, *m1);	//创建用户(插入数据库user_info)
						do {
							ret = write (fd, buf, strlen (buf));
							PRINT ("%s\n", buf);
							bzero(buf,BUFSIZ);
						} while (ret == -1 && EINTR == errno);
						if (ret == -1) {
							perror ("write error");
							exit (1);
						}
					}
					//close(fd);
					break;

				}
			case 1:
				{
					PRINT ("用户登录\n");
					//遍历数据库查看密码是否正确
					if (select_dev_reg_sqlite3 (db, m1)) {
						//用户存在
						PRINT ("用户存在...\n");

						if (m1->code == select_device_code_sqlite3 (db, m1)) {
							PRINT ("密码正确...\n");
							insert_usr_online_sqlite3(db,*clip,*m1);
							m1->key = 1;	//用户已存在，密码正确
							strcpy (buf, pack_user_info_t (m1));
							do {
								ret = write (fd, buf, strlen (buf));
								PRINT ("%s\n", buf);
								bzero(buf,BUFSIZ);
							} while (ret == -1 && EINTR == errno);
							if (ret == -1) {
								perror ("write error");
								exit (1);
							}
							//密码正确
							//返回用户环境信息
						} else
						{
							PRINT ("密码错误...\n");//密码错误
						strcpy (buf, pack_user_info_t (m1));
						do {
							ret = write (fd, buf, strlen (buf));
							PRINT ("%s\n", buf);
							bzero(buf,BUFSIZ);
						} while (ret == -1 && EINTR == errno);
						if (ret == -1) {
							perror ("write error");
							exit (1);
						}
						}
						//密码错误
						//返回密码错误信息

					} else {
						PRINT ("用户不存在...\n");
						m1->key = 0;	//用户不存在
						strcpy (buf, pack_user_info_t (m1));
						do {
							ret = write (fd, buf, strlen (buf));
							PRINT ("%s\n", buf);
							bzero(buf,BUFSIZ);
						} while (ret == -1 && EINTR == errno);
						if (ret == -1) {
							perror ("write error");
							exit (1);
						}
					}
					//close(fd);
					free(m1);
					break;

				}
			}
			break;
		}

		case MSG_ENVIR_DEV:
		{
			PRINT("环控终端发来环境数据...\n");
			environment_t *env = NULL;
			if ((env = (environment_t*) malloc (sizeof (environment_t ))) == NULL) {
				perror ("malloc error");
				exit (1);
			}
			strcpy (buf1, unpack_json (buf));
			PRINT ("解包为json 数据类型%s\n", buf1);
			unpack_environment_t (env, buf1);
			bzero(buf1,BUFSIZ);

			create_dev_sqlite3(db,*env);	
			if(select1_dev_online_sqlite3(db,*env)>0)
				insert_dev_online_sqlite3(db,*clip,*env);
			insert_dev_env_sqlite3(db,*env);

			int fd_s[BUFSIZ]={0};
			int online_num=0,i;
			//同时转发给在线的终端中已绑定此设备的终端
			if((online_num=select_fd_from_online_tel_sqlite3(db,fd_s,env->device_id))>0)
			{
				for (i = 0; i<online_num; i++) {
					//向每个在线用户发送环境数据
					do {
						ret = write (fd_s[i], buf, strlen (buf));
					} while (ret == -1 && EINTR == errno);
					if (ret == -1) {
						perror ("write error");
						exit (1);
					}
				}
			}
			bzero(buf,BUFSIZ);

			free(env);
			break;
		}
		case MSG_UPDATE_TIME:
		{
			PRINT ("环控终端更新时间...\n");	
			usleep(700000);
			strcpy (buf, pack_time_t (time(NULL)));
			do {
				ret = write (fd, buf, strlen (buf));
				PRINT ("%s\n", buf);
				bzero(buf,BUFSIZ);
			} while (ret == -1 && EINTR == errno);
			if (ret == -1) {
				perror ("write error");
				exit (1);
			}
			break;
		}
		case MSG_HEART_BEAT_DEV:
		{
			PRINT("收到心跳包\n");
			heart_beat_t *hb_t = NULL;
			if ((hb_t = (heart_beat_t*) malloc (sizeof (heart_beat_t ))) == NULL) {
				perror ("malloc error");
				exit (1);
			}
			strcpy (buf1, unpack_json (buf));
			bzero(buf,BUFSIZ);
			PRINT ("解包为json 数据类型%s\n", buf1);
			unpack_heart_beat_t (hb_t, buf1);
			bzero(buf1,BUFSIZ);

			if(hb_t->online==1)
			{
				pthread_cleanup_push(dev_heart_beat_unlock,NULL);
				pthread_mutex_lock(&dev_online_mutex);
				strncpy(dev_buffer_online_lock[dev_num_online_lock++],hb_t->tel,11);
				pthread_mutex_unlock(&dev_online_mutex);
				pthread_cleanup_pop(0);
			}

			free(hb_t);	
			break;
		}

		case MSG_HEART_BEAT_USR:
		{
			PRINT("收到心跳包\n");
			heart_beat_t *hb_t = NULL;
			if ((hb_t = (heart_beat_t*) malloc (sizeof (heart_beat_t ))) == NULL) {
				perror ("malloc error");
				exit (1);
			}
			strcpy (buf1, unpack_json (buf));
			bzero(buf,BUFSIZ);
			PRINT ("解包为json 数据类型%s\n", buf1);
			unpack_heart_beat_t (hb_t, buf1);
			bzero(buf1,BUFSIZ);

			if(hb_t->online==1)
			{
				pthread_cleanup_push(usr_heart_beat_unlock,NULL);
				pthread_mutex_lock(&usr_online_mutex);
				strncpy(usr_buffer_online_lock[usr_num_online_lock++],hb_t->tel,11);
				pthread_mutex_unlock(&usr_online_mutex);
				pthread_cleanup_pop(0);
			}

			free(hb_t);	
			break;
		}

	case MSG_BLIND_DEV:
		{
			PRINT("绑定设备\n");
			blind_dev_t *bd_t = NULL;
			if ((bd_t = (blind_dev_t*) malloc (sizeof (blind_dev_t))) == NULL) {
				perror ("malloc error");
				exit (1);
			}
			strcpy (buf1, unpack_json (buf));
			bzero(buf,BUFSIZ);
			PRINT ("解包为json 数据类型%s\n", buf1);
			unpack_blind_dev_t (bd_t, buf1);
			bzero(buf1,BUFSIZ);
			//绑定设备，将设备ID插入到数据库中tel下
			
			if(blind_dev_by_tel_sqlite3(db,*bd_t)>0){
				bd_t->flag=1;
			}else{
				bd_t->flag=0;	
			}
			strcpy (buf, pack_blind_dev_t (bd_t));
			do {
				ret = write (fd, buf, strlen (buf));
				PRINT ("%s\n", buf);
				bzero(buf,BUFSIZ);
			} while (ret == -1 && EINTR == errno);
			if (ret == -1) {
				perror ("write error");
				exit (1);
			}
			break;
		}
	case MSG_GET_DEV:
		{
			PRINT("获取设备列表\n");
			char dev_list[5][10];
			dev_list_t *dl_t = NULL;

			if ((dl_t = (dev_list_t*) malloc (sizeof (dev_list_t))) == NULL) {
				perror ("malloc error");
				exit (1);
			}

			strcpy (buf1, unpack_json (buf));
			bzero(buf,BUFSIZ);
			PRINT ("解包为json 数据类型%s\n", buf1);
			unpack_dev_list_t (dl_t, buf1);
			bzero(buf1,BUFSIZ);
			//绑定设备，将设备ID插入到数据库中tel下
			
			get_devlist_by_tel_sqlite3(db,*dl_t,&dev_list);
			if(dev_list!=NULL){
				send_dev_list_t(dev_list,fd,dl_t->tel);
			}
			free(dl_t);
			break;
		}
	}

	free(clip);
	return 0;
}
#endif


//将封过包的消息（头|消息类型|数据长度|数据|尾）的消息类型返回
int unpack_type (char *tmp)
{
	char char_head[6], char_tail[6];
	bzero (char_head, 6);
	bzero (char_tail, 6);

	int i, j;
	for (i = 0; i < 5; i++) {
		char_head[i] = *(tmp + i);
	}
	for (j = strlen (tmp) - 7, i = 0; j < strlen (tmp)-2; j++, i++) {
		char_tail[i] = *(tmp + j);
	}

	int n_head, n_tail;
	n_head = atoi (char_head);
	n_tail = atoi (char_tail);

	int head, tail;
	head = ntohs (n_head);
	tail = ntohs (n_tail);

  //    PRINT("head:%d tail:%d\n",head,tail);

	if (head != 0xfd)
		return 0;
	else if (tail != 0xfe)
		return 0;
	else {
//  PRINT("test!\n");
		int n_type;
		n_type = (unsigned int) (unsigned char)  (*(tmp + 5)) ;
		return n_type;
	}
}

//将封过包的消息（头|消息类型|数据长度|数据|尾）解包为json 类型
char *unpack_json (char *tmp)
{
	char char_head[6], char_tail[6];
	static char char_json[BUFSIZ];
	bzero (char_head, 6);
	bzero (char_tail, 6);
	bzero (char_json, BUFSIZ);

	int i, j;
	for (i = 0; i < 5; i++) {
		char_head[i] = *(tmp + i);
	}
	for (j = strlen (tmp) - 7, i = 0; j < strlen (tmp)-2; j++, i++) {
		char_tail[i] = *(tmp + j);
	}

	int n_head, n_tail;
	n_head = atoi (char_head);
	n_tail = atoi (char_tail);

	int head, tail;
	head = ntohs (n_head);
	tail = ntohs (n_tail);

//  PRINT("head:%d tail:%d\n",head,tail);

	if (head != 0xfd)
		return NULL;
	else if (tail != 0xfe)
		return NULL;
	else {
//  PRINT("test!\n");
		int n;
		n = (unsigned int) (unsigned char) (*(tmp + 6));
		PRINT ("%d\n", n);
		for (j = 7, i = 0; i < n; i++, j++) {
			char_json[i] = *(tmp + j);
		}
	}
//  PRINT("%s\n",char_json);
	return char_json;
}
//将json类型数据解包为environment_t类型结构体

void unpack_environment_t (environment_t * m, char *tmp)
{
	struct json_object *fat, *json_device_id, *json_date, *json_time, *json_temp, *json_hum, *json_pm2_5, *json_tvoc, *json_sound, *json_lx,
		*json_hcho;

	fat = json_tokener_parse (tmp);

	json_device_id = json_object_object_get (fat, "d_id");	//device_id
	json_date = json_object_object_get (fat, "date");	//date
	json_time = json_object_object_get (fat, "time");	//time
	json_temp = json_object_object_get (fat, "temp");	//temp
	json_hum = json_object_object_get (fat, "hum");	//hum
	json_pm2_5 = json_object_object_get (fat, "pm2_5");	//pm2_5
	json_tvoc = json_object_object_get (fat, "tvoc");	//tvoc
	json_sound = json_object_object_get (fat, "sound");	//sound
	json_lx = json_object_object_get (fat, "lx");	//lx
	json_hcho = json_object_object_get (fat, "hcho");	//hcho

	strncpy (m->device_id, json_object_get_string (json_device_id),9);
	strncpy (m->date, json_object_get_string (json_date),12);
	strncpy (m->time, json_object_get_string (json_time),6);
	m->temp = json_object_get_double (json_temp);
	m->hum = json_object_get_double (json_hum);
	m->pm2_5 = json_object_get_double (json_pm2_5);
	m->tvoc = json_object_get_double (json_tvoc);
	m->sound = json_object_get_double (json_sound);
	m->lx = json_object_get_double (json_lx);
	m->hcho = json_object_get_double (json_hcho);
	
	json_object_put(fat);
}
//将json类型数据解包为user_info_t类型结构体
void unpack_user_info_t (user_info_t * m, char *tmp)
{
	struct json_object *fat, *json_tel, *json_code, *json_r_l, *json_key;
	fat = json_tokener_parse (tmp);

	json_tel = json_object_object_get (fat, "tel");
	json_code = json_object_object_get (fat, "code");
	json_r_l = json_object_object_get (fat, "r_l");
	json_key = json_object_object_get (fat, "key");

	strncpy (m->tel, json_object_get_string (json_tel),12);
	m->code = json_object_get_int (json_code);
	m->r_l = json_object_get_int (json_r_l);
	m->key = json_object_get_int (json_key);
	
	json_object_put(fat);

}
//将json类型数据解包为heart_beat_t类型结构体
void unpack_heart_beat_t (heart_beat_t * m, char *tmp)
{
	struct json_object *fat, *json_tel, *json_key;
	fat = json_tokener_parse (tmp);

	json_tel = json_object_object_get (fat, "tel");
	json_key = json_object_object_get (fat, "online");

	strncpy (m->tel, json_object_get_string (json_tel),12);
	m->online = json_object_get_int (json_key);
	
	json_object_put(fat);
}
//将json类型数据解包为blind_dev_t类型结构体
void unpack_blind_dev_t (blind_dev_t * m, char *tmp)
{
	struct json_object *fat, *json_tel, *json_dev, *json_flag;
	fat = json_tokener_parse (tmp);

	json_tel = json_object_object_get (fat, "tel");
	json_dev = json_object_object_get (fat, "device_id");
	json_flag = json_object_object_get (fat, "flag");

	strncpy (m->tel, json_object_get_string (json_tel),12);
	strncpy(m->device_id , json_object_get_string (json_dev),11);
	m->flag= json_object_get_int (json_flag);
	
	json_object_put(fat);
}
//将json类型数据解包为dev_list_t类型结构体
void unpack_dev_list_t (dev_list_t * m, char *tmp)
{
	struct json_object *fat, *json_tel, *json_dev, *json_flag;
	fat = json_tokener_parse (tmp);

	json_tel = json_object_object_get (fat, "tel");
	json_dev = json_object_object_get (fat, "device_id");
	json_flag = json_object_object_get (fat, "flag");

	strncpy (m->tel, json_object_get_string (json_tel),12);
	strncpy(m->device_id , json_object_get_string (json_dev),11);
	m->flag= json_object_get_int (json_flag);
	
	json_object_put(fat);
}


//将environment_t类型结构体打包为json类型数据
char *pack_environment_t (environment_t * tmp){
	int head = 0xfd;
	int tail = 0xfe;
	int size;
	static char json_buf[BUFSIZ];

	struct json_object *fat, *json_device_id, *json_date, *json_time, *json_temp, *json_hum, *json_pm2_5, *json_tvoc, *json_sound, *json_lx,
		*json_hcho;

	fat = json_object_new_object ();

	json_device_id =json_object_new_string (tmp->device_id);
	json_date =json_object_new_string (tmp->date);
	json_time=json_object_new_string (tmp->time);
	json_temp = json_object_new_double  (tmp->temp);
	json_hum = json_object_new_double  (tmp->hum);
	json_pm2_5 = json_object_new_double  (tmp->pm2_5);
	json_tvoc = json_object_new_double  (tmp->tvoc);
	json_sound = json_object_new_double  (tmp->sound);
	json_lx = json_object_new_double  (tmp->lx);
	json_hcho = json_object_new_double  (tmp->hcho);

	json_object_object_add (fat, "d_id", json_device_id);	//device_id
	json_object_object_add (fat, "date", json_date);	//date
	json_object_object_add (fat, "time", json_time);	//time
	json_object_object_add (fat, "temp", json_temp);	//temp
	json_object_object_add (fat, "hum", json_hum);	//hum
	json_object_object_add (fat, "pm2_5", json_pm2_5);	//pm2_5
	json_object_object_add (fat, "tvoc", json_tvoc);	//tvoc
	json_object_object_add (fat, "sound", json_sound);	//sound
	json_object_object_add (fat, "lx", json_lx);	//lx
	json_object_object_add (fat, "hcho", json_hcho);	//hcho

	const char *json_string = json_object_to_json_string (fat);
	size = strlen (json_string);

	sprintf (json_buf, "%d%c%c%s%d\r\n", htons (head), (unsigned char)MSG_ENVIR_DEV,
			 (unsigned char) size, json_string, htons (tail));
//  PRINT("%d\n",strlen(json_buf));    
//  PRINT("%d\n",(unsigned char)129);
//  unsigned char xx = (unsigned char)129;
//  PRINT("%d\n",(unsigned int)xx);
	json_object_put(fat);
	return json_buf;
}
//将time类型结构体打包为json类型数据
char *pack_time_t (time_t tmp)
{
	int head = 0xfd;
	int tail = 0xfe;
	int size;
	char json_string[20];
	static char json_buf[BUFSIZ];
	sprintf(json_string,"%d",(unsigned int)tmp);

	size = strlen (json_string);

	sprintf (json_buf, "%d%c%c%s%d\r\n", htons (head), (unsigned char)MSG_UPDATE_TIME, (unsigned char) size, json_string, htons (tail));
//  PRINT("%d\n",strlen(json_buf));    
//  PRINT("%d\n",(unsigned char)129);
//  unsigned char xx = (unsigned char)129;
	return json_buf;

}

//将user_info_t类型结构体打包为json类型数据
char *pack_user_info_t (user_info_t * tmp)
{
	int head = 0xfd;
	int tail = 0xfe;
	int size;
	static char json_buf[BUFSIZ];

	struct json_object *fat, *json_tel, *json_code, *json_r_l, *json_key;


	fat = json_object_new_object ();

	json_tel = json_object_new_string (tmp->tel);
	json_code = json_object_new_int (tmp->code);
	json_r_l = json_object_new_int (tmp->r_l);
	json_key = json_object_new_int (tmp->key);

	json_object_object_add (fat, "tel", json_tel);
	json_object_object_add (fat, "code", json_code);
	json_object_object_add (fat, "r_l", json_r_l);
	json_object_object_add (fat, "key", json_key);

	const char *json_string = json_object_to_json_string (fat);
	size = strlen (json_string);

	sprintf (json_buf, "%d%c%c%s%d\r\n", htons (head), (unsigned char)MSG_REGEST_LOAD, (unsigned char) size, json_string, htons (tail));
//  PRINT("%d\n",strlen(json_buf));    
//  PRINT("%d\n",(unsigned char)129);
//  unsigned char xx = (unsigned char)129;
	json_object_put(fat);
	return json_buf;

}
//将heart_beat_t类型结构体打包为json类型数据
char *pack_heart_beat_t (heart_beat_t * tmp)
{
	int head = 0xfd;
	int tail = 0xfe;
	int size;
	static char json_buf[BUFSIZ];

	struct json_object *fat, *json_tel,*json_key;


	fat = json_object_new_object ();

	json_tel = json_object_new_string (tmp->tel);
	json_key = json_object_new_int (tmp->online);

	json_object_object_add (fat, "tel", json_tel);
	json_object_object_add (fat, "online", json_key);

	const char *json_string = json_object_to_json_string (fat);
	size = strlen (json_string);

	sprintf (json_buf, "%d%c%c%s%d\r\n", htons (head), (unsigned char)MSG_HEART_BEAT_USR, (unsigned char) size, json_string, htons (tail));
//  PRINT("%d\n",strlen(json_buf));    
//  PRINT("%d\n",(unsigned char)129);
//  unsigned char xx = (unsigned char)129;
	json_object_put(fat);
	return json_buf;

}
//将blind_dev_t类型结构体打包为json类型数据
char *pack_blind_dev_t (blind_dev_t * tmp)
{
	int head = 0xfd;
	int tail = 0xfe;
	int size;
	static char json_buf[BUFSIZ];

	struct json_object *fat, *json_tel,*json_dev,*json_flag;


	fat = json_object_new_object ();

	json_tel = json_object_new_string (tmp->tel);
	json_dev = json_object_new_string (tmp->device_id);
	json_flag = json_object_new_int (tmp->flag);

	json_object_object_add (fat, "tel", json_tel);
	json_object_object_add (fat, "device_id", json_dev);
	json_object_object_add (fat, "flag", json_flag);

	const char *json_string = json_object_to_json_string (fat);
	size = strlen (json_string);

	sprintf (json_buf, "%d%c%c%s%d\r\n", htons (head), (unsigned char)MSG_BLIND_DEV, (unsigned char) size, json_string, htons (tail));
//  PRINT("%d\n",strlen(json_buf));    
//  PRINT("%d\n",(unsigned char)129);
//  unsigned char xx = (unsigned char)129;
	json_object_put(fat);
	return json_buf;

}
//将dev_list_t类型结构体打包为json类型数据
struct json_object *pack_dev_list_t (dev_list_t *tmp)
{

	struct json_object *fat, *json_tel,*json_dev,*json_flag;


	fat = json_object_new_object ();

	json_tel = json_object_new_string (tmp->tel);
	json_dev = json_object_new_string (tmp->device_id);
	json_flag = json_object_new_int (tmp->flag);

	json_object_object_add (fat, "tel", json_tel);
	json_object_object_add (fat, "device_id", json_dev);
	json_object_object_add (fat, "flag", json_flag);

	return fat;

}
//将dev_list类型结构体打包为json类型数据
void send_dev_list_t (char dev_list[5][10],int fd,char *tel)
{
	int dev_num,ret=0;
	int head = 0xfd;
	int tail = 0xfe;
	int size;
	char buf[BUFSIZ];

	struct json_object *fat;
	dev_list_t *dl_t = NULL;


	if ((dl_t = (dev_list_t*) malloc (sizeof (dev_list_t))) == NULL) {
		perror ("malloc error");
		exit (1);
	}

	fat = json_object_new_array ();
	if(dev_list!=NULL){
		for(dev_num=0;dev_num<5;dev_num++){
			strncpy(dl_t->device_id,dev_list[dev_num],10);
			strncpy(dl_t->tel,tel,12);
			if(is_online_dev_sqlite3(db,dev_list[dev_num])>0){
				dl_t->flag=1;
			}else{
				dl_t->flag=0;	
			}
			json_object_array_add (fat, pack_dev_list_t (dl_t));
		}
		//PRINT("test %s\n",json_object_to_json_string (fat));
		const char *json_string = json_object_to_json_string (fat);
		size = strlen (json_string);

		sprintf (buf, "%d%c%c%s%d\r\n", htons (head), (unsigned char)MSG_GET_DEV, (unsigned char) size, json_string, htons (tail));
//  PRINT("%d\n",strlen(json_buf));    
//  PRINT("%d\n",(unsigned char)129);
//  unsigned char xx = (unsigned char)129;

		do {
			ret = write (fd, buf, strlen (buf));	
			PRINT ("%s\n", buf);
			bzero(buf,BUFSIZ);
		} while (ret == -1 && EINTR == errno);
		if (ret == -1) {
			perror ("write error");
			exit (1);
		}

		json_object_put(fat);
		free(dl_t);
	}else{
		json_object_put(fat);
		free(dl_t);
	}
}

void *heart_beat_dev(void *arg)
{
	memset(dev_buffer_online_lock,0,BUFSIZ*12);
	dev_num_online_lock=0;
	while(1)
	{
		
		//遍历dev_online数据库，向每一个在线设备发送心跳包	
		int s[10] = { 0 }, i,ret=0;
		char dev[BUFSIZ][12]={{0}};
		//memset(tel,0,BUFSIZ*12);
		char buf[BUFSIZ] = { 0 };
		heart_beat_t *hb = NULL;
		if ((hb = (heart_beat_t *) malloc (sizeof (heart_beat_t))) == NULL) {
			perror ("malloc error");
			exit (1);
		}
		if(select_dev_online_sqlite3 (db, s,&dev)>0)
		{
			PRINT ("遍历数据库查看在线设备\n");
			for (i = 0; s[i] != 0; i++) {
				//向每个在线设备发送心跳包
				strncpy(hb->tel,dev[i],11);	
				strcpy (buf, pack_heart_beat_t (hb));
				do {
					ret = write (s[i], buf, strlen (buf));
				//	PRINT ("%s\n", buf);

					bzero(buf,BUFSIZ);
				} while (ret == -1 && EINTR == errno);
				if (ret == -1) {
					perror ("write error");
					exit (1);
				}	

			}	
		}
		free(hb);
		//等待5s，接收心跳回复（建立一个BUFFER，如果收到心跳回复则将tel和fd放在buffer里面）
		sleep(5);
		//加线程锁
		pthread_cleanup_push(dev_heart_beat_unlock,NULL);
		pthread_mutex_lock(&dev_online_mutex);
		//遍历buffer，如果buffer里面没有dev_online上面的用户，则删除dev_online用户
		for (i = 0;strncmp(dev[i],"\0",1)!=0; i++) {
			int j=0;
			for(j=0;j<dev_num_online_lock;j++)
			{
				if(strncmp(dev[i],dev_buffer_online_lock[j],11)==0)
					break;
			}	
			if(j==(dev_num_online_lock))
			{

				PRINT("%d删除设备%s\n",i,dev[i]);
				//删除tel相应的数据
				del_dev_online_by_dev_sqlite3(db,dev[i]);

			}	
		}
			memset(dev_buffer_online_lock,0,BUFSIZ*12);
			dev_num_online_lock=0;
			//解线程锁
			pthread_mutex_unlock(&dev_online_mutex);
			pthread_cleanup_pop(0);
	}
	return NULL;
}

void *heart_beat_usr(void *arg)
{
	memset(usr_buffer_online_lock,0,BUFSIZ*12);
	usr_num_online_lock=0;
	while(1)
	{
		
		//遍历usr_online数据库，向每一个在线用户发送心跳包	
		int s[10] = { 0 }, i,ret=0;
		char tel[BUFSIZ][12]={{0}};
		//memset(tel,0,BUFSIZ*12);
		char buf[BUFSIZ] = { 0 };
		heart_beat_t *hb = NULL;
		if ((hb = (heart_beat_t *) malloc (sizeof (heart_beat_t))) == NULL) {
			perror ("malloc error");
			exit (1);
		}
		if(select_usr_online_sqlite3 (db, s,&tel)>0)
		{
			//PRINT ("遍历数据库查看在线用户\n");
			for (i = 0; s[i] != 0; i++) {
				//向每个在线用户发送心跳包
				strncpy(hb->tel,tel[i],11);	
				strcpy (buf, pack_heart_beat_t (hb));
				do {
					ret = write (s[i], buf, strlen (buf));
				//	PRINT ("%s\n", buf);

					bzero(buf,BUFSIZ);
				} while (ret == -1 && EINTR == errno);
				if (ret == -1) {
					perror ("write error");
					exit (1);
				}	

			}	
		}
		free(hb);
		//等待5s，接收心跳回复（建立一个BUFFER，如果收到心跳回复则将tel和fd放在buffer里面）
		sleep(10);
		//加线程锁
		pthread_cleanup_push(usr_heart_beat_unlock,NULL);
		pthread_mutex_lock(&usr_online_mutex);
		//遍历buffer，如果buffer里面没有usr_online上面的用户，则删除usr_online用户
		for (i = 0;strncmp(tel[i],"\0",1)!=0; i++) {
			int j=0;
			for(j=0;j<usr_num_online_lock;j++)
			{
				if(strncmp(tel[i],usr_buffer_online_lock[j],11)==0)
					break;
			}	
			if(j==(usr_num_online_lock))
			{

				PRINT("%d删除用户%s\n",i,tel[i]);
				//删除tel相应的数据
				del_usr_online_by_tel_sqlite3(db,tel[i]);

			}	
		}
			memset(usr_buffer_online_lock,0,BUFSIZ*12);
			usr_num_online_lock=0;
			//解线程锁
			pthread_mutex_unlock(&usr_online_mutex);
			pthread_cleanup_pop(0);
	}
	return NULL;
}
//程序退出时要做的一些处理
void quit(void)
{
	del_allusr_sqlite3(db);
	del_allusr_online_sqlite3(db);
	del_alldev_online_sqlite3(db);
	pthread_mutex_destroy(&dev_online_mutex);
	pthread_mutex_destroy(&usr_online_mutex);
}
//线程退出时要做的事情
void usr_heart_beat_unlock(void *arg)
{
	pthread_mutex_unlock(&usr_online_mutex);
}
void dev_heart_beat_unlock(void *arg)
{
	pthread_mutex_unlock(&dev_online_mutex);
}
int main ()
{
	int sock_fd = -1, newfd = -1;
	usrIP_info_t ser;
	pthread_mutex_init(&usr_online_mutex,NULL);
	pthread_mutex_init(&dev_online_mutex,NULL);
	if ((sock_fd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
		perror ("socket error");
		exit (1);
	}
	int b_reuse = 1;
	if (setsockopt (sock_fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof (int)) == -1) {
		perror ("setgetopt error");
		exit (1);
	}
	ser.fd = sock_fd;
	ser.usr.sin_family = AF_INET;
	ser.usr.sin_port = htons (SER_PORT);
	ser.usr.sin_addr.s_addr = INADDR_ANY;
	if (bind (sock_fd, (struct sockaddr *) &ser.usr, sizeof (ser.usr)) == -1) {
		perror ("bind error");
		exit (1);
	}

	if (listen (sock_fd, 5) == -1) {
		perror ("listen error");
		exit (1);
	}

	PRINT ("Server starting...OK!\n");

	int maxfd;
	fd_set rset;
	if (sqlite3_open ("test.db", &db) != SQLITE_OK) {
		PRINT ("sqlite_open error!\n");
		exit (1);
	}
	create_usr_sqlite3 (db);
	create_usr_online_sqlite3 (db);
	create_dev_online_sqlite3 (db);
	create_user_info_sqlite3(db);//
	del_allusr_sqlite3(db);
	del_allusr_online_sqlite3(db);
	del_alldev_online_sqlite3(db);
	//insert_usr_sqlite3 (db, ser);

	pthread_t p_id_usr;
	if(pthread_create(&p_id_usr,NULL,(void *)heart_beat_usr,NULL)!=0)
	{
		perror("pthread_create error");
		exit(1);
	}
	pthread_t p_id_dev;
	if(pthread_create(&p_id_dev,NULL,(void *)heart_beat_dev,NULL)!=0)
	{
		perror("pthread_create error");
		exit(1);
	}
	atexit(quit);

	while (1) {
		FD_ZERO (&rset);
		FD_SET (sock_fd, &rset);
		maxfd = sock_fd;
		/*遍历数据库，将已经建立好连接的客户端的文件描述符加到集合中 */
		int s[10] = { 0 }, i;

		PRINT ("遍历数据库将打开连接添加到select监控集合中\n");
		select_usr_sqlite3 (db, s);
		for (i = 0; s[i] != 0; i++) {
			FD_SET (s[i], &rset);
			if (maxfd < s[i])
				maxfd = s[i];
		}
		//PRINT("hello\n");
		select (maxfd + 1, &rset, NULL, NULL, NULL);
		if (FD_ISSET (sock_fd, &rset)) {
			PRINT ("有连接\n");
			usrIP_info_t cli;
			socklen_t cli_len = sizeof (cli.usr);
			if ((newfd = accept (sock_fd, (struct sockaddr *) &cli.usr, &cli_len)) == -1) {
				perror ("accept error");
				exit (1);
			}
			cli.fd = newfd;
			insert_usr_sqlite3 (db, cli);
		}
		/*遍历数据库，查看每一个client fd上是否有数据并处理 */
		PRINT("遍历数据库查看select监控的连接是否有数据\n");
		select_usr_sqlite3 (db, s);
		int ret_t = -1;
		for (i = 0; s[i] != 0; i++) {
			if (FD_ISSET (s[i], &rset)) {
				PRINT ("fd=%d有数据\n", s[i]);
				ret_t = cli_date_handler (s[i]);
				if (ret_t == 5)
					del_usr_sqlite3 (db, s[i]);

			}
		}
	}

}
