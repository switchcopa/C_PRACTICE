#ifndef SKILL_TREE_H
#define SKILL_TREE_H

#include <stdio.h>
#include <stdbool.h>

struct Node; 

typedef struct NodeArgs NodeArgs;

struct Node* create_node(NodeArgs args);

struct Node* init_agility_tree();

struct Node* init_power_tree();

struct Node* init_survivor_tree();

const char** show_purchased_skills(struct Node* skill_tree);

const char** show_skill_tree(struct Node* skill_tree);

bool can_purchase_skill(struct Node* skill_tree, const char* skill);

bool purchase_skill(struct Node* skill_tree, const char* skill); 

bool sell_skill(struct Node* skill_tree, const char* skill);


#endif // SKILL_TREE_H
