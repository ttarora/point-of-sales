#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "pos.h"

struct Route {
  char *origin;
  char *destiny;
  float price;
  char *schedule;
  double amount;
  int quantity;
};

void line_wrap(){
  printf("\n");
}

int menu_option(){
  int resp;
  printf("Digite uma opcao: ");
  scanf("%d", &resp);

  return resp;
}

char *find_schedule(int id){
  char *sdles[] = {"07:00 - 08:00", "12:00 - 13:00", "16:00 - 18:00"};
  int i;
  int sel  = (id - 1);

  for(i = 0; i < MAX; i++){
    if(i == sel) {
      return (char *)sdles[i];
    }
  }
  return NULL;
}

int schedule_choice() {
  line_wrap();

  printf("Escolha o horario de partida \n");

  char *sdles[] = {"07:00 - 08:00", "12:00 - 13:00", "16:00 - 18:00"};
  int i;

  for(i = 0; i < MAX; i++){
    printf("[%d] %s\n",(i+1), sdles[i]);
  }
  int resp = menu_option();

  if(!schedule_is_valid(resp)){
    invalid_option_message();
    return schedule_choice();
  }
  return resp;
}

char *find_city(int id) {
  char *cities[] = {"Cruzeiro", "Lorena","Cachoeitra Paulista"};

  int sel  = (id - 1);
  int i;

  for(i = 0; i < MAX; i++){
    if(i == sel) {
      return (char *)cities[i];
    }
  }
  return NULL;
}

int options_is_valid(int option) {
  if(option >= 1 && option <= 3 ) {
    return TRUE;
  } else {
    return FALSE;
  }
}


void start_menu() {
  printf("-------------------------\n");
  char *menu[] = {"Efetuar Compra", "Gerar Relatorio","Sair"};
  int i;

  for(i=0; i < MAX; i++){
    printf("[%d] %s\n",(i+1), menu[i]);
  }
  printf("-------------------------\n");

  int resp = menu_option();

  if(!options_is_valid(resp))
    start_menu();

  switch(resp){
    case 1:
        buy_op();
        break;
    case 2:
        report();
        break;
    case 3:
        exit(0);
        clear_screen();
        break;
    default:
        invalid_option_message();
        start_menu();
  }
}

float find_pricing(int city_id) {
  switch(city_id){
    case 1:
        return 10.00;
        break;
    case 2:
        return 20.00;
        break;
    case 3:
        return 15.00;
        break;
    default:
        invalid_option_message();
        find_pricing(city_id);
  }

  return 0.0;
}

int buy_op(){

  struct Route r;

  int city_origin = cities_options();
  int destiny = cities_available(city_origin);
  int schedule = schedule_choice();

  r.price = find_pricing(city_origin);
  r.origin = find_city(city_origin);
  r.destiny = find_city(destiny);
  r.schedule = find_schedule(schedule);

  resume(r);
  r.quantity = ticket_choice();

  finalize(r);

  struct Route route = get_total(r);

  if(confirm()){
    gen_report(route);
    success();
    start_menu();
  }

  return 0;
}

double amount(int qtdy, double price){
  return qtdy * price;
}

int is_valid_qty(int quantity){
  if(quantity >= 0){
     return TRUE;
  } else {
    return FALSE;
  }
}

int ticket_choice(){
  int qty;

  line_wrap();
  printf("Quantas passagens deseja comprar?: ");
  scanf("%d", &qty);

  if (!is_valid_qty(qty)){
     invalid_option_message();
     return ticket_choice();
  }
  return qty;
}

void finalize(struct Route route){
  struct Route r  = get_total(route);
  printf("Valor total: %.2lf\n", r.amount);
}

struct Route get_total(struct Route route) {
  double total = amount(route.quantity, route.price);
  route.amount = total;
  return route;
}


void success() {
  printf("Compra efetuada com sucesso!\n");
  line_wrap();
  start_menu();
}

int confirm() {
  char yesno;

  printf("Deseja confirmar a compra?(s|n): ");
  scanf("%s", &yesno);

  switch(yesno){
    case 's':
        return TRUE;
        break;
    case 'S':
        return TRUE;
        break;
    case 'n':
        return FALSE;
        break;
    case 'N':
        return FALSE;
        break;
    default:
        invalid_option_message();
        confirm();
        break;
  }

  return TRUE;
}

void resume(struct Route route){
  printf("\n");
  printf("Origin: %s \n", route.origin);
  printf("Destino: %s \n", route.destiny);
  printf("Itinerario: %s \n", route.schedule);
  printf("Preco: %.2f \n", route.price);
}

void invalid_option_message(){
  printf("Opcao invalida\n");
  line_wrap();
}

void clear_screen(){
  system("clear");
}

int city_is_valid(int option) {
  if(option >= 1 && option <= 3 ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int schedule_is_valid(int option) {
  if(option >= 1 && option <=3 ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int cities_available(int id) {
  line_wrap();

  printf("Selecione a cidade de destino\n");
  char *cities[] = {"Cruzeiro", "Lorena","Cachoeitra Paulista"};
  int removable = (id - 1);
  int i;

  for(i = 0; i < MAX; i++){
    if(i != removable) {
      printf("[%d] %s\n",(i+1), cities[i]);
    }
  }
  int resp = menu_option();

  if(!city_is_valid(resp) || resp == id){
    invalid_option_message();
    return cities_available(id);
  }
  return resp;
}

int cities_options() {
  line_wrap();

  printf("Selecione a cidade de origem\n");
  char *cities[] = {"Cruzeiro", "Lorena","Cachoeitra Paulista"};
  int i;

  for(i = 0; i < MAX; i++){
     printf("[%d] %s\n",(i+1), cities[i]);
  }
  int resp = menu_option();

  if(!city_is_valid(resp)){
    invalid_option_message();
    return cities_options();
  }
  return resp;
}

void gen_report(struct Route route){
  FILE *f;

  f = fopen("ticket.dat", "a+");

  fprintf(f, "%s x %s = R$ %.2lf\n", route.origin, route.destiny, route.amount);

  fclose(f);
}

void report(){
  line_wrap();
  printf("Relatorio de passagens\n\n");

  char data[50];
  FILE *f;

  f = fopen("ticket.dat", "r");

  while((fgets(data, sizeof(data), f)) != NULL){
    printf("%s", data);
  }

  line_wrap();
  fclose(f);
}
