#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "skill-tree.h"


#define MAX_PURCHASED_SKILLS 15
#define MAX_SIZE 100
#define MAX_TREE_SKILLS 5

struct Node {
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    const char* name;
    const char* description;
    int cost;
    bool purchased;
};


typedef struct NodeArgs {
    const char* name;
    const char* description;
    int cost;
} NodeArgs;


typedef struct User {
    int coins;
    unsigned long id;
    const char* name;
    const char* purchased_skills[MAX_PURCHASED_SKILLS];
    int ptr;
} User;

User* create_user(int coins, unsigned long id, const char* name) {
    User* usr = malloc(sizeof(User));

    if (!usr) {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    usr->coins = coins;
    usr->id = id;
    usr->name = name;
    usr->ptr = 0;
    
    for (int i = 0; i < MAX_PURCHASED_SKILLS; i++) {
        usr->purchased_skills[i] = NULL;
    }

    return usr;
}


struct Node* create_node(NodeArgs args) {
    struct Node* newnode = malloc(sizeof(struct Node));

    if (!newnode) {
        printf("Failed to allocate memory.\n");
        exit(1);
    }
       
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = NULL;
    newnode->name = args.name;
    newnode->description = args.description;
    newnode->cost = args.cost;
    newnode->purchased = false; 

    return newnode;
}


struct Node* init_agility_tree() {
    struct Node* quick_step = create_node((NodeArgs){
        .name = "Quick Step",
        .description = "Basic burst of speed and nimbleness.",
        .cost = 1,
    });

    struct Node* fleet_foot = create_node((NodeArgs){
        .name = "Fleet Foot",
        .description = "Swift evasive movement, almost vanishing in motion.",
        .cost = 2,
    });

    struct Node* shadow_dash = create_node((NodeArgs){
        .name = "Shadow Dash",
        .description = "Swift evasive movement, almost vanishing in motion.",
        .cost = 2,
    });

    struct Node* phantom_stride = create_node((NodeArgs){
        .name = "Phantom Stride",
        .description = "Blindingly fast, leaving afterimages behind.",
        .cost = 3,
    });
    
    struct Node* temporal_shift = create_node((NodeArgs){
        .name = "Temporal Shift",
        .description = "Agility so great it feels like bending time itself.",
        .cost = 3,
    });

    quick_step->left = fleet_foot;
    fleet_foot->parent = quick_step;
    
    quick_step->right = shadow_dash;
    shadow_dash->parent = quick_step;

    fleet_foot->left = phantom_stride;
    phantom_stride->parent = fleet_foot;

    shadow_dash->right = temporal_shift;
    temporal_shift->parent = shadow_dash;

    return quick_step;
}


struct Node* init_power_tree() {
    struct Node* iron_grip = create_node((NodeArgs){
        .name = "Iron Grip",
        .description = "Your strikes gain a crushing force, increasing weapon stability and damage by 10%.",
        .cost = 1,
    });

    struct Node* mighty_blow = create_node((NodeArgs){
        .name = "Mighty Blow",
        .description = "A devastating overhead attack that deals heavy single-target damage and staggers enemies.",
        .cost = 2,
    });

    struct Node* titans_endurance = create_node((NodeArgs){
        .name = "Titan's Endurance",
        .description = "Harness raw power to endure longer in battle, reducing damage taken by 15%.",
        .cost = 2,
    });

    struct Node* berserkers_wrath = create_node((NodeArgs){
        .name = "Berserker's Wrath",
        .description = "Enter a furious state where each hit boosts your attack speed, chaining faster combos.",
        .cost = 3,
    });

    struct Node* colossus_ascendancy = create_node((NodeArgs){
        .name = "Colossus Ascendancy",
        .description = "Unleash ultimate power, doubling damage for a short duration while becoming immune to knockback.",
        .cost = 4,
    });

    iron_grip->left = mighty_blow;
    mighty_blow->parent = iron_grip;

    iron_grip->right = titans_endurance;
    titans_endurance->parent = iron_grip;

    mighty_blow->left = berserkers_wrath;
    berserkers_wrath->parent = mighty_blow;

    titans_endurance->right = colossus_ascendancy;
    colossus_ascendancy->parent = titans_endurance;

    return iron_grip;
}


struct Node* init_survivor_tree() {
    struct Node* stone_skin = create_node((NodeArgs){
        .name = "Stone Skin",
        .description = "Toughen your body, gaining +10% resistance to physical damage.",
        .cost = 1,
    });

    struct Node* second_wind = create_node((NodeArgs){
        .name = "Second Wind",
        .description = "Once per battle, recover a small portion of health when falling below 20%.",
        .cost = 1,
    });

    struct Node* foragers_instinct = create_node((NodeArgs){
        .name = "Forager's Instinct",
        .description = "Improve survival efficiency — gain extra resources from the environment and reduced stamina cost while gathering.",
        .cost = 2,
    });

    struct Node* unyielding_spirit = create_node((NodeArgs){
        .name = "Unyielding Spirit",
        .description = "Your sheer willpower keeps you standing, reducing the effects of stuns and status ailments.",
        .cost = 3,
    });

    struct Node* last_stand = create_node((NodeArgs){
        .name = "Last Stand",
        .description = "In the face of death, unleash hidden strength: massive defense boost and health regeneration for a short time.",
        .cost = 4,
    });

    stone_skin->left = second_wind;
    second_wind->parent = stone_skin;

    stone_skin->right = foragers_instinct;
    foragers_instinct->parent = stone_skin;

    second_wind->left = unyielding_spirit;
    unyielding_spirit->parent = second_wind;

    foragers_instinct->right = last_stand;
    last_stand->parent = foragers_instinct;

    return stone_skin;
}


const char** show_purchased_skills(User* usr) {
    if (usr == NULL) return NULL;

    return usr->purchased_skills;
}


const char** show_skill_tree(struct Node* skill_tree) {
    const char** skill_nodes = malloc(sizeof(const char*) * MAX_TREE_SKILLS);

    struct Node* queue[MAX_SIZE];
    struct Node* curr = skill_tree;
    int rear, front, ptr;
    rear = front = ptr = 0;

    queue[rear++] = curr; 
    while (rear > front) {
        struct Node* temp = queue[front++];
    
        skill_nodes[ptr++] = temp->name;
        if (temp->left) queue[rear++] = temp->left;
        if (temp->right) queue[rear++] = temp->right;
    }

    return skill_nodes;
}


struct Node* find_node(struct Node* skill_tree, const char* skill_name) {
  struct Node* queue[MAX_SIZE];
  struct Node* curr = skill_tree;
  struct Node* skill = NULL;
  int rear, front;

  rear = front = 0;
  queue[rear++] = curr; 

  while (rear > front) {
    struct Node* temp = queue[front++];

    if (strcmp(temp->name, skill_name) == 0) {
      skill = temp;
      break;
    }

    if (temp->left) queue[rear++] = temp->left;
    if (temp->right) queue[rear++] = temp->right;
  }

  return skill;
}


bool has_skill(struct Node* skill_tree, const char* skill_name, User* usr, int* i) {
    if (usr == NULL) return false;

    for (*i = 0; *i < MAX_PURCHASED_SKILLS; (*i)++) {
        if (strcmp(usr->purchased_skills[(*i)], skill_name) == 0) {
            return true; 
        }
    }

    return false;
}

bool can_purchase_skill(struct Node* skill_tree, const char* name, User* usr) {
    if (usr == NULL) return false;
 
    struct Node* skill = NULL;

    struct Node* queue[MAX_SIZE];
    struct Node* curr = skill_tree;
    int front, rear;

    front = rear = 0;
    queue[rear++] = curr;

    while (rear > front) {
        struct Node* temp = queue[front++];

        if (strcmp(name, temp->name) == 0) {
            skill = temp;
            break;
        }

        if (temp->left) queue[rear++] = temp->left;
        if (temp->right) queue[rear++] = temp->right;
    }


    if (skill == NULL) return false;
  
    for (int i = 0; i < MAX_PURCHASED_SKILLS; i++) {
      if (usr->purchased_skills[i] == NULL) break;
      if (strcmp(usr->purchased_skills[i], skill->name) == 0) return false;
    }

    if (usr->coins >= skill->cost) {
        return true;
    }
    
    else return false;
}


bool purchase_skill(struct Node* skill_tree, const char* skill_name, User* usr) {
    if (usr == NULL) return false;

    struct Node* queue[MAX_SIZE];
    struct Node* curr = skill_tree;
    struct Node* skill = NULL;

    int rear, front;
    
    rear = front = 0;
    queue[rear++] = curr;

    while (rear > front) {
        struct Node* temp = queue[front++];

        if (strcmp(skill_name, temp->name) == 0) {
            skill = temp;
            break;
        }

        if (temp->left) queue[rear++] = temp->left;
        if (temp->right) queue[rear++] = temp->right;
    }
    
    if (!skill) return false;

     if (usr->coins >= skill->cost) {
        usr->coins -= skill->cost;
        usr->purchased_skills[usr->ptr++] = skill->name;
        printf("Successfully purchased skill \"%s\"\n", skill->name);
        return true;
    }

     else {
       printf("Failed to buy skill \"%s\"\n", skill->name);
       return false;
    }
}


bool sell_skill(struct Node* skill_tree, const char* skill_name, User* usr) {
    if (usr == NULL) return false;

    struct Node* queue[MAX_SIZE];
    struct Node* curr = skill_tree;
    struct Node* skill = NULL;

    int rear, front;
    
    rear = front = 0;
    queue[rear++] = curr;

    while (rear > front) {
        struct Node* temp = queue[front++];

        if (strcmp(skill_name, temp->name) == 0) {
            skill = temp;
            break;
        }

        if (temp->left) queue[front++] = temp->left;
        if (temp->right) queue[front++] = temp->right;
    }

    if (!skill) return false;
    
    int i;
    bool found = has_skill(skill_tree, skill_name, usr, &i);
    if (!found) return false;
    
    else {
        usr->coins += skill->cost;
        usr->purchased_skills[i] = usr->purchased_skills[usr->ptr];
        usr->purchased_skills[usr->ptr] = NULL;
        usr->ptr--;
    } 

    return true;
}
