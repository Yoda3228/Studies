/* Processed by ecpg (4.9.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "lab5.ec"
#include <stdio.h>
#include <stdlib.h>
#include <sqlca.h>

int query1() {
	printf("Query1:\n");
	//
	/* exec sql begin declare section */
	 
	 
	
#line 9 "lab5.ec"
 double total_sum ;
 
#line 10 "lab5.ec"
 int total_sum_ind ;
/* exec sql end declare section */
#line 11 "lab5.ec"

	//
	{ ECPGtrans(__LINE__, NULL, "begin work");}
#line 13 "lab5.ec"

	printf("Executing query1...\n");
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_execute, "t_sum", ECPGt_EOIT, 
	ECPGt_double,&(total_sum),(long)1,(long)1,sizeof(double), 
	ECPGt_int,&(total_sum_ind),(long)1,(long)1,sizeof(int), ECPGt_EORT);}
#line 15 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Execute", sqlca.sqlcode);
		{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 18 "lab5.ec"

		return 1;
	}
	//
	{ ECPGtrans(__LINE__, NULL, "commit work");}
#line 22 "lab5.ec"

	if(total_sum_ind < 0) 
		printf("No result\n");
	else printf("Total sum = %f\n", total_sum);
	return 0;
}

int query2() {
	//
	/* exec sql begin declare section */
	 
	   
	  
	
#line 32 "lab5.ec"
 char town [ 20 ] ;
 
#line 33 "lab5.ec"
 char n_post [ 6 ] , name [ 20 ] , n_det [ 6 ] ;
 
#line 34 "lab5.ec"
 float ves , avg_ves ;
/* exec sql end declare section */
#line 35 "lab5.ec"

	//
	printf("Query2:\nSelect post town: ");
	scanf("%s", town);
	//
	printf("Declaring cursor for query2...\n");
	/* declare query2_cursor cursor for $1 */
#line 41 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Declare", sqlca.sqlcode);
		return 1;
	}
	//
	printf("Opening cursor for query2...\n");
	{ ECPGtrans(__LINE__, NULL, "begin work");}
#line 48 "lab5.ec"

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare query2_cursor cursor for $1", 
	ECPGt_char_variable,(ECPGprepared_statement(NULL, "query2_res", __LINE__)),(long)1,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(town),(long)20,(long)1,(20)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 49 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Open cursor", sqlca.sqlcode);
		{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 52 "lab5.ec"

		return 1;
	}
	//
	printf("Fetching cursor...\n");
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch next from query2_cursor", ECPGt_EOIT, 
	ECPGt_char,(n_post),(long)6,(long)1,(6)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(name),(long)20,(long)1,(20)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(n_det),(long)6,(long)1,(6)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(ves),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(avg_ves),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 57 "lab5.ec"

	if (sqlca.sqlcode == 100) {
		printf("No results for request\n");
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query2_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 60 "lab5.ec"

		{ ECPGtrans(__LINE__, NULL, "commit work");}
#line 61 "lab5.ec"

		return 0;
	}
	else if (sqlca.sqlcode < 0) {
		errorHandler("Fetch", sqlca.sqlcode);
		{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 66 "lab5.ec"

		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query2_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 67 "lab5.ec"

		return 1;
	}
	//
	printf("n_post\tname\t\tn_det\tves\t\tavg_ves\n");
	do {
		printf("%s\t", n_post);
		printf("%s\t", name);
		printf("%s\t", n_det);
		printf("%f\t", ves);
		printf("%f\n", avg_ves);
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch next from query2_cursor", ECPGt_EOIT, 
	ECPGt_char,(n_post),(long)6,(long)1,(6)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(name),(long)20,(long)1,(20)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(n_det),(long)6,(long)1,(6)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(ves),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(avg_ves),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 78 "lab5.ec"

		if (sqlca.sqlcode < 0) {
			errorHandler("Fetch", sqlca.sqlcode);
			{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 81 "lab5.ec"

			{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query2_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 82 "lab5.ec"

			return 1;
		}
	} while (sqlca.sqlcode != 100);
	//
	printf("Closing cursor for query2...\n");
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query2_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 88 "lab5.ec"

	if (sqlca.sqlcode != 0) {
		errorHandler("Close cursor", sqlca.sqlcode);
		return 1;
	}
	{ ECPGtrans(__LINE__, NULL, "commit work");}
#line 93 "lab5.ec"

	printf("Cursor closed\n");
	return 0;
}


int query3() {
	//
	/* exec sql begin declare section */
	 
	 
	  
	 
	
#line 102 "lab5.ec"
 char n_post [ 6 ] ;
 
#line 103 "lab5.ec"
 char town [ 20 ] ;
 
#line 104 "lab5.ec"
 int value , total_v ;
 
#line 105 "lab5.ec"
 float procent ;
/* exec sql end declare section */
#line 106 "lab5.ec"

	//
	printf("Query3:\nSelect post number(S1-S5): ");
	scanf("%s", n_post);
	//
	printf("Declaring cursor for query3...\n");
	/* declare query3_cursor cursor for $1 */
#line 112 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Declare", sqlca.sqlcode);
		return 1;
	}
	//
	printf("Opening cursor for query3...\n");
	{ ECPGtrans(__LINE__, NULL, "begin work");}
#line 119 "lab5.ec"

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare query3_cursor cursor for $1", 
	ECPGt_char_variable,(ECPGprepared_statement(NULL, "query3_res", __LINE__)),(long)1,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(n_post),(long)6,(long)1,(6)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(n_post),(long)6,(long)1,(6)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 120 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Open cursor", sqlca.sqlcode);
		{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 123 "lab5.ec"

		return 1;
	}
	//
	printf("Fetching...\n");
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch next from query3_cursor", ECPGt_EOIT, 
	ECPGt_char,(town),(long)20,(long)1,(20)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(value),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(total_v),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(procent),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 128 "lab5.ec"

	if (sqlca.sqlcode == 100) {
		printf("No results for query3\n");
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query3_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 131 "lab5.ec"

		{ ECPGtrans(__LINE__, NULL, "commit work");}
#line 132 "lab5.ec"

		return 0;
	} else if (sqlca.sqlcode < 0) {
		errorHandler("Fetch", sqlca.sqlcode);
		{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 136 "lab5.ec"

		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query3_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 137 "lab5.ec"

		return 1;
	}
	//
	printf("town\t\tvalue\ttotal_v\tprocent\n");
	do {
		printf("%s\t%d\t%d\t%f\n",town, value, total_v, procent);
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch next from query3_cursor", ECPGt_EOIT, 
	ECPGt_char,(town),(long)20,(long)1,(20)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(value),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(total_v),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(procent),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 144 "lab5.ec"

		if (sqlca.sqlcode < 0) {
			errorHandler("Fetch", sqlca.sqlcode);
			{ ECPGtrans(__LINE__, NULL, "rollback work");}
#line 147 "lab5.ec"

			{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query3_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 148 "lab5.ec"

			return 1;
		}
	} while (sqlca.sqlcode != 100);
	//
	printf("Closing cursor for query3...\n");
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close query3_cursor", ECPGt_EOIT, ECPGt_EORT);}
#line 154 "lab5.ec"

	if (sqlca.sqlcode != 0) {
		errorHandler("Close cursor", sqlca.sqlcode);
		return 1;
	}
	{ ECPGtrans(__LINE__, NULL, "commit work");}
#line 159 "lab5.ec"

	printf("Cursor closed\n");
	return 0;
}

void errorHandler(const char* st_name, int errnum) {
	printf("Error code %d at %s \n", errnum, st_name);
	printf("Error message: %s\n", sqlca.sqlerrm.sqlerrmc);
	return;
}

void main() {
	//
	/* exec sql begin declare section */
	 
	 
	 
	//для первого запроса
	 
	   
	
	//для второго запроса
	 
	   
	  
	   

	//для третьего запроса
	//char n_post[6];
	//char town[20];
	  
	 
	   

	
#line 173 "lab5.ec"
 char login [ 10 ] ;
 
#line 174 "lab5.ec"
 char pass [ 10 ] ;
 
#line 175 "lab5.ec"
 char scheme [ 10 ] ;
 
#line 177 "lab5.ec"
 double total_sum ;
 
#line 178 "lab5.ec"
 char text_query1 [] = "select round(avg(b.sves),2)sr from (select t.n_post,max(t.kol*p.ves) sves from spj t join p on p.n_det=t.n_det group by t.n_post order by 1)b" ;
 
#line 181 "lab5.ec"
 char town [ 20 ] ;
 
#line 182 "lab5.ec"
 char n_post [ 6 ] , name [ 20 ] , n_det [ 6 ] ;
 
#line 183 "lab5.ec"
 float ves , avg_ves ;
 
#line 184 "lab5.ec"
 char text_query2 [] = "select a.n_post,s.name,a.n_det,p.ves,a.mves from (select t.n_post, t.n_det, max(t.kol*p.ves) mves from spj t join s s1 on t.n_post=s1.n_post join p on t.n_det=p.n_det where s1.town=? group by t.n_post, t.n_det)a join p on a.n_det=p.n_det join s on a.n_post=s.n_post order by a.n_post,a.n_det " ;
 
#line 189 "lab5.ec"
 int value , total_v ;
 
#line 190 "lab5.ec"
 float procent ;
 
#line 191 "lab5.ec"
 char text_query3 [] = "select a.town, a.value, b.total_v, round(a.value::numeric * 100 / b.total_v,2) as procent from (select p.town, count(*) as value from spj join p on p.n_det=spj.n_det where spj.n_post = ? group by 1) a cross join (select count(*) as total_v from spj where spj.n_post= ?)b order by 1" ;
/* exec sql end declare section */
#line 193 "lab5.ec"

	//
	printf("Enter login:\n");
	scanf("%s", &login);
	printf("Enter password:\n");
	scanf("%s", &pass);
	printf("Trying to connect...\n");
	{ ECPGconnect(__LINE__, 0, "students@fpm2.ami.nstu.ru" , login , pass , NULL, 0); }
#line 200 "lab5.ec"

	//
	if (sqlca.sqlcode != 0) {
		errorHandler("Connect to database", sqlca.sqlcode);
		return;
	}
	else printf("Connected\n");
	//
	printf("Enter scheme:\n");
	scanf("%s", &scheme);
	//
	printf("Setting scheme...\n");
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set search_path to $0", 
	ECPGt_char,(scheme),(long)10,(long)1,(10)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 212 "lab5.ec"

	if (sqlca.sqlcode != 0) {
		errorHandler("Set database", sqlca.sqlcode);
		return;
	}
	else printf("Done\n");
	//
	int n, flag = 0;
	bool query1_ready = true, query2_ready = true, query3_ready = true;
	//готовим первый запрос
	printf("Preparing query1...\n");
	{ ECPGprepare(__LINE__, NULL, 0, "t_sum", text_query1);}
#line 223 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Prepare", sqlca.sqlcode);
		query1_ready = false;
	} else printf("Done\n");	
	//
	//готовим второй запрос
	printf("Preparing query2...\n");
	{ ECPGprepare(__LINE__, NULL, 0, "query2_res", text_query2);}
#line 231 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Prepare", sqlca.sqlcode);
		query2_ready = false;
	} else printf("Done\n");
	//
	//готовим третий запрос
	printf("Preparing query3...\n");
	{ ECPGprepare(__LINE__, NULL, 0, "query3_res", text_query3);}
#line 239 "lab5.ec"

	if (sqlca.sqlcode < 0) {
		errorHandler("Prepare", sqlca.sqlcode);
		query3_ready = false;
	} else printf("Done\n");
	//
	do {
		printf("Chose 1 - 3 for queries or 4 for exit:");
		scanf("%d", &n);
		switch (n) {
		case 1:
			if(query1_ready) {
				query1();
			}
			else {
				printf("Can't execute query1, error occurred on prepare step\n");
			}
			break;
		case 2:
			if(query2_ready) {
				query2();
			}
			else {
				printf("Can't execute query2, error occurred on prepare step\n");
			}
			break;
		case 3:
			if(query3_ready) {
				query3();
			}
			else {
				printf("Can't execute query3, error occurred on prepare step\n");
			}
			break;
		case 4:
			flag = 1;
			{ ECPGdisconnect(__LINE__, "ALL");}
#line 275 "lab5.ec"

			printf("Session closed\n");
			break;
		default:
			printf("Chose 1-4 numbers!\n");
			break;
		}
	} while (flag == 0);
	return;
}
