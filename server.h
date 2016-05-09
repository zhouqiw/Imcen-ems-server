#ifndef __SERVER_H__
#define __SERVER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sqlite3.h>
#include "/usr/local/include/json/json.h"

#define SQL_SIZE    128
#define SER_PORT  50000 
#define DEBUG 1
#if DEBUG 
	#define PRINT(fmt,args...) printf(fmt,##args)
#else
	#define PRINT(fmt,args...)  
#endif


sqlite3 *db = NULL;
char sql[SQL_SIZE] = { '\0' };
time_t t;
char *errmsg;

pthread_mutex_t dev_online_mutex;//保护buffer_online_lock和num_online_lock的线程锁
char dev_buffer_online_lock[BUFSIZ][12];
int dev_num_online_lock;
pthread_mutex_t usr_online_mutex;//保护buffer_online_lock和num_online_lock的线程锁
char usr_buffer_online_lock[BUFSIZ][12];
int usr_num_online_lock;
/*  环境参数的结构体      用于保存终端所监测到的环境参数  */
typedef struct environment
{
	char device_id[9];  // 设备ID
	char date[12];
	char time[6];
	float temp;
	float hum;
	float pm2_5;
	float tvoc;
	float sound;
	float lx;
	float hcho;
}environment_t;

/* 查询环境参数的数据结构  用于手机向服务器查询环境参数 */
typedef struct querybyday_environment//以日为单位查询
{
	char device_id[9];  // 设备ID
	char start_date[12];
	char end_date[12];

} querybyday_environment_t;
/* 查询环境参数的数据结构  用于手机向服务器查询环境参数 */
typedef struct querybyhour_environment//以时为单位查询
{
	char device_id[9];  // 设备ID
	char start_time[3];
	char end_time[3];

} querybyhour_environment_t;
/*设备注册登录信息结构体*/
typedef struct user_information {
	char tel[12];   // 11位
	int code;       // 密码，必须数字
	int r_l;        //0注册或者1登陆
	int key;        //0代表NO，1代表YES
}user_info_t;
/* 消息类型  */
typedef enum msg_type
{
	MSG_NONE,
	MSG_ENVIR_DEV, //终端发送的环境参数
	MSG_QUERYBYDAY_ENVIR_DEV,//手机向服务器以日为单位查询环境参数
	MSG_QUERYBYHOUR_ENVIR_DEV,//手机向服务器以时为单位查询环境参数
	MSG_REGEST_LOAD, //登录注册信息
	MSG_UPDATE_TIME, //更新时间
	MSG_HEART_BEAT_USR,//心跳包
	MSG_HEART_BEAT_DEV,//心跳包
	MSG_BLIND_DEV,//绑定设备
	MSG_UNBLIND_DEV,//去绑定设备
	MSG_GET_DEV,//获取设备列表
}msg_type_t;

/* 用户信息结构体  */
typedef struct usrIP_info
{
	int fd;
	struct sockaddr_in usr;
} usrIP_info_t;
/* 心跳包结构体  */
typedef struct heart_beat_package 
{
	char tel[12];
	int online;
} heart_beat_t;
/*绑定设备结构体  */
typedef struct blind_device 
{
	char tel[12];
	char device_id[10];
	int flag;
} blind_dev_t;

/*获取设备列表结构体  */
typedef struct device_list 
{
	char tel[12];
	char device_id[10];
	int flag;
} dev_list_t;



extern void init_usr_sqlite3 (sqlite3 * db);	// 初始化table为usr数据库 
extern void del_usr_sqlite3 (sqlite3 * db ,int fd);	//删除usr中fd数据库 
extern void del_allusr_sqlite3 (sqlite3 * db );	//删除所有usr中fd数据库 
extern void del_alldev_sqlite3 (sqlite3 * db );	//删除所有dev中fd数据库 
extern void del_usr_online_by_tel_sqlite3 (sqlite3 * db ,char * tel);	//删除usr_online中tel数据库 
extern void del_dev_online_by_dev_sqlite3 (sqlite3 * db ,char * tel);	//删除dev_online中dev数据库 
extern void del_usr_online_by_fd_sqlite3 (sqlite3 * db ,int fd);	//删除usr_online中fd数据库 
extern void del_dev_online_by_fd_sqlite3 (sqlite3 * db ,int fd);	//删除dev_online中fd数据库 
extern void del_allusr_online_sqlite3 (sqlite3 * db);	//删除all usr_online数据库 
extern void del_alldev_online_sqlite3 (sqlite3 * db);	//删除all dev_online数据库 
extern void insert_usr_sqlite3 (sqlite3 * db, usrIP_info_t cli);//向table为usr的数据库中插入数据
extern int blind_dev_by_tel_sqlite3 (sqlite3 * db, blind_dev_t info);//向table为user_online的数据库中更新数据
extern void insert_usr_online_sqlite3 (sqlite3 * db, usrIP_info_t cli,user_info_t user);//向table为usr_online的数据库中插入数据
extern void insert_dev_online_sqlite3 (sqlite3 * db, usrIP_info_t cli,environment_t user);//向table为dev_online的数据库中插入数据
extern void insert_user_sqlite3 (sqlite3 * db, user_info_t info);//向table为user_info的数据库中插入数据
extern void create_usr_sqlite3 (sqlite3 * db);//创建table为usr的数据库
extern void create_usr_online_sqlite3 (sqlite3 * db);//创建table为usr_online的数据库
extern void create_dev_online_sqlite3 (sqlite3 * db);//创建table为dev_online的数据库
extern void create_user_info_sqlite3 (sqlite3 * db);//创建table为user_info的数据库专用于存放用户注册登录信息
extern void create_dev_sqlite3 (sqlite3 * db, environment_t env);//创建table为device_id的数据库
extern usrIP_info_t *select_usr1_sqlite3 (sqlite3 * db, usrIP_info_t * cli, int fd);//在table为usr的数据库中查询fd的用户并返回用户信息结构体 
extern int *select_usr_sqlite3 (sqlite3 * db, int *s);//在table为usr的数据库中获取所有的fd
extern int select_usr_online_sqlite3 (sqlite3 * db, int *s,char (*hb)[BUFSIZ][12]) ;//在table为usr_online的数据库中获取所有的fd
extern int select_dev_online_sqlite3 (sqlite3 * db, int *s,char (*hb)[BUFSIZ][12]) ;//在table为dev_online的数据库中获取所有的fd
extern int select_fd_from_online_tel_sqlite3 (sqlite3 * db, int *s,char *d_id);//在table为usr的数据库中获取所有的fd

extern int select_dev_reg_sqlite3 (sqlite3 * db, user_info_t *info);//查询账户是否存在
extern int select1_dev_online_sqlite3 (sqlite3 * db, environment_t info);//查询在线设备是否存在
extern int select_device_code_sqlite3 (sqlite3 * db, user_info_t* info);//在table为device_id的数据库中查询密码
extern void get_devlist_by_tel_sqlite3 (sqlite3 * db, dev_list_t info,char (*dev)[5][10]);//向table为user_info的数据库中查询数据获取设备列表
extern int is_online_dev_sqlite3 (sqlite3 * db, char * info);//查询在线设备是否存在
extern void send_dev_list_t (char dev_list[5][10],int fd,char *tel);//将dev_list类型结构体打包为json类型数据

extern int cli_date_handler (int fd);	//创建table为box_id的数据库
extern int unpack_type (char *tmp);	//将封过包的消息（头|消息类型|数据长度|数据|尾）的消息类型返回
extern char *unpack_json (char *tmp);	//将封过包的消息（头|消息类型|数据长度|数据|尾）解包为json 类型
extern void unpack_environment_t (environment_t * m, char *tmp);//将json类型数据解包为environment_t类型结构体
extern void unpack_user_info_t (user_info_t * m, char *tmp);//将json类型数据解包为user_info_t类型结构体
extern void unpack_heart_beat_t (heart_beat_t * m, char *tmp);//将json类型数据解包为heart_beat_t类型结构体
extern void unpack_blind_dev_t (blind_dev_t * m, char *tmp);//将json类型数据解包为blind_dev_t类型结构体
extern void unpack_dev_list_t (dev_list_t * m, char *tmp);//将json类型数据解包为dev_list_t类型结构体
extern char *pack_environment_t (environment_t * tmp);//将environment_t类型结构体打包为json类型数据
extern char *pack_user_info_t (user_info_t * tmp);//将user_info_t类型结构体打包为json类型数据
extern char *pack_time_t (time_t tmp);//将time类型结构体打包为json类型数据
extern char *pack_blind_dev_t (blind_dev_t * tmp);//将blind_dev_t类型结构体打包为json类型数据
extern struct json_object *pack_dev_list_t (dev_list_t * tmp);//将dev_list_t类型结构体打包为json类型数据


extern void *heart_beat_usr(void *arg);//定时发送心跳包线程
extern void *heart_beat_dev(void *arg);//定时发送心跳包线程
extern void quit(void);//程序退出时要做的一些处理
extern void usr_heart_beat_unlock(void *arg);//线程退出时要做的事情
extern void dev_heart_beat_unlock(void *arg);//线程退出时要做的事情

#endif
