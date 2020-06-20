
#define TRUE 1
#define FALSE 0
#define MAX 3

void clear_screen();
void start_menu();
int cities_options();
void gen_report();
int menu_option();
int city_is_valid(int option);
void invalid_option_message();
int buy_op();
int cities_available(int id);
char find_schedulee(int id);
int schedule_choice();
char *find_city(int id);
int schedule_is_valid(int option);
int find_princing(int city_id);
void resume();
double amount (int qtdy, double price);
struct Route get_total(struct Route route);
int is_valid_qty(int quantity );
void success();
int confirm();
void line_wrap();
int ticket_choice();
void total_amount(double val);
void finalize(struct Route r);
void report();
