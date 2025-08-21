#ifndef SKILL_TREE_H
#define SKILL_TREE_H

#include <stdio.h>
#include <stdbool.h>

struct Node; 

typedef struct User User;

typedef struct NodeArgs NodeArgs;

User* create_user(int coins, unsigned long id, const char* name);

struct Node* create_node(NodeArgs args);

struct Node* init_agility_tree();

struct Node* init_power_tree();

struct Node* init_survivor_tree();

const char** show_purchased_skills(struct Node* skill_tree, User* usr);

const char** show_skill_tree(struct Node* skill_tree);

bool can_purchase_skill(struct Node* skill_tree, const char* skill, User* usr);

bool purchase_skill(struct Node* skill_tree, const char* skill, User* usr); 

bool sell_skill(struct Node* skill_tree, const char* skill, User* usr);


#endif // SKILL_TREE_H
