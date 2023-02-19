#include <stdio.h>
#include <stdlib.h>
#include <sqlca.h>

int query1() {
	printf("Query1:\n");
	//
	EXEC SQL begin declare section;
	double total_sum;
	int total_sum_ind;
	EXEC SQL end declare section;
	//
	EXEC SQL begin work;
	printf("Executing query1...\n");
	EXEC SQL execute t_sum into :total_sum:total_sum_ind;
	if (sqlca.sqlcode < 0) {
		errorHandler("Execute", sqlca.sqlcode);
		EXEC SQL rollback work;
		return 1;
	}
	//
	EXEC SQL commit work;
	if(total_sum_ind < 0) 
		printf("No result\n");
	else printf("Total sum = %f\n", total_sum);
	return 0;
}

int query2() {
	//
	EXEC SQL begin declare section;
	char town[20];
	char n_post[6], name[20], n_det[6];
	float ves, avg_ves;
	EXEC SQL end declare section;
	//
	printf("Query2:\nSelect post town: ");
	scanf("%s", town);
	//
	printf("Declaring cursor for query2...\n");
	EXEC SQL declare query2_cursor cursor for query2_res;
	if (sqlca.sqlcode < 0) {
		errorHandler("Declare", sqlca.sqlcode);
		return 1;
	}
	//
	printf("Opening cursor for query2...\n");
	EXEC SQL begin work;
	EXEC SQL open query2_cursor using :town;
	if (sqlca.sqlcode < 0) {
		errorHandler("Open cursor", sqlca.sqlcode);
		EXEC SQL rollback work;
		return 1;
	}
	//
	printf("Fetching cursor...\n");
	EXEC SQL fetch next from query2_cursor into :n_post, :name, :n_det, :ves, :avg_ves;
	if (sqlca.sqlcode == 100) {
		printf("No results for request\n");
		EXEC SQL close query2_cursor;
		EXEC SQL commit work;
		return 0;
	}
	else if (sqlca.sqlcode < 0) {
		errorHandler("Fetch", sqlca.sqlcode);
		EXEC SQL rollback work;
		EXEC SQL close query2_cursor;
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
		EXEC SQL fetch next from query2_cursor into :n_post, :name, :n_det, :ves, :avg_ves;
		if (sqlca.sqlcode < 0) {
			errorHandler("Fetch", sqlca.sqlcode);
			EXEC SQL rollback work;
			EXEC SQL close query2_cursor;
			return 1;
		}
	} while (sqlca.sqlcode != 100);
	//
	printf("Closing cursor for query2...\n");
	EXEC SQL close query2_cursor;
	if (sqlca.sqlcode != 0) {
		errorHandler("Close cursor", sqlca.sqlcode);
		return 1;
	}
	EXEC SQL commit work;
	printf("Cursor closed\n");
	return 0;
}


int query3() {
	//
	EXEC SQL begin declare section;
	char n_post[6];
	char town[20];
	int value, total_v;
	float procent;
	EXEC SQL end declare section;
	//
	printf("Query3:\nSelect post number(S1-S5): ");
	scanf("%s", n_post);
	//
	printf("Declaring cursor for query3...\n");
	EXEC SQL declare query3_cursor cursor for query3_res;
	if (sqlca.sqlcode < 0) {
		errorHandler("Declare", sqlca.sqlcode);
		return 1;
	}
	//
	printf("Opening cursor for query3...\n");
	EXEC SQL begin work;
	EXEC SQL open query3_cursor using :n_post, :n_post;
	if (sqlca.sqlcode < 0) {
		errorHandler("Open cursor", sqlca.sqlcode);
		EXEC SQL rollback work;
		return 1;
	}
	//
	printf("Fetching...\n");
	EXEC SQL fetch next from query3_cursor into :town, :value, :total_v, :procent;
	if (sqlca.sqlcode == 100) {
		printf("No results for query3\n");
		EXEC SQL close query3_cursor;
		EXEC SQL commit work;
		return 0;
	} else if (sqlca.sqlcode < 0) {
		errorHandler("Fetch", sqlca.sqlcode);
		EXEC SQL rollback work;
		EXEC SQL close query3_cursor;
		return 1;
	}
	//
	printf("town\t\tvalue\ttotal_v\tprocent\n");
	do {
		printf("%s\t%d\t%d\t%f\n",town, value, total_v, procent);
		EXEC SQL fetch next from query3_cursor into :town, :value, :total_v, :procent;
		if (sqlca.sqlcode < 0) {
			errorHandler("Fetch", sqlca.sqlcode);
			EXEC SQL rollback work;
			EXEC SQL close query3_cursor;
			return 1;
		}
	} while (sqlca.sqlcode != 100);
	//
	printf("Closing cursor for query3...\n");
	EXEC SQL close query3_cursor;
	if (sqlca.sqlcode != 0) {
		errorHandler("Close cursor", sqlca.sqlcode);
		return 1;
	}
	EXEC SQL commit work;
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
	EXEC SQL begin declare section;
	char login[10];
	char pass[10];
	char scheme[10];
	//для первого запроса
	double total_sum;
	char text_query1[] = "select round(avg(b.sves),2)sr from (select t.n_post,max(t.kol*p.ves) sves from spj t join p on p.n_det=t.n_det group by t.n_post order by 1)b";
	
	//для второго запроса
	char town[20];
	char n_post[6], name[20], n_det[6];
	float ves, avg_ves;
	char text_query2[] = "select a.n_post,s.name,a.n_det,p.ves,a.mves from (select t.n_post, t.n_det, max(t.kol*p.ves) mves from spj t join s s1 on t.n_post=s1.n_post join p on t.n_det=p.n_det where s1.town=? group by t.n_post, t.n_det)a join p on a.n_det=p.n_det join s on a.n_post=s.n_post order by a.n_post,a.n_det ";

	//для третьего запроса
	//char n_post[6];
	//char town[20];
	int value, total_v;
	float procent;
	char text_query3[] = "select a.town, a.value, b.total_v, round(a.value::numeric * 100 / b.total_v,2) as procent from (select p.town, count(*) as value from spj join p on p.n_det=spj.n_det where spj.n_post = ? group by 1) a cross join (select count(*) as total_v from spj where spj.n_post= ?)b order by 1";

	EXEC SQL end declare section;
	//
	printf("Enter login:\n");
	scanf("%s", &login);
	printf("Enter password:\n");
	scanf("%s", &pass);
	printf("Trying to connect...\n");
	EXEC SQL connect to students@fpm2.ami.nstu.ru user :login using :pass;
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
	EXEC SQL set search_path to :scheme;
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
	EXEC SQL prepare t_sum from :text_query1;
	if (sqlca.sqlcode < 0) {
		errorHandler("Prepare", sqlca.sqlcode);
		query1_ready = false;
	} else printf("Done\n");	
	//
	//готовим второй запрос
	printf("Preparing query2...\n");
	EXEC SQL prepare query2_res from :text_query2;
	if (sqlca.sqlcode < 0) {
		errorHandler("Prepare", sqlca.sqlcode);
		query2_ready = false;
	} else printf("Done\n");
	//
	//готовим третий запрос
	printf("Preparing query3...\n");
	EXEC SQL prepare query3_res from :text_query3;
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
			EXEC SQL disconnect all;
			printf("Session closed\n");
			break;
		default:
			printf("Chose 1-4 numbers!\n");
			break;
		}
	} while (flag == 0);
	return;
}
