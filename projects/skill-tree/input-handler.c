#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "skill-tree.h"
#include "skill-tree.c"

#define MAX_INPUT_SIZE 100
#define SKILL_TREE_SIZE 5
#define MAX_PURCHASED_SKILLS 15
#define MAX_TREE_SKILLS 5

struct Node* handle_user_tree_choice() {
  struct Node* skill_tree = NULL;

  printf("Choose Skill Tree:\n");

  printf("1. Agility Tree\n");
  printf("2. Survivor Tree\n");
  printf("3. Power Tree\n");

  int choice;
  scanf("%d", &choice);

  switch (choice) {
    case 1: 
      skill_tree = init_agility_tree();
      break;
    case 2: 
      skill_tree = init_survivor_tree();
      break;
    case 3: 
      skill_tree = init_power_tree();
      break;
    default: 
      printf("Unknown choice\n");
  }

  return skill_tree;
}

const char* handle_user_skill_choice(struct Node* skill_tree) {
  printf("Choose skill:\n");
  const char** skill_array = show_skill_tree(skill_tree);

  int i;
  for (i = 1; i < SKILL_TREE_SIZE + 1; i++) {
    printf("%d. %s\n", i, skill_array[i-1]);
  }

  int choice;
  scanf("%d", &choice);

  return skill_array[choice - 1];
}

bool handle_user_purchases(User* usr) {
  struct Node* skill_tree = handle_user_tree_choice();
  const char* skill_name = handle_user_skill_choice(skill_tree); 

  bool allow_to_purchase = can_purchase_skill(skill_tree, skill_name, usr);

  if (allow_to_purchase) {
    purchase_skill(skill_tree, skill_name, usr);
    return true;
  }

  else return false;
}

bool handle_user_sell(User* usr) {
  struct Node* skill_tree = handle_user_tree_choice();
  const char* skill_name = handle_user_skill_choice(skill_tree);
  bool allow_to_sell = sell_skill(skill_tree, skill_name, usr); 
  
  if (allow_to_sell) {
    printf("Successfully sold skill!\n");
    return true;
  }

  else {
    printf("Failed to sell skill\n");
    return false;
  }
}

User* handle_user_creation() {
  printf("Enter your username: ");

  char buffer_name[MAX_INPUT_SIZE];
  scanf("%s", buffer_name);
  const char* username = buffer_name; 
  
  printf("\n");
  srand(time(NULL)); 
  
  int coins = 0;
  unsigned long id; 
  id = rand();
  User* usr = create_user(coins, id, buffer_name);

  if (usr == NULL) {
    printf("Failed to create user\n");
    return NULL;
  }

  else {
    printf("Successfully created user!\n");
    return usr;
  }
}

void handle_user_input() {
  User* usr = handle_user_creation();

  if (usr == NULL) return;

  while (1) {
    printf("==== Your coins: %d ====\nSelect an option:\n", usr->coins);
    printf("1. Purchase a skill\n");
    printf("2. Show purchased skills\n");
    printf("3. Show skills within a tree\n");
    printf("4. Sell a skill\n");
    printf("5. Add coins\n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
      case 1: 
        handle_user_purchases(usr);
        break;
      case 2: 
        const char** purchased_skills = show_purchased_skills(usr);

        for (int i = 0; i < MAX_PURCHASED_SKILLS; i++) {
          if (purchased_skills[i] == NULL) {
            continue; 
          }

          else printf("%s\n", purchased_skills[i]);
        }
         break;
      case 3: 
        struct Node* skill_tree = handle_user_tree_choice();
        const char** skill_nodes = show_skill_tree(skill_tree);

        for (int i = 0; i < MAX_TREE_SKILLS; i++) {
          printf("%s\n", skill_nodes[i]);
        }
        break;
      case 4: 
        handle_user_sell(usr);
        break;
      case 5: 
        printf("Choose number of coins to add: ");
        
        int amount;
        scanf("%d", &amount);
        
        usr->coins += amount;
        printf("Successfully added coins!\n\n");
        break;
      default:
        printf("Unknown choice\n\n");
    }
  }
}
